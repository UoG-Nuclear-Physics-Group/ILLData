#ifndef TFIPPSBGOHIT_H
#define TFIPPSBGOHIT_H

/** \addtogroup Detectors
 *  @{
 */

#include <cstdio>
#include <cmath>
#if !defined(__CINT__) && !defined(__CLING__)
#include <tuple>
#endif

#include "TMath.h"
#include "TVector3.h"
#include "TClonesArray.h"

#include "TFragment.h"
#include "TChannel.h"
#include "TPulseAnalyzer.h"

#include "TBgoHit.h"

class TFippsBgoHit : public TBgoHit {
public:
   TFippsBgoHit();
   TFippsBgoHit(const TFippsBgoHit& hit) : TBgoHit(static_cast<const TBgoHit&>(hit)) {}
   TFippsBgoHit(const TFragment& frag) : TBgoHit(frag) {}
   ~TFippsBgoHit() override;

   /////////////////////////		/////////////////////////////////////
   UShort_t GetArrayNumber() const override { return (20 * (GetDetector() - 1) + 5 * GetCrystal() + GetSegment()); } //!<!

   /// \cond CLASSIMP
   ClassDefOverride(TFippsBgoHit, 1)
   /// \endcond
};
/*! @} */
#endif
