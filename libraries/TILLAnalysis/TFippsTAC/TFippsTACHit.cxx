#include "TFippsTACHit.h"

#include <iostream>
#include <algorithm>
#include <climits>

#include "Globals.h"
#include "TFippsTAC.h"

/// \cond CLASSIMP
ClassImp(TFippsTACHit)
/// \endcond

TFippsTACHit::TFippsTACHit()
{
	// Default Constructor
#if ROOT_VERSION_CODE < ROOT_VERSION(6,0,0)
	Class()->IgnoreTObjectStreamer(kTRUE);
#endif
	Clear();
}

TFippsTACHit::~TFippsTACHit() = default;

TFippsTACHit::TFippsTACHit(const TFippsTACHit& rhs) : TDetectorHit()
{
	// Copy Constructor
#if ROOT_VERSION_CODE < ROOT_VERSION(6,0,0)
	Class()->IgnoreTObjectStreamer(kTRUE);
#endif
	Clear();
	rhs.Copy(*this);
}

void TFippsTACHit::Copy(TObject& rhs) const
{
	// Copies a TFippsTACHit
	TDetectorHit::Copy(rhs);
}

void TFippsTACHit::Copy(TObject& obj, bool) const
{
	Copy(obj);
}

Double_t TFippsTACHit::GetTempCorrectedCharge(TGraph* correction_graph) const {
	//Applies the kValue ot the charge
	if(!correction_graph){
		std::cout << "Graph for temperture corrections is null" << std::endl;
	}

	return GetCharge()*correction_graph->Eval(GetTime()/1e9);//The graph should be defined in seconds
}

Double_t TFippsTACHit::TempCorrectedCharge(TGraph* correction_graph) const {
	//Returns the raw charge with no kValue applied
	if(!correction_graph){
		std::cout << "Graph for temperture corrections is null" << std::endl;
	}

	return Charge()*correction_graph->Eval(GetTime()/1e9);//The graph should be defined in seconds
}

Double_t TFippsTACHit::GetTempCorrectedEnergy(TGraph* correction_graph) const {
	//This will not overwrite the normal energy, nor will it get stored as the energy.
	if(!correction_graph){
		std::cout << "Graph for temperture corrections is null" << std::endl;
	}

	TChannel* channel = GetChannel();
	if(channel == nullptr) {
		return 0.0;
	}
	if(GetKValue() > 0) {
		return channel->CalibrateENG(TempCorrectedCharge(correction_graph), GetKValue());
	} else if(channel->UseCalFileIntegration()) {
		return channel->CalibrateENG(TempCorrectedCharge(correction_graph), 0);
	}
	return channel->CalibrateENG(TempCorrectedCharge(correction_graph));
}

void TFippsTACHit::Clear(Option_t*)
{
	// Clears the TACHit
	TDetectorHit::Clear();
}

void TFippsTACHit::Print(Option_t*) const
{
	// Prints the TACHit. Returns:
	// Detector
	// Energy
	// Time
	Print(std::cout);
}

void TFippsTACHit::Print(std::ostream& out) const
{
	std::ostringstream str;
	str<<"TAC Detector:   "<<GetDetector()<<std::endl;
	str<<"TAC hit energy: "<<GetEnergy()<<std::endl;
	str<<"TAC hit time:   "<<GetTime()<<std::endl;
	out<<str.str();
}
