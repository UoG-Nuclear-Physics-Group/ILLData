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
   /// Process this TLstEvent using the provided data parser.
   /// Returns the total number of fragments read (good and bad).
   // right now the parser only returns the total number of fragments read
   // so we assume (for now) that all fragments are good fragments
	std::shared_ptr<TLstEvent> event = std::static_pointer_cast<TLstEvent>(rawEvent);
   return FippsToFragment(event->GetData(), event->GetDataSize());
}

int TILLDataParser::FippsToFragment(char* data, uint32_t size)
{
   uint32_t* ptr = reinterpret_cast<uint32_t*>(data);

   int                        totalEventsRead = 0;
   std::shared_ptr<TFragment> eventFrag       = std::make_shared<TFragment>();
   Long64_t                   tmpTimestamp;
   if(fItemsPopped != nullptr && fInputSize != nullptr) {
      *fItemsPopped = 0;
      *fInputSize   = size/16; //4 words of 4 bytes for each event
   }

   // *=* LST File Parsing *=* //
   // This section is for reading in V1 .lst files of a C1725 board
   // TODO: Change this to read in both V1 and V2 type files, as well as all possible DAQ boards at ILL.
   // This reqires reading the header at the beginning of the .lst file to find the version.

   // we read 4 words for each event, and size is in bytes, so we need to divide it by 4 (size of uint32_t)
   for(size_t i = 0; i + 3 < size / 4; i += 4) {
      if(fItemsPopped != nullptr && fInputSize != nullptr) {
         ++(*fItemsPopped);
         --(*fInputSize);
      }

      // Crate: (buffer >> 28) & 0xF (4 Bits)
      // Board: (buffer >>  22) & 0x3F (6 Bits)
      EDigitizer Board = static_cast<EDigitizer>(( ptr[i] >> 22) & 0x3F);
      
      // Channel: (Buffer >> 16) & 0x3F (6 Bits)
      eventFrag->SetAddress( (ptr[i] >> 16) & 0xffff );

      //*=* Timestamp bits *=*//

      // Rollover, constant beween boards
      tmpTimestamp = ptr[i] & 0xffff;
      tmpTimestamp = tmpTimestamp<<31;

      // Concatonate timestamp informatioan
      if( Board == EDigitizer::kV1724 ) {
          // Timestamp bits is 30
          tmpTimestamp |= ptr[ i + 1 ] & 0x3FFFFFFF;
      } else if ( Board == EDigitizer::kV1725_PHA ) {
          // Timestamp bits is 31
          tmpTimestamp |= ptr[i + 1] & 0x7fffffff;
      } else {
          tmpTimestamp |= ptr[i + 1] & 0xffffffff;
      }
      eventFrag->SetTimeStamp(tmpTimestamp);

      ++totalEventsRead;

      //*=* Charge Information *=*//
      int32_t Charge = (ptr[i+2] & 0x7FFF);

      // Discriminate bad fragments
      if( Charge == 0 || Charge == 0x8000 ) {
         if(fRecordDiag) {
            TParsingDiagnostics::Get()->BadFragment(99);
         }
         // Push(*fBadOutputQueue, std::make_shared<TBadFragment>(*eventFrag, ptr, size / 4, i + 2, false));
         continue;
      }
      eventFrag->SetCharge(static_cast<int32_t>(Charge) );
      if(fRecordDiag) {
         TParsingDiagnostics::Get()->GoodFragment(eventFrag);
      }
      Push(fGoodOutputQueues, std::make_shared<TFragment>(*eventFrag));
      // std::cout<<totalEventsRead<<": "<<eventFrag->Charge()<<", "<<eventFrag->GetTimeStamp()<<std::endl;
   }

   return totalEventsRead;
}

