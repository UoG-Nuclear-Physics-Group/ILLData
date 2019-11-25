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

   if( fInputSize != nullptr )
      *fInputSize += event->GetDataSize()/16; // 16 bytes per event, number of bytes in 32bits

   if( event->GetLstVersion() == 1 ) {
      for( size_t i = 0; i + 3 < size/4; i += 4 ) {
         EventsProcessed += V1SingleFippsEventToFragment(EvntData + i);
         if(fItemsPopped != nullptr && fInputSize != nullptr) {
            ++(*fItemsPopped);
            --(*fInputSize);
         }
      }
   } else {
      for( size_t i = 0; i + 3 < size / 4; i += 4 ) {
         EventsProcessed += V2SingleFippsEventToFragment(EvntData + i);
         if(fItemsPopped != nullptr && fInputSize != nullptr) {
            ++(*fItemsPopped);
            --(*fInputSize);
         }
      }
   }

   return EventsProcessed;
}

int TILLDataParser::V1SingleFippsEventToFragment(uint32_t* data)
{
   std::shared_ptr<TFragment> eventFrag       = std::make_shared<TFragment>();
   Long64_t                   tmpTimestamp;

   eventFrag->SetAddress( (data[0] >> 16) & 0xffff );

   // Rollover, constant beween boards
   tmpTimestamp = data[0] & 0xffff;

   // Concatonate timestamp informatioan based on board type
   switch( static_cast<EDigitizer>((data[0] >> 22) && 0x3F) ) {
      case EDigitizer::kV1724: 
         tmpTimestamp = tmpTimestamp<<30;
         tmpTimestamp |= data[1] & 0x3ffffffF; // 30 bit timestamp
      case EDigitizer::kV1725_PHA:
         tmpTimestamp = tmpTimestamp<<31;
         tmpTimestamp |= data[1] & 0x7ffffffF; // 31 bit timestamp
      default:
         tmpTimestamp = tmpTimestamp<<32;
         tmpTimestamp |= data[1] & 0xffffffff; // 32 bit timestamp
   }
   eventFrag->SetTimeStamp(tmpTimestamp);

   int32_t Charge = (data[2] & 0x7fff);
   // Discriminate bad fragments
   if( Charge == 0 || Charge == 0x8000 ) {
      if(fRecordDiag) {
         TParsingDiagnostics::Get()->BadFragment(99);
      }
      Push(*fBadOutputQueue, std::make_shared<TBadFragment>(*eventFrag, data, 4, 2, false));
      return 1;
   }
   // Good event
   eventFrag->SetCharge(static_cast<int32_t>(Charge) );
   if(fRecordDiag) {
      TParsingDiagnostics::Get()->GoodFragment(eventFrag);
   }
   Push(fGoodOutputQueues, std::make_shared<TFragment>(*eventFrag));
   return 1;
}

int TILLDataParser::V2SingleFippsEventToFragment(uint32_t* data)
{
   std::shared_ptr<TFragment> eventFrag       = std::make_shared<TFragment>();
   Long64_t                   tmpTimestamp;

   eventFrag->SetAddress( (data[0] >> 16) & 0xffff );

   // Rollover, constant beween boards
   tmpTimestamp = data[0] & 0xffff;
   tmpTimestamp = tmpTimestamp<<32;
   tmpTimestamp |= data[1] & 0xffffffff; // 32 bit timestamp
   eventFrag->SetTimeStamp(tmpTimestamp);

   int32_t Charge = (data[2] & 0x7fff);
   // Discriminate bad fragments
   if( Charge == 0 || Charge == 0x8000 ) {
      if(fRecordDiag) {
         TParsingDiagnostics::Get()->BadFragment(99);
      }
      Push(*fBadOutputQueue, std::make_shared<TBadFragment>(*eventFrag, data, 4, 2, false));
      return 1;
   }
   // Good event
   eventFrag->SetCharge(static_cast<int32_t>(Charge) );
   if(fRecordDiag) {
      TParsingDiagnostics::Get()->GoodFragment(eventFrag);
   }
   Push(fGoodOutputQueues, std::make_shared<TFragment>(*eventFrag));
   return 1;
}
