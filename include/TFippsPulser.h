#ifndef TFIPPSPULSER_H
#define TFIPPSPULSER_H

/** \addtogroup Detectors
 *  @{
 */

#include <iostream>

#include "TDetector.h"
#include "TChannel.h"
#include "TDetectorHit.h"

class TFippsPulser : public TDetector {
public:

   TFippsPulser();
   TFippsPulser(const TFippsPulser&);
   ~TFippsPulser() override;

#ifndef __CINT__
   void AddFragment(const std::shared_ptr<const TFragment>&, TChannel*) override; //!<!
#endif
	void BuildHits() override {} // no need to build any hits, everything already done in AddFragment

   TFippsPulser& operator=(const TFippsPulser&);                    //
   void Print(Option_t* opt = "") const override; //!<!
	void Print(std::ostream& out) const override;

private:
   void  ClearStatus() {  }

   /// \cond CLASSIMP
   ClassDefOverride(TFippsPulser, 1)
   /// \endcond
};
/*! @} */
#endif
