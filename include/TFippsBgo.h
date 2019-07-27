#ifndef TFIPPSBGO_H
#define TFIPPSBGO_H

/** \addtogroup Detectors
 *  @{
 */

#include "Globals.h"
#include "TBgo.h"

class TFippsBgo : public TBgo {
public:
   TFippsBgo();
   TFippsBgo(const TFippsBgo&);
   virtual ~TFippsBgo();

   TFippsBgo& operator=(const TFippsBgo&); //!<!

   /// \cond CLASSIMP
   ClassDef(TFippsBgo, 1) // GriffinBgo Physics structure
   /// \endcond
};
/*! @} */
#endif
