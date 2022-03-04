#include "TFippsPulser.h"

#include "TClass.h"
#include <cmath>
#include "TMath.h"

#include "TGRSIOptions.h"

/// \cond CLASSIMP
ClassImp(TFippsPulser)
/// \endcond


TFippsPulser::TFippsPulser()
{
   Clear();
}

TFippsPulser::~TFippsPulser() = default;

TFippsPulser& TFippsPulser::operator=(const TFippsPulser& rhs)
{
   rhs.Copy(*this);
   return *this;
}

TFippsPulser::TFippsPulser(const TFippsPulser& rhs) : TDetector()
{
   rhs.Copy(*this);
}

void TFippsPulser::AddFragment(const std::shared_ptr<const TFragment>& frag, TChannel* chan)
{
	
   /// This function creates TFippsPulserHits for each fragment and stores them in separate front and back vectors
   if(frag == nullptr || chan == nullptr) {
      return;
   }

   auto hit = new TDetectorHit(*frag);
   fHits.push_back(hit);
}

void TFippsPulser::Print(Option_t*) const
{
   printf("%s\tnot yet written.\n", __PRETTY_FUNCTION__);
}
