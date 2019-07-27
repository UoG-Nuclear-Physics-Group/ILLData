#include <iostream>
#include "TFippsLaBr.h"
#include <TRandom.h>
#include <TMath.h>

#include "TGRSIOptions.h"

/// \cond CLASSIMP
ClassImp(TFippsLaBr)
/// \endcond

bool DefaultLaBrSuppression(const TDetectorHit* hit, const TDetectorHit* bgoHit)
{
	return ((hit->GetDetector() == bgoHit->GetDetector()) &&
	(std::fabs(hit->GetTime() - bgoHit->GetTime()) < TGRSIOptions::AnalysisOptions()->SuppressionWindow()) &&
	(bgoHit->GetEnergy() > TGRSIOptions::AnalysisOptions()->SuppressionEnergy()));
}

std::function<bool(const TDetectorHit*, const TDetectorHit*)> TFippsLaBr::fSuppressionCriterion = DefaultLaBrSuppression;

TVector3 TFippsLaBr::gPosition[9] = {
	// These positions should be updated (they are currently SCEPTAR-ish)
	TVector3(0, 0, 1),
	TVector3(14.3025, 4.6472, 22.8096),
	TVector3(0, 15.0386, 22.8096),
	TVector3(-14.3025, 4.6472, 22.8096),
	TVector3(-8.8395, -12.1665, 22.8096),
	TVector3(8.8395, -12.1665, 22.8096),
	TVector3(19.7051, 6.4026, 6.2123),
	TVector3(0, 20.7192, 6.2123),
	TVector3(-19.7051, 6.4026, 6.2123),
};

TFippsLaBr::TFippsLaBr()
{
	// Default Constructor
#if MAJOR_ROOT_VERSION < 6
	Class()->IgnoreTObjectStreamer(kTRUE);
#endif
	Clear();
}

TFippsLaBr::~TFippsLaBr()
{
	// Default Destructor
}

TFippsLaBr::TFippsLaBr(const TFippsLaBr& rhs) : TSuppressed()
{
	// Copy Contructor
#if MAJOR_ROOT_VERSION < 6
	Class()->IgnoreTObjectStreamer(kTRUE);
#endif
	rhs.Copy(*this);
}

void TFippsLaBr::Clear(Option_t* opt)
{
	// Clears all of the hits
	TSuppressed::Clear(opt);
	fSuppressedHits.clear();
	fLaBrBits = 0;
}

void TFippsLaBr::Copy(TObject& rhs) const
{
	// Copies a TFippsLaBr
	TSuppressed::Copy(rhs);

	static_cast<TFippsLaBr&>(rhs).fSuppressedHits = fSuppressedHits;
   static_cast<TFippsLaBr&>(rhs).fLaBrBits       = 0;
}

TFippsLaBr& TFippsLaBr::operator=(const TFippsLaBr& rhs)
{
	rhs.Copy(*this);
	return *this;
}

void TFippsLaBr::Print(Option_t*) const
{
	// Prints out TFippsLaBr Multiplicity, currently does little.
	printf("%lu fHits\n", fHits.size());
}

bool TFippsLaBr::IsSuppressed() const
{
	return TestBitNumber(ELaBrBits::kIsSuppressed);
}

void TFippsLaBr::SetSuppressed(const bool flag)
{
	return SetBitNumber(ELaBrBits::kIsSuppressed, flag);
}

void TFippsLaBr::ResetSuppressed()
{
   SetSuppressed(false);
   fSuppressedHits.clear();
}

Short_t TFippsLaBr::GetSuppressedMultiplicity(const TBgo* bgo)
{
	/// Automatically builds the suppressed hits using the fSuppressionCriterion and returns the number of suppressed hits
	if(fHits.empty()) {
		return 0;
	}
   // if the suppressed has been reset, clear the suppressed hits
   if(!IsSuppressed()) {
      fSuppressedHits.clear();
   }
   if(fSuppressedHits.empty()) {
		CreateSuppressed(bgo, fHits, fSuppressedHits);
      SetSuppressed(true);
   }

   return fSuppressedHits.size();
}

TFippsLaBrHit* TFippsLaBr::GetSuppressedHit(const int& i)
{
	try {
		return static_cast<TFippsLaBrHit*>(fSuppressedHits.at(i));
	} catch(const std::out_of_range& oor) {
		std::cerr<<ClassName()<<" is out of range: "<<oor.what()<<std::endl;
      throw grsi::exit_exception(1);
   }
	return nullptr;
}

void TFippsLaBr::AddFragment(const std::shared_ptr<const TFragment>& frag, TChannel*)
{
	TFippsLaBrHit* laHit = new TFippsLaBrHit(*frag);                 // Building is controlled in the constructor of the hit
	fHits.push_back(std::move(laHit)); // use std::move for efficienciy since laHit loses scope here anyway
}
