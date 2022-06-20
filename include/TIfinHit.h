#ifndef IFINHIT_H
#define IFINHIT_H

/** \addtogroup Detectors
 *  @{
 */

#include <cstdio>
#include <cmath>

#include "TVector3.h"

#include "TFragment.h"
#include "TChannel.h"
#include "TDetectorHit.h"

class TIfinHit : public TDetectorHit {
public:
   TIfinHit();
   TIfinHit(const TIfinHit&);
   TIfinHit(const TFragment&);
   ~TIfinHit() override;

public:
   /////////////////////////  Getters	/////////////////////////////////////
   Double_t GetNoCTEnergy(Option_t* opt = "") const;

   /////////////////////////  Recommended Functions/////////////////////////

   /////////////////////////		/////////////////////////////////////

   inline UShort_t GetArrayNumber() const override { return (4 * (GetDetector() - 1) + (GetCrystal() + 1)); } //!<!
   static bool CompareEnergy(const TIfinHit*, const TIfinHit*); //!<!
   void        Add(const TDetectorHit*) override;             //!<!

public:
   void Clear(Option_t* opt = "") override;       //!<!
   void Print(Option_t* opt = "") const override; //!<!
	void Print(std::ostream& out) const override;
   void Copy(TObject&) const override;            //!<!
   void Copy(TObject&, bool) const override;      //!<!

   TVector3 GetPosition(double dist) const override; //!<!
   TVector3 GetPosition() const override;

private:
   Double_t GetDefaultDistance() const { return 90.; } // Target to detector distance is 90mm (Unless suppressed)

   /// \cond CLASSIMP
   ClassDefOverride(TIfinHit, 1); // Information about a IFIN Hit
   /// \endcond
};
/*! @} */
#endif
