#include "TFippsLaBrBgo.h"

#include <sstream>
#include <iostream>
#include <iomanip>

#include "TRandom.h"
#include "TMath.h"
#include "TInterpreter.h"

#include "TGRSIOptions.h"

////////////////////////////////////////////////////////////
//
// TFippsLaBrBgo
//
// The TFippsLaBrBgo class is just a differently name TBgo.
// This allows us to distinguish between the BGOs for diiferent
// detector classes.
//
////////////////////////////////////////////////////////////

/// \cond CLASSIMP
ClassImp(TFippsLaBrBgo)
/// \endcond

TFippsLaBrBgo::TFippsLaBrBgo() : TBgo()
{
	/// Default ctor.
   TBgo::Clear();
}

TFippsLaBrBgo::TFippsLaBrBgo(const TFippsLaBrBgo& rhs) : TBgo()
{
	/// Copy ctor.
   rhs.Copy(*this);
}

TFippsLaBrBgo::~TFippsLaBrBgo()
{
   // Default Destructor
}

TFippsLaBrBgo& TFippsLaBrBgo::operator=(const TFippsLaBrBgo& rhs)
{
   rhs.Copy(*this);
   return *this;
}
