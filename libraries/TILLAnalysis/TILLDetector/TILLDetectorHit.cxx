#include "TILLDetectorHit.h"

#include "TChannel.h"
#include "TILLMnemonic.h"

/// \cond CLASSIMP
ClassImp(TILLDetectorHit)
/// \endcond

TILLDetectorHit::TILLDetectorHit(const int& Address) : TDetectorHit()
{
   /// Default constructor
   Clear();
   fAddress = Address;

#if MAJOR_ROOT_VERSION < 6
   Class()->IgnoreTObjectStreamer(kTRUE);
#endif
}

TILLDetectorHit::TILLDetectorHit(const TILLDetectorHit& rhs, bool copywave) : TDetectorHit(rhs)
{
   /// Default Copy constructor
   rhs.Copy(*this);
   if(copywave) {
      rhs.CopyWave(*this);
   }
   ClearTransients();

#if MAJOR_ROOT_VERSION < 6
   Class()->IgnoreTObjectStreamer(kTRUE);
#endif
}

TILLDetectorHit::~TILLDetectorHit()
{
   // Default destructor
}

Double_t TILLDetectorHit::GetTime(const ETimeFlag&, Option_t*) const
{
   if(IsTimeSet()) {
      return fTime;
   }

   TChannel* channel = GetChannel();
   if(channel == nullptr) {
      Error("GetTime", "No TChannel exists for address 0x%08x", GetAddress());
      return SetTime(10. * (static_cast<Double_t>((GetTimeStamp()) + gRandom->Uniform())));
   }
	Double_t dTime = static_cast<Double_t>((GetTimeStamp()) + gRandom->Uniform());
	return SetTime(10. * (dTime - channel->GetTZero(GetEnergy())));
}
