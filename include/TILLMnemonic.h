#ifndef TILLMNEMONIC_H
#define TILLMNEMONIC_H

#include <string>
#include "TMnemonic.h"
#include "Globals.h"
#include "TClass.h"

class TILLMnemonic : public TMnemonic {
public:
   TILLMnemonic() : TMnemonic() { Clear(); }
   TILLMnemonic(const char* name) : TILLMnemonic() { TMnemonic::Parse(name); }
   ~TILLMnemonic() override = default;

   // standard C++ makes these enumerations global to the class. ie, the name of the enumeration
   // EMnemonic or ESystem has no effect on the clashing of enumerated variable names.
   // These separations exist only to easily see the difference when looking at the code here.
   enum class ESystem {
      kFipps,           //0
		kClear            //1
   };
   enum class EDigitizer { kDefault, kCAEN8, kCaen };

   ESystem   System() const { return fSystem; }

   void Parse(std::string* name) override;

   static EDigitizer EnumerateDigitizer(std::string name);

	TClass* GetClassType() const override;
   void Print(Option_t* opt = "") const override;
   void Clear(Option_t* opt = "") override;

private:
   ESystem fSystem;

   void EnumerateSystem();

   /// \cond CLASSIMP
   ClassDefOverride(TILLMnemonic, 1)
   /// \endcond
};

#endif
