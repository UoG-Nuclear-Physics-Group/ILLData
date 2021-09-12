#include "TIfinBgo.h"

#include <sstream>
#include <iostream>
#include <iomanip>

#include "TRandom.h"
#include "TMath.h"
#include "TInterpreter.h"

#include "TGRSIOptions.h"

////////////////////////////////////////////////////////////
//
// TIfinBgo
//
// The TIfinBgo class is just a differently name TBgo.
// This allows us to distinguish between the BGOs for diiferent
// detector classes.
//
////////////////////////////////////////////////////////////

/// \cond CLASSIMP
ClassImp(TIfinBgo)
/// \endcond

TIfinBgo::TIfinBgo() : TBgo()
{
	/// Default ctor.
   TBgo::Clear();
}

TIfinBgo::TIfinBgo(const TIfinBgo& rhs) : TBgo()
{
	/// Copy ctor.
   rhs.Copy(*this);
}

TIfinBgo::~TIfinBgo()
{
   // Default Destructor
}

TIfinBgo& TIfinBgo::operator=(const TIfinBgo& rhs)
{
   rhs.Copy(*this);
   return *this;
}
