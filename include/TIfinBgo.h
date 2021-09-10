#ifndef TIFINBGO_H
#define TIFINBGO_H

/** \addtogroup Detectors
 *  @{
 */

#include "Globals.h"
#include "TBgo.h"

class TIfinBgo : public TBgo {
public:
   TIfinBgo();
   TIfinBgo(const TIfinBgo&);
   virtual ~TIfinBgo();

   TIfinBgo& operator=(const TIfinBgo&); //!<!

   /// \cond CLASSIMP
   ClassDef(TIfinBgo, 1) // GriffinBgo Physics structure
   /// \endcond
};
/*! @} */
#endif
