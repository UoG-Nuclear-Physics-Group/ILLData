#ifndef TILLDETECTORINFORMATION_H
#define TILLDETECTORINFORMATION_H

/** \addtogroup Sorting
 *  @{
 */

/////////////////////////////////////////////////////////////////
///
/// \class TILLDetectorInformation
///
/// This Class is designed to store setup dependent
/// information. It is used to store existence of
/// detector systems, etc.
///
/////////////////////////////////////////////////////////////////

#include "Globals.h"

#include "TDetectorInformation.h"
#include "TChannel.h"

class TILLDetectorInformation : public TDetectorInformation {
public:
   TILLDetectorInformation();
   ~TILLDetectorInformation() override;
	
	TEventBuildingLoop::EBuildMode BuildMode() const override;

	void Print(Option_t* opt = "") const override;
	void Clear(Option_t* = "") override;
	void Set() override;

   inline void SetFipps(bool flag = true)    { fFipps = flag; }

   inline bool Fipps()    const { return fFipps; }

private:
   //  detector types to switch over in Set()
   //  for more info, see: https://www.triumf.info/wiki/tigwiki/index.php/Detector_Nomenclature

   bool fFipps{false}; // flag for Fipps on/off

   /// \cond CLASSIMP
   ClassDefOverride(TILLDetectorInformation, 1); // Contains the run-dependent information.
   /// \endcond
};
/*! @} */
#endif
