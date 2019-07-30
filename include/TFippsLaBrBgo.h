#ifndef TLABRBGO_H
#define TLABRBGO_H

/** \addtogroup Detectors
 *  @{
 */

#include "Globals.h"
#include "TBgo.h"

class TFippsLaBrBgo : public TBgo {
public:
   TFippsLaBrBgo();
   TFippsLaBrBgo(const TFippsLaBrBgo&);
   virtual ~TFippsLaBrBgo();

   TFippsLaBrBgo& operator=(const TFippsLaBrBgo&); //!<!

   /// \cond CLASSIMP
   ClassDef(TFippsLaBrBgo, 1) // LaBrBgo Physics structure
   /// \endcond
};
/*! @} */
#endif
