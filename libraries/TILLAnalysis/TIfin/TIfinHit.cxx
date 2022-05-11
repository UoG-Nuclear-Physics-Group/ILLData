#include "TIfin.h"
#include "TIfinHit.h"
#include "Globals.h"
#include <cmath>
#include <iostream>

/// \cond CLASSIMP
ClassImp(TIfinHit)
/// \endcond

TIfinHit::TIfinHit()
	: TDetectorHit()
{
	// Default Ctor. Ignores TObject Streamer in ROOT < 6.
#if MAJOR_ROOT_VERSION < 6
	Class()->IgnoreTObjectStreamer(kTRUE);
#endif
	Clear();
}

TIfinHit::TIfinHit(const TIfinHit& rhs) : TDetectorHit()
{
	// Copy Ctor. Ignores TObject Streamer in ROOT < 6.
	Clear();
	rhs.Copy(*this);
}

TIfinHit::TIfinHit(const TFragment& frag) : TDetectorHit(frag)
{
}

TIfinHit::~TIfinHit() = default;

void TIfinHit::Copy(TObject& rhs) const
{
	TDetectorHit::Copy(rhs);
}

void TIfinHit::Copy(TObject& obj, bool) const
{
	Copy(obj);
}

void TIfinHit::Clear(Option_t* opt)
{
	// Clears the information stored in the TIfinHit.
	TDetectorHit::Clear(opt); // clears the base (address, position and waveform)
}

void TIfinHit::Print(Option_t*) const
{
	// Prints the Detector Number, Crystal Number, Energy, Time and Angle.
	printf("Ifin Detector: %i\n", GetDetector());
	printf("Ifin Crystal:  %i\n", GetCrystal());
	printf("Ifin Energy:   %lf\n", GetEnergy());
	printf("Ifin hit time:   %lf\n", GetTime());
	printf("Ifin hit TV3 theta: %.2f\tphi%.2f\n", GetPosition().Theta() * 180 / (3.141597),
			GetPosition().Phi() * 180 / (3.141597));
}

TVector3 TIfinHit::GetPosition(double dist) const
{
	return TIfin::GetPosition(GetDetector(), GetCrystal(), dist);
}

TVector3 TIfinHit::GetPosition() const
{
	return GetPosition(GetDefaultDistance());
}

bool TIfinHit::CompareEnergy(const TIfinHit* lhs, const TIfinHit* rhs)
{
	return (lhs->GetEnergy() > rhs->GetEnergy());
}

void TIfinHit::Add(const TDetectorHit* hit)
{
	const TIfinHit* fippsHit = dynamic_cast<const TIfinHit*>(hit);
	if(fippsHit == nullptr) {
		throw std::runtime_error("trying to add non-fipps hit to fipps hit!");
	}
	// add another griffin hit to this one (for addback),
	// using the time and position information of the one with the higher energy
	if(!CompareEnergy(this, fippsHit)) {
		SetCfd(fippsHit->GetCfd());
		SetTime(fippsHit->GetTime());
		// SetPosition(fippsHit->GetPosition());
		SetAddress(fippsHit->GetAddress());
	}
	SetEnergy(GetEnergy() + fippsHit->GetEnergy());
	// this has to be done at the very end, otherwise GetEnergy() might not work
	SetCharge(0);
	// KValue is somewhate meaningless in addback, so I am using it as an indicator that a piledup hit was added-back RD
	if(GetKValue() > fippsHit->GetKValue()) {
		SetKValue(fippsHit->GetKValue());
	}
}

Double_t TIfinHit::GetNoCTEnergy(Option_t*) const
{
	TChannel* chan = GetChannel();
	if(chan == nullptr) {
		Error("GetEnergy", "No TChannel exists for address 0x%08x", GetAddress());
		return 0.;
	}
	return chan->CalibrateENG(Charge(), GetKValue());
}
