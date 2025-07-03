#include "TFippsLaBrHit.h"

#include <iostream>
#include <algorithm>
#include <climits>

#include "Globals.h"
#include "TFippsLaBr.h"

TFippsLaBrHit::TFippsLaBrHit()
{
// Default Constructor
#if ROOT_VERSION_CODE < ROOT_VERSION(6, 0, 0)
   Class()->IgnoreTObjectStreamer(kTRUE);
#endif
   Clear();
}

TFippsLaBrHit::~TFippsLaBrHit() = default;

TFippsLaBrHit::TFippsLaBrHit(const TFippsLaBrHit& rhs) : TDetectorHit()
{
// Copy Constructor
#if ROOT_VERSION_CODE < ROOT_VERSION(6, 0, 0)
   Class()->IgnoreTObjectStreamer(kTRUE);
#endif
   Clear();
   rhs.Copy(*this);
}

void TFippsLaBrHit::Copy(TObject& rhs) const
{
   // Copies a TFippsLaBrHit
   TDetectorHit::Copy(rhs);
}

void TFippsLaBrHit::Copy(TObject& obj, bool) const
{
   Copy(obj);
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

void TFippsLaBrHit::Clear(Option_t*)
{
   // Clears the LaBrHit
   TDetectorHit::Clear();
}

void TFippsLaBrHit::Print(Option_t*) const
{
   /// Prints the LaBrHit. Returns:
   /// Detector
   /// Energy
   /// Time
   Print(std::cout);
}

void TFippsLaBrHit::Print(std::ostream& out) const
{
   std::ostringstream str;
   str << "TAC Detector:   " << GetDetector() << std::endl;
   str << "TAC hit energy: " << GetEnergy() << std::endl;
   str << "TAC hit time:   " << GetTime() << std::endl;
   out << str.str();
}
