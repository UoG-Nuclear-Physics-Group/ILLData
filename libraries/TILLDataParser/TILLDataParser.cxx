#include "TILLDataParser.h"
#include "TILLDataParserException.h"

#include "TChannel.h"
#include "Globals.h"

#include "TScalerQueue.h"

#include "TEpicsFrag.h"
#include "TParsingDiagnostics.h"

#include "Rtypes.h"

#include "TFragment.h"
#include "TBadFragment.h"

#include "TILLMnemonic.h"

TILLDataParser::TILLDataParser()
   : TDataParser()
{
	fState = EDataParserState::kGood;
}

TILLDataParser::~TILLDataParser()
{
}

int TILLDataParser::Process(std::shared_ptr<TRawEvent> rawEvent)
{
   std::shared_ptr<TLstEvent> event = std::static_pointer_cast<TLstEvent>(rawEvent);
   /// Process this TLstEvent using the provided data parser.
   /// Returns the total number of fragments read (good and bad).
   // right now the parser only returns the total number of fragments read
   // so we assume (for now) that all fragments are good fragments
   int EventsProcessed = 0;
   uint32_t size = event->GetDataSize();
   uint32_t* EvntData = reinterpret_cast<uint32_t*>(event->GetData());

   IncrementInputSize(event->GetDataSize()/16); // 16 bytes per event, number of bytes in 32bits

	for(size_t i = 0; i + 3 < size/4; i += 4) {
		if(event->GetLstVersion() == 1) {
         EventsProcessed += V1SingleFippsEventToFragment(EvntData + i);
		} else {
         EventsProcessed += V2SingleFippsEventToFragment(EvntData + i);
		}
		IncrementItemsPopped();
		DecrementInputSize();
	}

   return EventsProcessed;
}

int TILLDataParser::V1SingleFippsEventToFragment(uint32_t* data)
{
   std::shared_ptr<TFragment> eventFrag       = std::make_shared<TFragment>();
   Long64_t                   tmpTimestamp;

	// address is 4 bit crate, 6 bit board, and 6 bit channel
   eventFrag->SetAddress((data[0] >> 16) & 0xffff);

   // Rollover, constant beween boards
   tmpTimestamp = data[0] & 0xffff;

   // Concatenate timestamp informatioan based on board type
	if(eventFrag->GetChannel() != nullptr) {
		switch(eventFrag->GetChannel()->GetDigitizerType()) {
			case EDigitizer::kV1724: 
				tmpTimestamp = tmpTimestamp<<30;
				tmpTimestamp |= data[1] & 0x3fffffff; // 30 bit timestamp
				break;
			case EDigitizer::kV1725:
				tmpTimestamp = tmpTimestamp<<31;
				tmpTimestamp |= data[1] & 0x7fffffff; // 31 bit timestamp
				break;
			default:
				tmpTimestamp = tmpTimestamp<<32;
				tmpTimestamp |= data[1] & 0xffffffff; // 32 bit timestamp
				break;
		}
	} else {
		std::cerr<<DRED<<"Failed to find channel for address "<<hex(eventFrag->GetAddress(),4)<<" using default 32bit timestamp format, times might be wrong for this channel!"<<std::endl;
		tmpTimestamp = tmpTimestamp<<32;
		tmpTimestamp |= data[1] & 0xffffffff; // 32 bit timestamp
	}
	eventFrag->SetTimeStamp(tmpTimestamp);

	int32_t Charge = (data[2] & 0x7fff);
	// Discriminate bad fragments
	if(Charge == 0 || Charge == 0x8000) {
		if(RecordDiag()) {
			TParsingDiagnostics::Get()->BadFragment(99);
		}
		Push(*BadOutputQueue(), std::make_shared<TBadFragment>(*eventFrag, data, 4, 2, false));
		return 1;
	}
	// Good event
	eventFrag->SetCharge(static_cast<int32_t>(Charge));
	if(RecordDiag()) {
		TParsingDiagnostics::Get()->GoodFragment(eventFrag);
	}
	Push(GoodOutputQueues(), std::make_shared<TFragment>(*eventFrag));
	return 1;
}

int TILLDataParser::V2SingleFippsEventToFragment(uint32_t* data)
{
	std::shared_ptr<TFragment> eventFrag       = std::make_shared<TFragment>();
	Long64_t                   tmpTimestamp;

	eventFrag->SetAddress((data[0] >> 16) & 0xffff);

	// Rollover, constant beween boards
	tmpTimestamp = data[0] & 0xffff;
	tmpTimestamp = tmpTimestamp<<32;
	tmpTimestamp |= data[1] & 0xffffffff; // 32 bit timestamp
	eventFrag->SetTimeStamp(tmpTimestamp);

	int32_t Charge = (data[2] & 0x7fff);
	// Discriminate bad fragments
	if(Charge == 0 || Charge == 0x8000) {
		if(RecordDiag()) {
			TParsingDiagnostics::Get()->BadFragment(99);
		}
		Push(*BadOutputQueue(), std::make_shared<TBadFragment>(*eventFrag, data, 4, 2, false));
		return 1;
	}
	// Good event
	eventFrag->SetCharge(static_cast<int32_t>(Charge));
	if(RecordDiag()) {
		TParsingDiagnostics::Get()->GoodFragment(eventFrag);
	}
	Push(GoodOutputQueues(), std::make_shared<TFragment>(*eventFrag));
	return 1;
}
