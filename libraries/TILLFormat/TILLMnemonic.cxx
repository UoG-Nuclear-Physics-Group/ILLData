#include "TILLMnemonic.h"

#include <algorithm>

// Detector dependent includes
#include "TFipps.h"

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
      fSystem = ESystem::kFipps;
   } else {
      fSystem = ESystem::kClear;
   }
}

TILLMnemonic::EDigitizer TILLMnemonic::EnumerateDigitizer(std::string name)
{
   std::transform(name.begin(), name.end(), name.begin(), ::toupper);
   if(name.compare("CAEN8") == 0) {
      return EDigitizer::kCAEN8;
   }
   if(name.compare("CAEN") == 0) {
      return EDigitizer::kCaen;
   }
   return EDigitizer::kDefault;
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
   printf("====== ILLMNEMONIC ======\n");
   printf("fArrayPosition           = %i\n", fArrayPosition);
   printf("fSegment                 = %i\n", fSegment);
   printf("fSystemString            = %s\n", fSystemString.c_str());
   printf("fSubSystemString         = %s\n", fSubSystemString.c_str());
   printf("fArraySubPositionString  = %s\n", fArraySubPositionString.c_str());
   printf("fCollectedChargeString   = %s\n", fCollectedChargeString.c_str());
   printf("fOutputSensorString      = %s\n", fOutputSensorString.c_str());
   printf("===============================\n");
}

TClass* TILLMnemonic::GetClassType() const
{
   if(fClassType != nullptr) {
      return fClassType;
   }

   switch(System()) {
		case ESystem::kFipps: fClassType = TFipps::Class(); break;
		default:              fClassType = nullptr;
   };
   return fClassType;
}
