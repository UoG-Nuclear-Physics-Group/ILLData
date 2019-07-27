#ifndef TILLDARAPARSER_H
#define TILLDARAPARSER_H

/** \addtogroup Sorting
 *  @{
 */

/////////////////////////////////////////////////////////////////
///
/// \class TILLDataParser
///
/// The TILLDataParser is the DAQ dependent part of GRSISort.
/// It takes a "DAQ-dependent"-flavoured LST file and
/// converts it into a generic TFragment that the rest of
/// GRSISort can deal with. This is where event word masks
/// are applied, and any changes to the event format must
/// be implemented.
///
/////////////////////////////////////////////////////////////////

#include "Globals.h"
#include <ctime>
#include <sstream>
#include <vector>
#include <map>
#include <limits>

#ifndef __CINT__
#include <memory>
#endif

#include "TDataParser.h"
#include "TChannel.h"
#include "TFragment.h"
#include "TPPG.h"
#include "TScaler.h"
#include "TFragmentMap.h"
#include "ThreadsafeQueue.h"
#include "TEpicsFrag.h"
#include "TRawEvent.h"
#include "TLstEvent.h"

class TILLDataParser : public TDataParser {
public:
   TILLDataParser();
   ~TILLDataParser();

   // ENUM(EBank, char, kWFDN,kGRF1,kGRF2,kGRF3,kFME0,kFME1,kFME2,kFME3);
   enum class EBank { kWFDN = 0, kGRF1 = 1, kGRF2 = 2, kGRF3 = 3, kGRF4 = 4, kFME0 = 5, kFME1 = 6, kFME2 = 7, kFME3 = 8 };

   enum class EDigitizer {
       kV1751 = 1,
       kV1724 = 2,
       kV1730_PSD = 3,
       kV1730_PHA = 4,
       kV1725_PHA = 7,
       kV1725_PSD = 8
   };

   enum class EDataParserState {
      kGood,
      kBadHeader,
      kMissingWords,
      kBadScalerLowTS,
      kBadScalerValue,
      kBadScalerHighTS,
      kBadScalerType,
      kBadTriggerId,
      kBadLowTS,
      kBadHighTS,
      kSecondHeader,
      kWrongNofWords,
      kNotSingleCfd,
      kSizeMismatch,
      kBadFooter,
      kFault,
      kMissingPsd,
      kMissingCfd,
      kMissingCharge,
      kBadBank,
      kBadModuleType,
      kEndOfData,
      kUndefined
   };

#ifndef __CINT__
	int Process(std::shared_ptr<TRawEvent>) override;
#endif

private:
	int FippsToFragment(char* data, uint32_t size);

	EDataParserState fState;
};
/*! @} */
#endif
