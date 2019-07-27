#include "TFippsBgo.h"

#include <sstream>
#include <iostream>
#include <iomanip>

#include "TRandom.h"
#include "TMath.h"
#include "TInterpreter.h"

#include "TGRSIOptions.h"

////////////////////////////////////////////////////////////
//
// TFippsBgo
//
// The TFippsBgo class is just a differently name TBgo.
// This allows us to distinguish between the BGOs for diiferent
// detector classes.
//
////////////////////////////////////////////////////////////

/// \cond CLASSIMP
ClassImp(TFippsBgo)
/// \endcond

TFippsBgo::TFippsBgo() : TBgo()
{
	/// Default ctor.
   TBgo::Clear();
}

TFippsBgo::TFippsBgo(const TFippsBgo& rhs) : TBgo()
{
	/// Copy ctor.
   rhs.Copy(*this);
}

TFippsBgo::~TFippsBgo()
{
   // Default Destructor
}

TFippsBgo& TFippsBgo::operator=(const TFippsBgo& rhs)
{
   rhs.Copy(*this);
   return *this;
}
