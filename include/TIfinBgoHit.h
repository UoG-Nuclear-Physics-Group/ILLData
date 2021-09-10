#ifndef TIFINBGOHIT_H
#define TIFINBGOHIT_H

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

class TIfinBgoHit : public TBgoHit {
public:
   TIfinBgoHit();
   TIfinBgoHit(const TIfinBgoHit& hit) : TBgoHit(static_cast<const TBgoHit&>(hit)) {}
   TIfinBgoHit(const TFragment& frag) : TBgoHit(frag) {}
   ~TIfinBgoHit() override;

   /////////////////////////		/////////////////////////////////////
   UShort_t GetArrayNumber() const override { return (20 * (GetDetector() - 1) + 5 * GetCrystal() + GetSegment()); } //!<!

   /// \cond CLASSIMP
   ClassDefOverride(TIfinBgoHit, 1)
   /// \endcond
};
/*! @} */
#endif
