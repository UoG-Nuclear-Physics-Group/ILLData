#ifndef TFIPPSLABRBGOHIT_H
#define TFIPPSLABRBGOHIT_H

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

class TFippsLaBrBgoHit : public TBgoHit {
public:
   TFippsLaBrBgoHit();
   TFippsLaBrBgoHit(const TFippsLaBrBgoHit& hit) : TBgoHit(static_cast<const TBgoHit&>(hit)) {}
   TFippsLaBrBgoHit(const TFragment& frag) : TBgoHit(frag) {}
   ~TFippsLaBrBgoHit() override;

   /////////////////////////		/////////////////////////////////////
   UShort_t GetArrayNumber() const override { return (3 * (GetDetector() - 1) + GetSegment()); } //!<! the BGO of each detector has three segments

   /// \cond CLASSIMP
   ClassDefOverride(TFippsLaBrBgoHit, 1)
   /// \endcond
};
/*! @} */
#endif
