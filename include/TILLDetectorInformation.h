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
   inline void SetFippsBgo(bool flag = true)    { fFippsBgo = flag; }
   inline void SetFippsLaBr(bool flag = true)    { fFippsLaBr = flag; }
   inline void SetFippsLaBrBgo(bool flag = true)    { fFippsLaBrBgo = flag; }
   inline void SetFippsTAC(bool flag = true)    { fFippsTAC = flag; }
   inline void SetFippsPulser(bool flag = true)    { fFippsPulser = flag; }

   inline bool Fipps()    const { return fFipps; }
   inline bool FippsBgo()   const { return fFippsBgo; }
   inline bool FippsLaBr() const { return fFippsLaBr; }
   inline bool FippsLaBrBgo() const { return fFippsLaBrBgo; }
   inline bool FippsTAC() const { return fFippsTAC; }
   inline bool FippsPulser() const { return fFippsPulser; }

private:
   //  detector types to switch over in Set()
   //  for more info, see: https://www.triumf.info/wiki/tigwiki/index.php/Detector_Nomenclature

   bool fFipps{false}; // flag for Fipps on/off
   bool fFippsBgo{false};
   bool fFippsLaBr{false};
   bool fFippsLaBrBgo{false};
   bool fFippsTAC{false};
   bool fFippsPulser{false};

   /// \cond CLASSIMP
   ClassDefOverride(TILLDetectorInformation, 1); // Contains the run-dependent information.
   /// \endcond
};
/*! @} */
#endif
