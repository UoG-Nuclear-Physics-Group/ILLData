#include "TILLMnemonic.h"

#include <algorithm>

// Detector dependent includes
#include "TFipps.h"
#include "TFippsBgo.h"
#include "TFippsLaBr.h"
#include "TFippsLaBrBgo.h"
#include "TFippsTAC.h"
#include "TFippsPulser.h"
#include "TIfin.h"
#include "TIfinBgo.h"

ClassImp(TILLMnemonic)

void TILLMnemonic::Clear(Option_t*)
{
	TMnemonic::Clear();
   fSystem = ESystem::kClear;
}

void TILLMnemonic::EnumerateSystem()
{
   // Enumerating the fSystemString must come after the total mnemonic has been parsed as the details of other parts of
   // the mnemonic must be known
   if(fSystemString.compare("FI") == 0) {
      if(SubSystem() == EMnemonic::kS) {
         fSystem = ESystem::kFippsBgo;
      } else {
          fSystem = ESystem::kFipps;
      }
   } else if(fSystemString.compare("IF") == 0) {
      if(SubSystem() == EMnemonic::kS) {
         fSystem = ESystem::kIfinBgo;
      } else {
          fSystem = ESystem::kIfin;
      }
   } else if(fSystemString.compare("LB") == 0) {
       if(SubSystem() == EMnemonic::kS ) {
           fSystem = ESystem::kFippsLaBrBgo;
       } else if(SubSystem() == EMnemonic::kT) {
           fSystem = ESystem::kFippsTAC;
       } else {
           fSystem = ESystem::kFippsLaBr;
       }
   } else if(fSystemString.compare("PU") == 0) {
       fSystem = ESystem::kFippsPulser;
   } else {
      fSystem = ESystem::kClear;
   }
}

void TILLMnemonic::EnumerateDigitizer(TPriorityValue<std::string>& digitizerName, TPriorityValue<EDigitizer>& digitizerType, TPriorityValue<int>& timeStampUnit)
{
	std::string name = digitizerName.Value();
	std::transform(name.begin(), name.end(), name.begin(), ::toupper);
	EDigitizer tmpType = EDigitizer::kDefault;
	int tmpUnit = 10;
	if(name.compare("V1724") == 0) {
		tmpType = EDigitizer::kV1724;
		tmpUnit = 10;
	} else if(name.compare("V1725") == 0) {
		tmpType = EDigitizer::kV1725;
		tmpUnit = 4; // ns
	} else if(name.compare("V1730") == 0) {
		tmpType = EDigitizer::kV1730;
		tmpUnit = 2; // ns
	} else if(name.compare("V1751") == 0) {
		tmpType = EDigitizer::kV1751;
		tmpUnit = 1; // ns
	} else {
		std::cout<<"Warning, digitizer type '"<<name<<"' not recognized, options are 'V1724', 'V1725', 'V1730', and 'V1751'!"<<std::endl;
	}
	digitizerType.Set(tmpType, digitizerName.Priority());
	timeStampUnit.Set(tmpUnit, digitizerName.Priority());
}

void TILLMnemonic::Parse(std::string* name)
{
	if((name == nullptr) || name->length() < 9) {
		return;
	}
	TMnemonic::Parse(name);
	// Enumerating the fSystemString must come last as the details of other parts of
	// the mnemonic must be known
	EnumerateSystem();
}

void TILLMnemonic::Print(Option_t*) const
{
	std::cout<<"====== ILLMNEMONIC ======"<<std::endl;
	std::cout<<"fArrayPosition           = "<<fArrayPosition<<std::endl;
	std::cout<<"fSegment                 = "<<fSegment<<std::endl;
	std::cout<<"fSystemString            = "<<fSystemString<<std::endl;
	std::cout<<"fSubSystemString         = "<<fSubSystemString<<std::endl;
	std::cout<<"fArraySubPositionString  = "<<fArraySubPositionString<<std::endl;
	std::cout<<"fCollectedChargeString   = "<<fCollectedChargeString<<std::endl;
	std::cout<<"fOutputSensorString      = "<<fOutputSensorString<<std::endl;
	std::cout<<"========================="<<std::endl;
}

TClass* TILLMnemonic::GetClassType() const
{
	if(fClassType != nullptr) {
		return fClassType;
	}

	switch(System()) {
		case ESystem::kFipps:        fClassType = TFipps::Class(); break;
		case ESystem::kFippsBgo:     fClassType = TFippsBgo::Class(); break;
		case ESystem::kFippsLaBr:    fClassType = TFippsLaBr::Class(); break;
		case ESystem::kFippsLaBrBgo: fClassType = TFippsLaBrBgo::Class(); break;
		case ESystem::kFippsTAC:     fClassType = TFippsTAC::Class(); break;
		case ESystem::kFippsPulser:  fClassType = TFippsPulser::Class(); break;
		case ESystem::kIfin:         fClassType = TIfin::Class(); break;
		case ESystem::kIfinBgo:      fClassType = TIfinBgo::Class(); break;
		default:                     fClassType = nullptr;
	};
	return fClassType;
}

double TILLMnemonic::GetTime(Long64_t timestamp, Float_t, double energy, const TChannel* channel) const
{
	if(channel == nullptr) {
      Error("GetTime", "No TChannel provided");
      return static_cast<Double_t>((timestamp) + gRandom->Uniform());
   }
	Double_t dTime = static_cast<Double_t>((timestamp) + gRandom->Uniform()) * channel->GetTimeStampUnit() + channel->CalibrateCFD(0.);
	return dTime - channel->GetTZero(energy) - channel->GetTimeOffset();
}
