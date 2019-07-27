#include "TFippsLaBrHit.h"

#include <iostream>
#include <algorithm>
#include <climits>

#include "Globals.h"
#include "TFippsLaBr.h"

/// \cond CLASSIMP
ClassImp(TFippsLaBrHit)
/// \endcond

TFippsLaBrHit::TFippsLaBrHit()
{
// Default Constructor
#if MAJOR_ROOT_VERSION < 6
   Class()->IgnoreTObjectStreamer(kTRUE);
#endif
   Clear();
}

TFippsLaBrHit::~TFippsLaBrHit() = default;

TFippsLaBrHit::TFippsLaBrHit(const TFippsLaBrHit& rhs) : TFippsHit()
{
// Copy Constructor
#if MAJOR_ROOT_VERSION < 6
   Class()->IgnoreTObjectStreamer(kTRUE);
#endif
   Clear();
   rhs.Copy(*this);
}

void TFippsLaBrHit::Copy(TObject& rhs) const
{
   // Copies a TFippsLaBrHit
   TFippsHit::Copy(rhs);
   static_cast<TFippsLaBrHit&>(rhs).fFilter = fFilter;
}

TVector3 TFippsLaBrHit::GetPosition(Double_t) const
{
   // Gets the position of the current TFippsLaBrHit
   return TFippsLaBr::GetPosition(GetDetector());
}

TVector3 TFippsLaBrHit::GetPosition() const
{
   // Gets the position of the current TFippsLaBrHit
   return GetPosition(GetDefaultDistance());
}

bool TFippsLaBrHit::InFilter(Int_t)
{
   // check if the desired filter is in wanted filter;
   // return the answer;
   // currently does nothing
   return true;
}

void TFippsLaBrHit::Clear(Option_t*)
{
   // Clears the LaBrHit
   fFilter = 0;
   TFippsHit::Clear();
}

void TFippsLaBrHit::Print(Option_t*) const
{
   // Prints the LaBrHit. Returns:
   // Detector
   // Energy
   // Time
   printf("LaBr Detector: %i\n", GetDetector());
   printf("LaBr hit energy: %.2f\n", GetEnergy());
   printf("LaBr hit time:   %.lf\n", GetTime());
}
