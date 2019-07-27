#ifndef TFIPPSTAC_H
#define TFIPPSTAC_H

/** \addtogroup Detectors
 *  @{
 */

/////////////////////////////////////////////////////////////
///
/// \class TFippsTAC
///
/// The TFippsTAC class defines the observables and algorithms used
/// when analyzing TAC data. It includes detector positions,
/// etc.
///
/////////////////////////////////////////////////////////////

#include <vector>
#include <cstdio>

#include "TVector3.h"

#include "Globals.h"
#include "TDetector.h"
#include "TFippsTACHit.h"

class TFippsTAC : public TDetector {
public:
   TFippsTAC();
   ~TFippsTAC() override;
   TFippsTAC(const TFippsTAC& rhs);

public:
   TFippsTACHit* GetTACHit(const int& i) const { return static_cast<TFippsTACHit*>(GetHit(i)); }

#ifndef __CINT__
   void AddFragment(const std::shared_ptr<const TFragment>&, TChannel*) override; //!<!
#endif
	void BuildHits() override {} // no need to build any hits, everything already done in AddFragment

   TFippsTAC& operator=(const TFippsTAC&); //!<!

public:
   void Print(Option_t* opt = "") const override; //!<!

   /// \cond CLASSIMP
   ClassDefOverride(TFippsTAC, 1) // TAC Physics structure
   /// \endcond
};
/*! @} */
#endif
