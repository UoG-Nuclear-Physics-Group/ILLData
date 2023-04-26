#include "TIfin.h"

#include <sstream>
#include <iostream>
#include <iomanip>

#include "TRandom.h"
#include "TMath.h"
#include "TInterpreter.h"
#include "TMnemonic.h"

#include "TGRSIOptions.h"

/// \cond CLASSIMP
ClassImp(TIfin)
/// \endcond


bool DefaultIfinAddback(const TDetectorHit* one, const TDetectorHit* two)
{
   return ((one->GetDetector() == two->GetDetector()) &&
           (std::fabs(one->GetTime() - two->GetTime()) < TGRSIOptions::AnalysisOptions()->AddbackWindow()));
}

std::function<bool(const TDetectorHit*, const TDetectorHit*)> TIfin::fAddbackCriterion = DefaultIfinAddback;

bool DefaultIfinSuppression(const TDetectorHit* hit, const TDetectorHit* bgoHit)
{
	return ((hit->GetDetector() == bgoHit->GetDetector()) &&
	(std::fabs(hit->GetTime() - bgoHit->GetTime()) < TGRSIOptions::AnalysisOptions()->SuppressionWindow()) &&
	(bgoHit->GetEnergy() > TGRSIOptions::AnalysisOptions()->SuppressionEnergy()));
}

std::function<bool(const TDetectorHit*, const TDetectorHit*)> TIfin::fSuppressionCriterion = DefaultIfinSuppression;

// Ifin detector locations.
// Angles are in ISO standard
// x = cos(theta)*sin(phi)      // Points port
// y = sin(theta)*sin(phi)      // Points upwards
// z = sin(theta)               // Points in the direction of the neutrons
// TVector(x,y,z)
// TODO: Add link to picture showing detector positions when uploaded
TVector3 TIfin::gCloverPosition[17] = {
   TVector3(TMath::Sin(TMath::DegToRad() * (0.0)) * TMath::Cos(TMath::DegToRad() * (0.0)),
            TMath::Sin(TMath::DegToRad() * (0.0)) * TMath::Sin(TMath::DegToRad() * (0.0)),
            TMath::Cos(TMath::DegToRad() * (0.0))), // Zeroth Position
   // Corona
   TVector3(TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Cos(TMath::DegToRad() * (90.0)),
            TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Sin(TMath::DegToRad() * (90.0)),
            TMath::Cos(TMath::DegToRad() * (90.0))), // Ifin Pos 0
   TVector3(TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Cos(TMath::DegToRad() * (45.0)),
            TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Sin(TMath::DegToRad() * (45.0)),
            TMath::Cos(TMath::DegToRad() * (90.0))), // Ifin Pos 1
   TVector3(TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Cos(TMath::DegToRad() * (0.0)),
            TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Sin(TMath::DegToRad() * (0.0)),
            TMath::Cos(TMath::DegToRad() * (90.0))), // Ifin Pos 2
   TVector3(TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Cos(TMath::DegToRad() * (315.0)),
            TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Sin(TMath::DegToRad() * (315.0)),
            TMath::Cos(TMath::DegToRad() * (90.0))), // Ifin Pos 3
   TVector3(TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Cos(TMath::DegToRad() * (270.0)),
            TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Sin(TMath::DegToRad() * (270.0)),
            TMath::Cos(TMath::DegToRad() * (90.0))), // Ifin Pos 4
   TVector3(TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Cos(TMath::DegToRad() * (225.0)),
            TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Sin(TMath::DegToRad() * (225.0)),
            TMath::Cos(TMath::DegToRad() * (90.0))), // Ifin Pos 5
   TVector3(TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Cos(TMath::DegToRad() * (180.0)),
            TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Sin(TMath::DegToRad() * (180.0)),
            TMath::Cos(TMath::DegToRad() * (90.0))), // Ifin Pos 6
   TVector3(TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Cos(TMath::DegToRad() * (135.0)),
            TMath::Sin(TMath::DegToRad() * (90.0)) * TMath::Sin(TMath::DegToRad() * (135.0)),
            TMath::Cos(TMath::DegToRad() * (90.0))), // Ifin Pos 7
   // Downstream lampshade
   TVector3(TMath::Sin(TMath::DegToRad() * (45.0)) * TMath::Cos(TMath::DegToRad() * (90.0)),
            TMath::Sin(TMath::DegToRad() * (45.0)) * TMath::Sin(TMath::DegToRad() * (90.0)),
            TMath::Cos(TMath::DegToRad() * (45.0))), // Ifin Pos 8
   TVector3(TMath::Sin(TMath::DegToRad() * (45.0)) * TMath::Cos(TMath::DegToRad() * (0.0)),
            TMath::Sin(TMath::DegToRad() * (45.0)) * TMath::Sin(TMath::DegToRad() * (0.0)),
            TMath::Cos(TMath::DegToRad() * (45.0))), // Ifin Pos 9
   TVector3(TMath::Sin(TMath::DegToRad() * (45.0)) * TMath::Cos(TMath::DegToRad() * (270.0)),
            TMath::Sin(TMath::DegToRad() * (45.0)) * TMath::Sin(TMath::DegToRad() * (270.0)),
            TMath::Cos(TMath::DegToRad() * (45.0))), // Ifin Pos 10
   TVector3(TMath::Sin(TMath::DegToRad() * (45.0)) * TMath::Cos(TMath::DegToRad() * (180.0)),
            TMath::Sin(TMath::DegToRad() * (45.0)) * TMath::Sin(TMath::DegToRad() * (180.0)),
            TMath::Cos(TMath::DegToRad() * (45.0))), // Ifin Pos 11
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

// Cross Talk stuff
const Double_t TIfin::gStrongCT[2]           = {-0.02674, -0.000977}; // This is for the 0-1 and 2-3 combination
const Double_t TIfin::gWeakCT[2]             = {0.005663, -0.00028014};
const Double_t TIfin::gCrossTalkPar[2][4][4] = {{{0.0, gStrongCT[0], gWeakCT[0], gWeakCT[0]},
                                                  {gStrongCT[0], 0.0, gWeakCT[0], gWeakCT[0]},
                                                  {gWeakCT[0], gWeakCT[0], 0.0, gStrongCT[0]},
                                                  {gWeakCT[0], gWeakCT[0], gStrongCT[0], 0.0}},
                                                 {{0.0, gStrongCT[1], gWeakCT[1], gWeakCT[1]},
                                                  {gStrongCT[1], 0.0, gWeakCT[1], gWeakCT[1]},
                                                  {gWeakCT[1], gWeakCT[1], 0.0, gStrongCT[1]},
                                                  {gWeakCT[1], gWeakCT[1], gStrongCT[1], 0.0}}};

TIfin::TIfin() : TSuppressed()
{
// Default ctor. Ignores TObjectStreamer in ROOT < 6
#if MAJOR_ROOT_VERSION < 6
   Class()->IgnoreTObjectStreamer(kTRUE);
#endif
   Clear();
}

TIfin::TIfin(const TIfin& rhs) : TSuppressed()
{
// Copy ctor. Ignores TObjectStreamer in ROOT < 6
#if MAJOR_ROOT_VERSION < 6
   Class()->IgnoreTObjectStreamer(kTRUE);
#endif
   rhs.Copy(*this);
}

void TIfin::Copy(TObject& rhs) const
{
   // Copy function.
   TSuppressed::Copy(rhs);

   for(auto& hit : static_cast<TIfin&>(rhs).fAddbackHits) delete hit;
   for(auto& hit : static_cast<TIfin&>(rhs).fSuppressedHits) delete hit;
   for(auto& hit : static_cast<TIfin&>(rhs).fSuppressedAddbackHits) delete hit;

   static_cast<TIfin&>(rhs).fAddbackHits.clear();
   static_cast<TIfin&>(rhs).fAddbackFrags.clear();
   static_cast<TIfin&>(rhs).fSuppressedHits.clear();
   static_cast<TIfin&>(rhs).fSuppressedAddbackHits.clear();
   static_cast<TIfin&>(rhs).fSuppressedAddbackFrags.clear();
   static_cast<TIfin&>(rhs).fIfinBits    = 0;
}

TIfin::~TIfin()
{
   // Default Destructor

   // fHits automatically deleted in TDetector
   for(auto& hit : fAddbackHits) delete hit;
   for(auto& hit : fSuppressedHits) delete hit;
   for(auto& hit : fSuppressedAddbackHits) delete hit;

}

void TIfin::Clear(Option_t* opt)
{
   // Clears the mother, and all of the hits
   ClearStatus();
   TSuppressed::Clear(opt);

   for(auto& hit : fAddbackHits) delete hit;
   for(auto& hit : fSuppressedHits) delete hit;
   for(auto& hit : fSuppressedAddbackHits) delete hit;

   fAddbackHits.clear();
   fSuppressedHits.clear();
   fSuppressedAddbackHits.clear();
   fAddbackFrags.clear();
   fSuppressedAddbackFrags.clear();
}

void TIfin::Print(Option_t*) const
{
	Print(std::cout);
}

void TIfin::Print(std::ostream& out) const
{
	std::ostringstream str;
   str<<"Ifin Contains: "<<std::endl;
   str<<std::setw(6)<<GetMultiplicity()<<" hits"<<std::endl;

   if(IsAddbackSet()) {
      str<<std::setw(6)<<fAddbackHits.size()<<" addback hits"<<std::endl;
   } else {
      str<<std::setw(6)<<" "<<" Addback not set"<<std::endl;
   }

   str<<std::setw(6)<<" "<<" Cross-talk Set?  "<<IsCrossTalkSet()<<std::endl;
	out<<str.str();
}

TIfin& TIfin::operator=(const TIfin& rhs)
{
   rhs.Copy(*this);
   return *this;
}


std::vector<TDetectorHit*>& TIfin::GetAddbackVector()
{
   return fAddbackHits;
}

std::vector<UShort_t>& TIfin::GetAddbackFragVector()
{
   return fAddbackFrags;
}

std::vector<TDetectorHit*>& TIfin::GetSuppressedVector()
{
    return fSuppressedHits;
}

std::vector<TDetectorHit*>& TIfin::GetSuppressedAddbackVector()
{
    return fSuppressedAddbackHits;
}

std::vector<UShort_t>& TIfin::GetSuppressedAddbackFragVector()
{
    return fSuppressedAddbackFrags;
}

bool TIfin::IsAddbackSet() const
{
   return TestBitNumber(EIfinBits::kIsAddbackSet);
}

bool TIfin::IsCrossTalkSet() const
{
   return TestBitNumber(EIfinBits::kIsCrossTalkSet);
}

bool TIfin::IsSuppressed() const
{
    return TestBitNumber(EIfinBits::kIsSuppressedSet);
}

bool TIfin::IsSuppressedAddbackSet() const
{
    return TestBitNumber(EIfinBits::kIsSuppressedAddbackSet);
}

void TIfin::SetAddback(const Bool_t flag) const
{
   return SetBitNumber(EIfinBits::kIsAddbackSet, flag);
}

void TIfin::SetCrossTalk(const Bool_t flag) const
{
   return SetBitNumber(EIfinBits::kIsCrossTalkSet, flag);
}

void TIfin::SetSuppressed(const Bool_t flag) const
{
    return SetBitNumber(EIfinBits::kIsSuppressedSet, flag);
}

void TIfin::SetSuppressedAddback(const Bool_t flag) const
{
    return SetBitNumber(EIfinBits::kIsSuppressedAddbackSet, flag);
}

TIfinHit* TIfin::GetIfinHit(const int& i)
{
   try {
      if(!IsCrossTalkSet()) {
         FixCrossTalk();
      }
      return static_cast<TIfinHit*>(fHits.at(i));
   } catch(const std::out_of_range& oor) {
      std::cerr<<ClassName()<<" Hits are out of range: "<<oor.what()<<std::endl;
      if(!gInterpreter) {
         throw grsi::exit_exception(1);
      }
   }
   return nullptr;
}

TIfinHit* TIfin::GetSuppressedHit(const int& i)
{
    try {
        if(!IsCrossTalkSet()) {
            FixCrossTalk();
        }
        return static_cast<TIfinHit*>(fSuppressedHits.at(i));
   } catch(const std::out_of_range& oor) {
      std::cerr<<ClassName()<<"Suppressed hits are out of range: "<<oor.what()<<std::endl;
      if(!gInterpreter) {
         throw grsi::exit_exception(1);
      }
   }
   return nullptr;
}

Int_t TIfin::GetSuppressedMultiplicity(const TBgo* bgo)
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
		for(auto& hit : sup_vec) {
			delete hit;
		}
      sup_vec.clear();
   }
   if(sup_vec.empty()) {
		CreateSuppressed(bgo, hit_vec, sup_vec);
      SetSuppressed(true);
   }

   return sup_vec.size();
}
    

Int_t TIfin::GetAddbackMultiplicity()
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
		for(auto& hit : ab_vec) {
			delete hit;
		}
      ab_vec.clear();
      frag_vec.clear();
   }
   if(ab_vec.empty()) {
		CreateAddback(hit_vec, ab_vec, frag_vec);
      SetAddback(true);
   }

   return ab_vec.size();
}

Int_t TIfin::GetSuppressedAddbackMultiplicity(const TBgo* bgo)
{
   // Automatically builds the addback hits using the fAddbackCriterion (if
   // the size of the fAddbackHits vector is zero) and return the number of
   // addback hits.
   if(!IsCrossTalkSet()) {
      FixCrossTalk();
   }
   auto& hit_vec  = GetHitVector();
   auto& ab_vec   = GetSuppressedAddbackVector();
   auto& frag_vec = GetSuppressedAddbackFragVector();
   if(hit_vec.empty()) {
      return 0;
   }

   // if the addback has been reset, clear the addback hits
   if(!IsAddbackSet()) {
		for(auto& hit : ab_vec) {
			delete hit;
		}
      ab_vec.clear();
      frag_vec.clear();
   }
   if(ab_vec.empty()) {
		CreateSuppressedAddback(bgo, hit_vec, ab_vec, frag_vec);
        SetSuppressedAddback(true);
   }

   return ab_vec.size();
}


TIfinHit* TIfin::GetAddbackHit(const int& i)
{
   try{
       if(!IsCrossTalkSet()) {
           FixCrossTalk();
       }
       return static_cast<TIfinHit*>(GetAddbackVector().at(i));
   } catch(const std::out_of_range& oor) {
       std::cerr<<ClassName()<<" Addback hits are out of range: "<<oor.what()<<std::endl;
       if(!gInterpreter) {
           throw grsi::exit_exception(1);
       }
   }
   return nullptr;
}

TIfinHit* TIfin::GetSuppressedAddbackHit(const int& i)
{
   try{
       if(!IsCrossTalkSet()) {
           FixCrossTalk();
       }
       return static_cast<TIfinHit*>(GetSuppressedAddbackVector().at(i));
   } catch(const std::out_of_range& oor) {
       std::cerr<<ClassName()<<" Suppressed addback hits are out of range: "<<oor.what()<<std::endl;
       if(!gInterpreter) {
           throw grsi::exit_exception(1);
       }
   }
   return nullptr;
}

void TIfin::AddFragment(const std::shared_ptr<const TFragment>& frag, TChannel* chan)
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
				auto geHit = new TIfinHit(*frag);
				fHits.push_back(std::move(geHit));
			}
			break;
		default:
			break;
   };
}

TVector3 TIfin::GetPosition(int DetNbr, int CryNbr, double dist)
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

void TIfin::ResetFlags() const
{
   fIfinBits = 0;
}

void TIfin::ResetAddback()
{
   SetAddback(false);
   SetCrossTalk(false);
	for(auto& hit : GetAddbackVector()) {
		delete hit;
	}
   GetAddbackVector().clear();
   GetAddbackFragVector().clear();
}

void TIfin::ResetSuppressed()
{
	SetSuppressed(false);
	for(auto& hit : GetSuppressedVector()) {
		delete hit;
	}
	GetSuppressedVector().clear();
}

void TIfin::ResetSuppressedAddback()
{
   SetAddback(false);
   SetCrossTalk(false);
   SetSuppressed(false);
	for(auto& hit : GetSuppressedAddbackVector()) {
		delete hit;
	}
   GetSuppressedAddbackVector().clear();
   GetSuppressedAddbackFragVector().clear();
}

UShort_t TIfin::GetNAddbackFrags(const size_t& idx)
{
   // Get the number of addback "fragments" contributing to the total addback hit
   // with index idx.
   if(idx < GetAddbackFragVector().size()) {
      return GetAddbackFragVector().at(idx);
   }
   return 0;
}

void TIfin::SetBitNumber(enum EIfinBits bit, Bool_t set) const
{
   // Used to set the flags that are stored in TIfin.
   fIfinBits.SetBit(bit, set);
}

Double_t TIfin::CTCorrectedEnergy(const TIfinHit* const hit_to_correct, const TIfinHit* const other_hit,
                                   Bool_t time_constraint)
{
	/// Corrects the energy of the hit to correct by ADDING the uncorrected energy of the other hit times the parameter for this combination
	/// This is different to the very similar TGriffin function that SUBTRACTS instead of ADDING.
	/// If time_constraint is true it also checks that the two hits are within the addback window to each other.
	if((hit_to_correct == nullptr) || (other_hit == nullptr)) {
		std::cerr<<"One of the hits is invalid in TIfin::CTCorrectedEnergy"<<std::endl;
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
			fixed_energy += hit_to_correct->GetChannel()->GetCTCoeff().at(other_hit->GetCrystal()) * other_hit->GetNoCTEnergy();
		}
	} catch(const std::out_of_range& oor) {
		int id = 16 * hit_to_correct->GetDetector() + 4 * hit_to_correct->GetCrystal() + other_hit->GetCrystal();
		if(!been_warned[id]) {
			been_warned[id] = true;
			std::cerr<<DRED<<"Missing CT correction for Det: "<<hit_to_correct->GetDetector()
				<<" Crystals: "<<hit_to_correct->GetCrystal()<<" "<<other_hit->GetCrystal()<<" (id "<<id<<")"<<std::endl;
		}
		return hit_to_correct->GetEnergy();
	}

	return fixed_energy;
}

void TIfin::FixCrossTalk()
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
		for(auto& one : hit_vec) {
			for(auto& two : hit_vec) {
				if(one == two) continue;
				one->SetEnergy(TIfin::CTCorrectedEnergy(static_cast<TIfinHit*>(one), static_cast<TIfinHit*>(two)));
			}
		}
   }
   SetCrossTalk(true);
}

const char* TIfin::GetColorFromNumber(int number)
{
	switch(number) {
		case 0: return "R";
		case 1: return "W";
		case 2: return "B";
		case 3: return "G";
	}
	return "X";
}
