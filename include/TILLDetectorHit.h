#ifndef TILLDETECTORHIT_H
#define TILLDETECTORHIT_H

/** \addtogroup Detectors
 *  @{
 */

#include "Globals.h"

#include "TDetectorHit.h"
#include "TFragment.h"

/////////////////////////////////////////////////////////////////
///
/// \class TILLDetectorHit
///
/// This class overloads the GetTime function of TDetectorHit
/// and uses the TILLMnemonic detectory type enumeration to
/// calculate the time.
///
/////////////////////////////////////////////////////////////////

class TILLDetectorHit : public TDetectorHit {

   //
public:
   TILLDetectorHit(const int& Address = 0xffffffff);
   TILLDetectorHit(const TILLDetectorHit&, bool copywave = true);
	TILLDetectorHit(const TFragment& frag) : TDetectorHit(frag) {}
   ~TILLDetectorHit() override;

   virtual Double_t GetTime(const ETimeFlag& correct_flag = ETimeFlag::kAll,
                            Option_t*     opt          = "") const override; ///< Returns a time value to the nearest nanosecond!
	
   /// \cond CLASSIMP
   ClassDefOverride(TILLDetectorHit, 1) // Stores the information for a detector hit
   /// \endcond
};
/*! @} */
#endif
