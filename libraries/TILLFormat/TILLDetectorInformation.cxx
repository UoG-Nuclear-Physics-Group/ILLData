#include "TILLDetectorInformation.h"

#include <iostream>

#include "TROOT.h"

#include "TILLMnemonic.h"

/// \cond CLASSIMP
ClassImp(TILLDetectorInformation)
/// \endcond

TILLDetectorInformation::TILLDetectorInformation() : TDetectorInformation()
{
   /// Default ctor for TILLDetectorInformation.
   Clear();
	Set();
}

TILLDetectorInformation::~TILLDetectorInformation() = default;

TEventBuildingLoop::EBuildMode TILLDetectorInformation::BuildMode() const
{
	return TEventBuildingLoop::EBuildMode::kTimestamp;
}

void TILLDetectorInformation::Print(Option_t* opt) const
{
   // Prints the TILLDetectorInformation. Options:
   // a: Print out more details.
   if(strchr(opt, 'a') != nullptr) {
      printf("\t\tFIPPS:            %s\n", Fipps() ? "true" : "false");
      printf("\t\tFIPPSBGO:         %s\n", FippsBgo() ? "true" : "false");
      printf("\t\tFIPPSLABR:        %s\n", FippsLaBr() ? "true" : "false");
      printf("\t\tFIPPSLABRBGO:     %s\n", FippsLaBrBgo() ? "true" : "false");
      printf("\t\tFIPPSTAC:         %s\n", FippsTAC() ? "true" : "false");
      printf("\t\tFIPPSPULSER:      %s\n", FippsPulser() ? "true" : "false");
      printf("\n");
   }
}

void TILLDetectorInformation::Clear(Option_t*)
{
   // Clears the TILLDetectorInformation. Currently, there are no available
   // options.

	TDetectorInformation::Clear();

   fFipps = false;
}

void TILLDetectorInformation::Set()
{
   /// Sets the run info. This figures out what systems are available.
   std::map<unsigned int, TChannel*>::iterator iter;
   for(iter = TChannel::GetChannelMap()->begin(); iter != TChannel::GetChannelMap()->end(); iter++) {
      std::string channelname = iter->second->GetName();

      //  detector system type.
      //  for more info, see: https://www.triumf.info/wiki/tigwiki/index.php/Detector_Nomenclature
      switch(static_cast<const TILLMnemonic*>(iter->second->GetMnemonic())->System()) {
			case TILLMnemonic::ESystem::kFipps:
				SetFipps();
				break;
			case TILLMnemonic::ESystem::kFippsBgo:
				SetFippsBgo();
				break;
			case TILLMnemonic::ESystem::kFippsLaBr:
				SetFippsLaBr();
				break;
			case TILLMnemonic::ESystem::kFippsLaBrBgo:
				SetFippsLaBrBgo();
				break;
			case TILLMnemonic::ESystem::kFippsTAC:
				SetFippsTAC();
				break;
			case TILLMnemonic::ESystem::kFippsPulser:
				SetFippsPulser();
				break;
			default:
				break;
		};
	}
}

