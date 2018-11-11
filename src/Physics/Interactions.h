#ifndef INTERACTIONS_H_
#define INTERACTIONS_H_

#include <plib/sg.h>

#include "../Parallelization/Quadrant.h"
#include "../ShapeFiles/ShapeList.h"
#include "../ShapeFiles/PairCollection.h"
#include "Simulations.h"

using std::size_t;

/*! \brief Calculates new velocities after a collision
 *
 *  This function is called when 2 objects are determined to be too close to eachother, and either WorldSettings::isAllElastic() returns true or
 *  both objects have their elastic bools on.
 *
 */
void elasticCollision(MyShape &object1, MyShape &object2, float dt);

VecStruct calcForceGravNew(MyShape &object1, MyShape &object2, float dt) ;

/*! \brief Will eventually calculate drag force on a single object
 */
void calcDrag(float dt);

#endif /* INTERACTIONS_H_ */
