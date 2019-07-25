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

// This seems unnecessary, and why 17?;//  they are static members, and need
//  to be defined outside the header
//  17 is to have the detectors go from 1-16
//  plus we can use position zero
//  when the detector winds up back in
//  one of the stands like Alex used in the
//  gps run. pcb.
// Initiallizes the HPGe Clover positions as per the wiki
// <https://www.triumf.info/wiki/tigwiki/index.php/HPGe_Coordinate_Table>
//                                                                             theta                                 phi
//                                                                             theta                                phi
//                                                                             theta
TVector3 TFipps::gCloverPosition[17] = {
   TVector3(TMath::Sin(TMath::DegToRad() * (0.0)) * TMath::Cos(TMath::DegToRad() * (0.0)),
            TMath::Sin(TMath::DegToRad() * (0.0)) * TMath::Sin(TMath::DegToRad() * (0.0)),
            TMath::Cos(TMath::DegToRad() * (0.0))),
   // Downstream lampshade
   TVector3(TMath::Sin(TMath::DegToRad() * (45.0)) * TMath::Cos(TMath::DegToRad() * (67.5)),
            TMath::Sin(TMath::DegToRad() * (45.0)) * TMath::Sin(TMath::DegToRad() * (67.5)),
            TMath::Cos(TMath::DegToRad() * (45.0))),
   TVector3(TMath::Sin(TMath::DegToRad() * (45.0)) * TMath::Cos(TMath::DegToRad() * (157.5)),
            TMath::Sin(TMath::DegToRad() * (45.0)) * TMath::Sin(TMath::DegToRad() * (157.5)),
            TMath::Cos(TMath::DegToRad() * (45.0))),
   TVector3(TMath::Sin(TMath::DegToRad() * (45.0)) * TMath::Cos(TMath::DegToRad() * (247.5)),
            TMath::Sin(TMath::DegToRad() * (45.0)) * TMath::Sin(TMath::DegToRad() * (247.5)),
            TMath::Cos(TMath::DegToRad() * (45.0))),
   TVector3(TMath::Sin(TMath::DegToRad() * (45.0)) * TMath::Cos(TMath::DegToRad() * (337.5)),
            TMath::Sin(TMath::DegToRad() * (45.0)) * TMath::Sin(TMath::DegToRad() * (337.5)),
            TMath::Cos(TMath::DegToRad() * (45.0))),
   // Corona
   TVector3(TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Cos(TMath::DegToRad() * (22.5)),
            TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Sin(TMath::DegToRad() * (22.5)),
            TMath::Cos(TMath::DegToRad() * (90.0))),
   TVector3(TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Cos(TMath::DegToRad() * (67.5)),
            TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Sin(TMath::DegToRad() * (67.5)),
            TMath::Cos(TMath::DegToRad() * (90.0))),
   TVector3(TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Cos(TMath::DegToRad() * (112.5)),
            TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Sin(TMath::DegToRad() * (112.5)),
            TMath::Cos(TMath::DegToRad() * (90.0))),
   TVector3(TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Cos(TMath::DegToRad() * (157.5)),
            TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Sin(TMath::DegToRad() * (157.5)),
            TMath::Cos(TMath::DegToRad() * (90.0))),
   TVector3(TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Cos(TMath::DegToRad() * (202.5)),
            TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Sin(TMath::DegToRad() * (202.5)),
            TMath::Cos(TMath::DegToRad() * (90.0))),
   TVector3(TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Cos(TMath::DegToRad() * (247.5)),
            TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Sin(TMath::DegToRad() * (247.5)),
            TMath::Cos(TMath::DegToRad() * (90.0))),
   TVector3(TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Cos(TMath::DegToRad() * (292.5)),
            TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Sin(TMath::DegToRad() * (292.5)),
            TMath::Cos(TMath::DegToRad() * (90.0))),
   TVector3(TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Cos(TMath::DegToRad() * (337.5)),
            TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Sin(TMath::DegToRad() * (337.5)),
            TMath::Cos(TMath::DegToRad() * (90.0))),
   // Upstream lampshade
   TVector3(TMath::Sin(TMath::DegToRad() * (135.0)) * TMath::Cos(TMath::DegToRad() * (67.5)),
            TMath::Sin(TMath::DegToRad() * (135.0)) * TMath::Sin(TMath::DegToRad() * (67.5)),
            TMath::Cos(TMath::DegToRad() * (135.0))),
   TVector3(TMath::Sin(TMath::DegToRad() * (135.0)) * TMath::Cos(TMath::DegToRad() * (157.5)),
            TMath::Sin(TMath::DegToRad() * (135.0)) * TMath::Sin(TMath::DegToRad() * (157.5)),
            TMath::Cos(TMath::DegToRad() * (135.0))),
   TVector3(TMath::Sin(TMath::DegToRad() * (135.0)) * TMath::Cos(TMath::DegToRad() * (247.5)),
            TMath::Sin(TMath::DegToRad() * (135.0)) * TMath::Sin(TMath::DegToRad() * (247.5)),
            TMath::Cos(TMath::DegToRad() * (135.0))),
   TVector3(TMath::Sin(TMath::DegToRad() * (135.0)) * TMath::Cos(TMath::DegToRad() * (337.5)),
            TMath::Sin(TMath::DegToRad() * (135.0)) * TMath::Sin(TMath::DegToRad() * (337.5)),
            TMath::Cos(TMath::DegToRad() * (135.0)))};

// Cross Talk stuff
const Double_t TFipps::gStrongCT[2]           = {-0.02674, -0.000977}; // This is for the 0-1 and 2-3 combination
const Double_t TFipps::gWeakCT[2]             = {0.005663, -0.00028014};
const Double_t TFipps::gCrossTalkPar[2][4][4] = {{{0.0, gStrongCT[0], gWeakCT[0], gWeakCT[0]},
                                                  {gStrongCT[0], 0.0, gWeakCT[0], gWeakCT[0]},
                                                  {gWeakCT[0], gWeakCT[0], 0.0, gStrongCT[0]},
                                                  {gWeakCT[0], gWeakCT[0], gStrongCT[0], 0.0}},
                                                 {{0.0, gStrongCT[1], gWeakCT[1], gWeakCT[1]},
                                                  {gStrongCT[1], 0.0, gWeakCT[1], gWeakCT[1]},
                                                  {gWeakCT[1], gWeakCT[1], 0.0, gStrongCT[1]},
                                                  {gWeakCT[1], gWeakCT[1], gStrongCT[1], 0.0}}};

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

   static_cast<TFipps&>(rhs).fAddbackHits  = fAddbackHits;
   static_cast<TFipps&>(rhs).fAddbackFrags = fAddbackFrags;
   static_cast<TFipps&>(rhs).fFippsBits    = 0;
}

TFipps::~TFipps()
{
   // Default Destructor
}

void TFipps::Clear(Option_t* opt)
{
   // Clears the mother, and all of the hits
   ClearStatus();
   TSuppressed::Clear(opt);
   fAddbackHits.clear();
   fAddbackFrags.clear();
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
				auto geHit = new TFippsHit(*frag);
				fHits.push_back(std::move(geHit));
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

   TVector3 temp_pos(gCloverPosition[DetNbr]);

   // Interaction points may eventually be set externally. May make these members of each crystal, or pass from
   // waveforms.
   Double_t cp = 26.0; // Crystal Center Point  mm.
   Double_t id = 45.0; // 45.0;  //Crystal interaction depth mm.
   // Set Theta's of the center of each DETECTOR face
   ////Define one Detector position
   TVector3 shift;
   switch(CryNbr) {
   case 0: shift.SetXYZ(-cp, cp, id); break;
   case 1: shift.SetXYZ(cp, cp, id); break;
   case 2: shift.SetXYZ(cp, -cp, id); break;
   case 3: shift.SetXYZ(-cp, -cp, id); break;
   default: shift.SetXYZ(0, 0, 1); break;
   };
   shift.RotateY(temp_pos.Theta());
   shift.RotateZ(temp_pos.Phi());

   temp_pos.SetMag(dist);

   return (temp_pos + shift);
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
      printf("One of the hits is invalid in TFipps::CTCorrectedEnergy\n");
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
   return hit_to_correct->GetEnergy() -
          (gCrossTalkPar[0][hit_to_correct->GetCrystal()][other_hit->GetCrystal()] +
           gCrossTalkPar[1][hit_to_correct->GetCrystal()][other_hit->GetCrystal()] * other_hit->GetNoCTEnergy());
}

void TFipps::FixCrossTalk()
{
   auto hit_vec = GetHitVector();
   if(hit_vec.size() < 2) {
      SetCrossTalk(true);
      return;
   }
   for(auto& i : hit_vec) {
      i->ClearEnergy();
   }

   if(TGRSIOptions::AnalysisOptions()->IsCorrectingCrossTalk()) {
      size_t i, j;
      for(i = 0; i < hit_vec.size(); ++i) {
         for(j = i + 1; j < hit_vec.size(); ++j) {
            hit_vec.at(i)->SetEnergy(TFipps::CTCorrectedEnergy(static_cast<TFippsHit*>(hit_vec.at(i)), static_cast<TFippsHit*>(hit_vec.at(j))));
            hit_vec.at(j)->SetEnergy(TFipps::CTCorrectedEnergy(static_cast<TFippsHit*>(hit_vec.at(j)), static_cast<TFippsHit*>(hit_vec.at(i))));
         }
      }
   }
   SetCrossTalk(true);
}
