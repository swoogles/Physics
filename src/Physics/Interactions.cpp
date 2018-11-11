#include "Interactions.h"

//TODO Fix and only apply to a single shape
void calcDrag(float dt, vectorT shapes) {
  float dragConstant = -30;

  for ( const auto & curShape : shapes ) {
    VecStruct dragForce(curShape->getVelocity());
    sgScaleVec4(dragForce.vec, dragConstant);
    sgScaleVec4(dragForce.vec, dt);
    curShape->adjustMomentum(dragForce.vec);
  }
}
