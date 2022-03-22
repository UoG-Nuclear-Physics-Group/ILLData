#include "TFipps.h"

#include <sstream>
#include <iostream>
#include <iomanip>

#include "TRandom.h"
#include "TMath.h"
#include "TInterpreter.h"
#include "TMnemonic.h"

#include "TGRSIOptions.h"

/// \cond CLASSIMP
ClassImp(TFipps)
/// \endcond


bool DefaultFippsAddback(const TDetectorHit* one, const TDetectorHit* two)
{
   return ((one->GetDetector() == two->GetDetector()) &&
           (std::fabs(one->GetTime() - two->GetTime()) < TGRSIOptions::AnalysisOptions()->AddbackWindow()));
}

std::function<bool(const TDetectorHit*, const TDetectorHit*)> TFipps::fAddbackCriterion = DefaultFippsAddback;

bool DefaultFippsSuppression(const TDetectorHit* hit, const TDetectorHit* bgoHit)
{
	return ((hit->GetDetector() == bgoHit->GetDetector()) &&
	(std::fabs(hit->GetTime() - bgoHit->GetTime()) < TGRSIOptions::AnalysisOptions()->SuppressionWindow()) &&
	(bgoHit->GetEnergy() > TGRSIOptions::AnalysisOptions()->SuppressionEnergy()));
}

std::function<bool(const TDetectorHit*, const TDetectorHit*)> TFipps::fSuppressionCriterion = DefaultFippsSuppression;

// Fipps detector locations.
// Angles are in ISO standard
// x = cos(theta)*sin(phi)      // Points port
// y = sin(theta)*sin(phi)      // Points upwards
// z = sin(theta)               // Points in the direction of the neutrons
// TVector(x,y,z)
// TODO: Add link to picture showing detector positions when uploaded
TVector3 TFipps::gCloverPosition[17] = {
   TVector3(TMath::Sin(TMath::DegToRad() * (0.0)) * TMath::Cos(TMath::DegToRad() * (0.0)),
            TMath::Sin(TMath::DegToRad() * (0.0)) * TMath::Sin(TMath::DegToRad() * (0.0)),
            TMath::Cos(TMath::DegToRad() * (0.0))), // Zeroth Position
   // Corona
   TVector3(TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Cos(TMath::DegToRad() * (90.0)),
            TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Sin(TMath::DegToRad() * (90.0)),
            TMath::Cos(TMath::DegToRad() * (90.0))), // Fipps Pos 0
   TVector3(TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Cos(TMath::DegToRad() * (45.0)),
            TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Sin(TMath::DegToRad() * (45.0)),
            TMath::Cos(TMath::DegToRad() * (90.0))), // Fipps Pos 1
   TVector3(TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Cos(TMath::DegToRad() * (0.0)),
            TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Sin(TMath::DegToRad() * (0.0)),
            TMath::Cos(TMath::DegToRad() * (90.0))), // Fipps Pos 2
   TVector3(TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Cos(TMath::DegToRad() * (315.0)),
            TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Sin(TMath::DegToRad() * (315.0)),
            TMath::Cos(TMath::DegToRad() * (90.0))), // Fipps Pos 3
   TVector3(TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Cos(TMath::DegToRad() * (270.0)),
            TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Sin(TMath::DegToRad() * (270.0)),
            TMath::Cos(TMath::DegToRad() * (90.0))), // Fipps Pos 4
   TVector3(TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Cos(TMath::DegToRad() * (225.0)),
            TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Sin(TMath::DegToRad() * (225.0)),
            TMath::Cos(TMath::DegToRad() * (90.0))), // Fipps Pos 5
   TVector3(TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Cos(TMath::DegToRad() * (180.0)),
            TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Sin(TMath::DegToRad() * (180.0)),
            TMath::Cos(TMath::DegToRad() * (90.0))), // Fipps Pos 6
   TVector3(TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Cos(TMath::DegToRad() * (135.0)),
            TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Sin(TMath::DegToRad() * (135.0)),
            TMath::Cos(TMath::DegToRad() * (90.0))), // Fipps Pos 7
   // Downstream lampshade
   TVector3(TMath::Sin(TMath::DegToRad() * (45.0)) * TMath::Cos(TMath::DegToRad() * (90.0)),
            TMath::Sin(TMath::DegToRad() * (45.0)) * TMath::Sin(TMath::DegToRad() * (90.0)),
            TMath::Cos(TMath::DegToRad() * (45.0))), // Fipps Pos 8
   TVector3(TMath::Sin(TMath::DegToRad() * (45.0)) * TMath::Cos(TMath::DegToRad() * (0.0)),
            TMath::Sin(TMath::DegToRad() * (45.0)) * TMath::Sin(TMath::DegToRad() * (0.0)),
            TMath::Cos(TMath::DegToRad() * (45.0))), // Fipps Pos 9
   TVector3(TMath::Sin(TMath::DegToRad() * (45.0)) * TMath::Cos(TMath::DegToRad() * (270.0)),
            TMath::Sin(TMath::DegToRad() * (45.0)) * TMath::Sin(TMath::DegToRad() * (270.0)),
            TMath::Cos(TMath::DegToRad() * (45.0))), // Fipps Pos 10
   TVector3(TMath::Sin(TMath::DegToRad() * (45.0)) * TMath::Cos(TMath::DegToRad() * (180.0)),
            TMath::Sin(TMath::DegToRad() * (45.0)) * TMath::Sin(TMath::DegToRad() * (180.0)),
            TMath::Cos(TMath::DegToRad() * (45.0))), // Fipps Pos 11
   // Upstream lampshade
   TVector3(TMath::Sin(TMath::DegToRad() * (135.0)) * TMath::Cos(TMath::DegToRad() * (90.0)),
            TMath::Sin(TMath::DegToRad() * (135.0)) * TMath::Sin(TMath::DegToRad() * (90.0)),
            TMath::Cos(TMath::DegToRad() * (135.0))), // G: 13 F: 12
   TVector3(TMath::Sin(TMath::DegToRad() * (135.0)) * TMath::Cos(TMath::DegToRad() * (0.0)),
            TMath::Sin(TMath::DegToRad() * (135.0)) * TMath::Sin(TMath::DegToRad() * (0.0)),
            TMath::Cos(TMath::DegToRad() * (135.0))), // G: 16 F: 13
   TVector3(TMath::Sin(TMath::DegToRad() * (135.0)) * TMath::Cos(TMath::DegToRad() * (270.0)),
            TMath::Sin(TMath::DegToRad() * (135.0)) * TMath::Sin(TMath::DegToRad() * (270.0)),
            TMath::Cos(TMath::DegToRad() * (135.0))), // G: 15 F: 14
   TVector3(TMath::Sin(TMath::DegToRad() * (135.0)) * TMath::Cos(TMath::DegToRad() * (180.0)),
            TMath::Sin(TMath::DegToRad() * (135.0)) * TMath::Sin(TMath::DegToRad() * (180.0)),
            TMath::Cos(TMath::DegToRad() * (135.0))) // G: 14 F: 15
            };

TFipps::TFipps() : TSuppressed()
{
// Default ctor. Ignores TObjectStreamer in ROOT < 6
#if MAJOR_ROOT_VERSION < 6
   Class()->IgnoreTObjectStreamer(kTRUE);
#endif
   Clear();
}

TFipps::TFipps(const TFipps& rhs) : TSuppressed()
{
// Copy ctor. Ignores TObjectStreamer in ROOT < 6
#if MAJOR_ROOT_VERSION < 6
   Class()->IgnoreTObjectStreamer(kTRUE);
#endif
   rhs.Copy(*this);
}

void TFipps::Copy(TObject& rhs) const
{
   // Copy function.
   TSuppressed::Copy(rhs);

   static_cast<TFipps&>(rhs).fAddbackHits.clear();
   static_cast<TFipps&>(rhs).fAddbackFrags.clear();
   static_cast<TFipps&>(rhs).fSuppressedHits.clear();
   static_cast<TFipps&>(rhs).fSuppressedAddbackHits.clear();
   static_cast<TFipps&>(rhs).fSuppressedAddbackFrags.clear();
   static_cast<TFipps&>(rhs).fFippsBits    = 0;
}

TFipps::~TFipps()
{
   // Default Destructor

   // fHits automatically deleted in TDetector
   for( auto hit : fAddbackHits ) delete hit;
   for( auto hit : fSuppressedHits ) delete hit;
   for( auto hit : fSuppressedAddbackHits ) delete hit;

}

void TFipps::Clear(Option_t* opt)
{
   // Clears the mother, and all of the hits
   ClearStatus();
   TSuppressed::Clear(opt);


   for( auto hit : fAddbackHits ) delete hit;
   for( auto hit : fSuppressedHits ) delete hit;
   for( auto hit : fSuppressedAddbackHits ) delete hit;

   fAddbackHits.clear();
   fSuppressedHits.clear();
   fSuppressedAddbackHits.clear();
   fAddbackFrags.clear();
   fSuppressedAddbackFrags.clear();
}

void TFipps::Print(Option_t*) const
{
   std::cout<<"Fipps Contains: "<<std::endl;
   std::cout<<std::setw(6)<<GetMultiplicity()<<" hits"<<std::endl;

   if(IsAddbackSet()) {
      std::cout<<std::setw(6)<<fAddbackHits.size()<<" addback hits"<<std::endl;
   } else {
      std::cout<<std::setw(6)<<" "
               <<" Addback not set"<<std::endl;
   }

   std::cout<<std::setw(6)<<" "
            <<" Cross-talk Set?  "<<IsCrossTalkSet()<<std::endl;
}

TFipps& TFipps::operator=(const TFipps& rhs)
{
   rhs.Copy(*this);
   return *this;
}


std::vector<TDetectorHit*>& TFipps::GetAddbackVector()
{
   return fAddbackHits;
}

std::vector<UShort_t>& TFipps::GetAddbackFragVector()
{
   return fAddbackFrags;
}

std::vector<TDetectorHit*>& TFipps::GetSuppressedVector()
{
    return fSuppressedHits;
}

std::vector<TDetectorHit*>& TFipps::GetSuppressedAddbackVector()
{
    return fSuppressedAddbackHits;
}

std::vector<UShort_t>& TFipps::GetSuppressedAddbackFragVector()
{
    return fSuppressedAddbackFrags;
}

bool TFipps::IsAddbackSet() const
{
   return TestBitNumber(EFippsBits::kIsAddbackSet);
}

bool TFipps::IsCrossTalkSet() const
{
   return TestBitNumber(EFippsBits::kIsCrossTalkSet);
}

bool TFipps::IsSuppressed() const
{
    return TestBitNumber(EFippsBits::kIsSuppressedSet);
}

bool TFipps::IsSuppressedAddbackSet() const
{
    return TestBitNumber(EFippsBits::kIsSuppressedAddbackSet);
}

void TFipps::SetAddback(const Bool_t flag) const
{
   return SetBitNumber(EFippsBits::kIsAddbackSet, flag);
}

void TFipps::SetCrossTalk(const Bool_t flag) const
{
   return SetBitNumber(EFippsBits::kIsCrossTalkSet, flag);
}

void TFipps::SetSuppressed(const Bool_t flag) const
{
    return SetBitNumber(EFippsBits::kIsSuppressedSet, flag);
}

void TFipps::SetSuppressedAddback(const Bool_t flag) const
{
    return SetBitNumber(EFippsBits::kIsSuppressedAddbackSet, flag);
}

TDetectorHit* TFipps::GetFippsHit(const int& i)
{
   try {
      if(!IsCrossTalkSet()) {
         FixCrossTalk();
      }
      return fHits.at(i);
   } catch(const std::out_of_range& oor) {
      std::cerr<<ClassName()<<" Hits are out of range: "<<oor.what()<<std::endl;
      if(!gInterpreter) {
         throw grsi::exit_exception(1);
      }
   }
   return nullptr;
}

TDetectorHit* TFipps::GetSuppressedHit(const int& i)
{
    try {
        if(!IsCrossTalkSet()) {
            FixCrossTalk();
        }
        return fSuppressedHits.at(i);
   } catch(const std::out_of_range& oor) {
      std::cerr<<ClassName()<<"Suppressed hits are out of range: "<<oor.what()<<std::endl;
      if(!gInterpreter) {
         throw grsi::exit_exception(1);
      }
   }
   return nullptr;
}

Int_t TFipps::GetSuppressedMultiplicity(const TBgo* bgo)
{
	/// Automatically builds the suppressed hits using the fSuppressionCriterion and returns the number of suppressed hits
   if(!IsCrossTalkSet()) {
      // Calculate Cross Talk on each hit
      FixCrossTalk();
   }
   auto& hit_vec  = GetHitVector();
   auto& sup_vec  = GetSuppressedVector();
	if(hit_vec.empty()) {
		return 0;
	}
   // if the suppressed has been reset, clear the suppressed hits
   if(!IsSuppressed()) {
      sup_vec.clear();
   }
   if(sup_vec.empty()) {
		CreateSuppressed(bgo, hit_vec, sup_vec);
      SetSuppressed(true);
   }

   return sup_vec.size();
}
    

Int_t TFipps::GetAddbackMultiplicity()
{
   // Automatically builds the addback hits using the fAddbackCriterion (if
   // the size of the fAddbackHits vector is zero) and return the number of
   // addback hits.
   if(!IsCrossTalkSet()) {
      FixCrossTalk();
   }
   auto& hit_vec  = GetHitVector();
   auto& ab_vec   = GetAddbackVector();
   auto& frag_vec = GetAddbackFragVector();
   if(hit_vec.empty()) {
      return 0;
   }

   // if the addback has been reset, clear the addback hits
   if(!IsAddbackSet()) {
      ab_vec.clear();
      frag_vec.clear();
   }
   if(ab_vec.empty()) {
		CreateAddback(hit_vec, ab_vec, frag_vec);
      SetAddback(true);
   }

   return ab_vec.size();
}

Int_t TFipps::GetSuppressedAddbackMultiplicity(const TBgo* bgo)
{
   // Automatically builds the addback hits using the fAddbackCriterion (if
   // the size of the fAddbackHits vector is zero) and return the number of
   // addback hits.
   if(!IsCrossTalkSet()) {
      FixCrossTalk();
   }
   auto& hit_vec  = GetSuppressedVector();
   auto& ab_vec   = GetSuppressedAddbackVector();
   auto& frag_vec = GetSuppressedAddbackFragVector();
   if(hit_vec.empty()) {
      return 0;
   }

   // if the addback has been reset, clear the addback hits
   if(!IsAddbackSet()) {
      ab_vec.clear();
      frag_vec.clear();
   }
   if(ab_vec.empty()) {
		CreateSuppressedAddback(bgo, hit_vec, ab_vec, frag_vec);
        SetSuppressedAddback(true);
   }

   return ab_vec.size();
}


TDetectorHit* TFipps::GetAddbackHit(const int& i)
{
   try{
       if(!IsCrossTalkSet()) {
           FixCrossTalk();
       }
       return static_cast<TDetectorHit*>(GetAddbackVector().at(i));
   } catch(const std::out_of_range& oor) {
       std::cerr<<ClassName()<<" Addback hits are out of range: "<<oor.what()<<std::endl;
       if(!gInterpreter) {
           throw grsi::exit_exception(1);
       }
   }
   return nullptr;
}

TDetectorHit* TFipps::GetSuppressedAddbackHit(const int& i)
{
   try{
       if(!IsCrossTalkSet()) {
           FixCrossTalk();
       }
       return static_cast<TDetectorHit*>(GetSuppressedAddbackVector().at(i));
   } catch(const std::out_of_range& oor) {
       std::cerr<<ClassName()<<" Suppressed addback hits are out of range: "<<oor.what()<<std::endl;
       if(!gInterpreter) {
           throw grsi::exit_exception(1);
       }
   }
   return nullptr;
}

void TFipps::AddFragment(const std::shared_ptr<const TFragment>& frag, TChannel* chan)
{
   // Builds the FIPPS Hits directly from the TFragment. Basically, loops through the hits for an event and sets
   // observables.
   // This is done for both FIPPS and it's suppressors.
   if(frag == nullptr || chan == nullptr) {
      return;
   }

   switch(chan->GetMnemonic()->SubSystem()) {
		case TMnemonic::EMnemonic::kG: 
			{
				auto hit = new TFippsHit(*frag);
				fHits.push_back(hit);
			}
			break;
		default:
			break;
   };
}

TVector3 TFipps::GetPosition(int DetNbr, int CryNbr, double dist)
{
   // Gets the position vector for a crystal specified by CryNbr within Clover DetNbr at a distance of dist mm away.
   // This is calculated to the most likely interaction point within the crystal.
   if(DetNbr > 16) {
      return TVector3(0, 0, 1);
   }

   TVector3 CloverPosition(gCloverPosition[DetNbr]);

   // XAxis: Array's Port
   // YAxis: Upwards
   // ZAxis: Points in the neutron beam direction

   // Interaction points may eventually be set externally. May make these members of each crystal, or pass from
   // waveforms.
   Double_t cp = 17.678;//25.0; // Crystal Center Point  mm. (diameter 50mm)
   Double_t id = 40.0; // Crystal interaction depth mm. (length 80mm)
   // Set Theta's of the center of each DETECTOR face
   ////Define one Detector position
   TVector3 CrystalPosition;
   switch(CryNbr) {
   case 0: CrystalPosition.SetXYZ(-cp, cp, id); break;  // BLUE
   case 1: CrystalPosition.SetXYZ(cp, cp, id); break;   // GREEN
   case 2: CrystalPosition.SetXYZ(cp, -cp, id); break;  // RED
   case 3: CrystalPosition.SetXYZ(-cp, -cp, id); break; // WHITE
   default: CrystalPosition.SetXYZ(0, 0, 1); break;
   };
   // Rotate counterclockwise from the downstream position
   CrystalPosition.RotateY(CloverPosition.Theta());
   // Rotate around the neutron beam
   CrystalPosition.RotateZ(CloverPosition.Phi());
   // Set distance of detector from target
   CloverPosition.SetMag(dist);

   return (CloverPosition + CrystalPosition);
}

void TFipps::ResetFlags() const
{
   fFippsBits = 0;
}

void TFipps::ResetAddback()
{
   SetAddback(false);
   SetCrossTalk(false);
   GetAddbackVector().clear();
   GetAddbackFragVector().clear();
}

void TFipps::ResetSuppressed()
{
    SetSuppressed(false);
    GetSuppressedVector().clear();
}

void TFipps::ResetSuppressedAddback()
{
   SetAddback(false);
   SetCrossTalk(false);
   SetSuppressed(false);
   GetSuppressedAddbackVector().clear();
   GetSuppressedAddbackFragVector().clear();
}

UShort_t TFipps::GetNAddbackFrags(const size_t& idx)
{
   // Get the number of addback "fragments" contributing to the total addback hit
   // with index idx.
   if(idx < GetAddbackFragVector().size()) {
      return GetAddbackFragVector().at(idx);
   }
   return 0;
}

void TFipps::SetBitNumber(enum EFippsBits bit, Bool_t set) const
{
   // Used to set the flags that are stored in TFipps.
   fFippsBits.SetBit(bit, set);
}


Double_t TFipps::CTCorrectedEnergy(const TFippsHit* const hit_to_correct, const TFippsHit* const other_hit,
                                   Bool_t time_constraint)
{
   if((hit_to_correct == nullptr) || (other_hit == nullptr)) {
		std::cout<<"One of the hits is invalid in TFipps::CTCorrectedEnergy"<<std::endl;
      return 0;
   }

   if(time_constraint) {
      // Figure out if this passes the selected window
      if(TMath::Abs(other_hit->GetTime() - hit_to_correct->GetTime()) >
         TGRSIOptions::AnalysisOptions()->AddbackWindow()) { // placeholder
         return hit_to_correct->GetEnergy();
      }
   }

   if(hit_to_correct->GetDetector() != other_hit->GetDetector()) {
      return hit_to_correct->GetEnergy();
   }
	static bool been_warned[256] = {false};
	double      fixed_energy     = hit_to_correct->GetEnergy();
	try {
		if(hit_to_correct->GetChannel() != nullptr) {
			fixed_energy -= hit_to_correct->GetChannel()->GetCTCoeff().at(other_hit->GetCrystal()) * other_hit->GetNoCTEnergy();
		}
	} catch(const std::out_of_range& oor) {
		int id = 16 * hit_to_correct->GetDetector() + 4 * hit_to_correct->GetCrystal() + other_hit->GetCrystal();
		if(!been_warned[id]) {
			been_warned[id] = true;
			std::cerr<<DRED<<"Missing CT correction for Det: "<<hit_to_correct->GetDetector()
				<<" Crystals: "<<hit_to_correct->GetCrystal()<<" "<<other_hit->GetCrystal()<<" (id "<<id<<")"<<RESET_COLOR<<std::endl;
		}
		return hit_to_correct->GetEnergy();
	}

	return fixed_energy;
}

void TFipps::FixCrossTalk()
{
   if(!TGRSIOptions::AnalysisOptions()->IsCorrectingCrossTalk()) return;

   auto hit_vec = GetHitVector();
   if(hit_vec.size() < 2) {
      SetCrossTalk(true);
      return;
   }
   for(auto& hit : hit_vec) {
      static_cast<TFippsHit*>(hit)->ClearEnergy();
   }

	for(auto& one : hit_vec) {
		for(auto& two : hit_vec) {
			one->SetEnergy(CTCorrectedEnergy(static_cast<TFippsHit*>(one), static_cast<TFippsHit*>(two)));
      }
   }
   SetCrossTalk(true);
}
