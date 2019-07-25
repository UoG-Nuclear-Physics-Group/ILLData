#include "TFippsTAC.h"
#include <iostream>
#include "TRandom.h"
#include "TMath.h"

/// \cond CLASSIMP
ClassImp(TFippsTAC)
/// \endcond

TFippsTAC::TFippsTAC()
{
	// Default Constructor
#if MAJOR_ROOT_VERSION < 6
	Class()->IgnoreTObjectStreamer(kTRUE);
#endif
	Clear();
}

TFippsTAC::~TFippsTAC()
{
	// Default Destructor
}

TFippsTAC::TFippsTAC(const TFippsTAC& rhs) : TDetector()
{
	// Copy Contructor
#if MAJOR_ROOT_VERSION < 6
	Class()->IgnoreTObjectStreamer(kTRUE);
#endif
	rhs.Copy(*this);
}

TFippsTAC& TFippsTAC::operator=(const TFippsTAC& rhs)
{
	rhs.Copy(*this);
	return *this;
}

void TFippsTAC::Print(Option_t*) const
{
	// Prints out TFippsTAC Multiplicity, currently does little.
	printf("%lu fHits\n", fHits.size());
}

void TFippsTAC::AddFragment(const std::shared_ptr<const TFragment>& frag, TChannel*)
{
	TFippsTACHit* hit = new TFippsTACHit(*frag);
	fHits.push_back(std::move(hit));
}
