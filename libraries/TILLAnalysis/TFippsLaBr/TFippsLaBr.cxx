#include <iostream>
#include "TFippsLaBr.h"
#include <TRandom.h>
#include <TMath.h>

#include "TGRSIOptions.h"

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
#if ROOT_VERSION_CODE < ROOT_VERSION(6, 0, 0)
   Class()->IgnoreTObjectStreamer(kTRUE);
#endif
   Clear();
}

TFippsLaBr::~TFippsLaBr()
{
   // Default Destructor
   for(auto hit : fSuppressedHits) delete hit;
}

TFippsLaBr::TFippsLaBr(const TFippsLaBr& rhs) : TSuppressed()
{
   // Copy Contructor
#if ROOT_VERSION_CODE < ROOT_VERSION(6, 0, 0)
   Class()->IgnoreTObjectStreamer(kTRUE);
#endif
   rhs.Copy(*this);
}

void TFippsLaBr::Clear(Option_t* opt)
{
   // Clears all of the hits
   TSuppressed::Clear(opt);
   for(auto hit : fSuppressedHits) delete hit;
   fSuppressedHits.clear();
   fLaBrBits = 0;
}

void TFippsLaBr::Copy(TObject& rhs) const
{
   // Copies a TFippsLaBr
   TSuppressed::Copy(rhs);

   static_cast<TFippsLaBr&>(rhs).fSuppressedHits.resize(fSuppressedHits.size());
   for(size_t i = 0; i < fSuppressedHits.size(); ++i) {
      static_cast<TFippsLaBr&>(rhs).fSuppressedHits[i] = new TFippsLaBrHit(*static_cast<TFippsLaBrHit*>(fSuppressedHits[i]));
   }
   static_cast<TFippsLaBr&>(rhs).fLaBrBits = 0;
}

TFippsLaBr& TFippsLaBr::operator=(const TFippsLaBr& rhs)
{
   rhs.Copy(*this);
   return *this;
}

void TFippsLaBr::Print(Option_t*) const
{
   // Prints out TFippsLaBr Multiplicity, currently does little.
   Print(std::cout);
}

void TFippsLaBr::Print(std::ostream& out) const
{
   std::ostringstream str;
   str << Hits().size() << " Hits" << std::endl;
   out << str.str();
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
   for(auto hit : fSuppressedHits) delete hit;
   fSuppressedHits.clear();
}

Short_t TFippsLaBr::GetSuppressedMultiplicity(const TBgo* bgo)
{
   /// Automatically builds the suppressed hits using the fSuppressionCriterion and returns the number of suppressed hits
   if(Hits().empty()) {
      return 0;
   }
   // if the suppressed has been reset, clear the suppressed hits
   if(!IsSuppressed()) {
      fSuppressedHits.clear();
   }
   if(fSuppressedHits.empty()) {
      if(!IsSuppressed()) {
         for(auto hit : fSuppressedHits) delete hit;
         fSuppressedHits.clear();
      }

      CreateSuppressed(bgo, Hits(), fSuppressedHits);
      SetSuppressed(true);
   }

   return fSuppressedHits.size();
}

TFippsLaBrHit* TFippsLaBr::GetSuppressedHit(const int& i)
{
   try {
      return static_cast<TFippsLaBrHit*>(fSuppressedHits.at(i));
   } catch(const std::out_of_range& oor) {
      std::cerr << ClassName() << " is out of range: " << oor.what() << std::endl;
      throw grsi::exit_exception(1);
   }
   return nullptr;
}

void TFippsLaBr::AddFragment(const std::shared_ptr<const TFragment>& frag, TChannel*)
{
   TFippsLaBrHit* hit = new TFippsLaBrHit(*frag);   // Building is controlled in the constructor of the hit
   Hits().push_back(hit);
}
