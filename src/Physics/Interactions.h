#ifndef INTERACTIONS_H_
#define INTERACTIONS_H_

#include <plib/sg.h>

#include "../Parallelization/Quadrant.h"
#include "../ShapeFiles/ShapeList.h"
#include "../ShapeFiles/PairCollection.h"
#include <ShapeFiles/ParticleList.h>
#include "Simulations.h"

using std::size_t;

class Interactions {
public:
    ParticleList crackPhysicalObject(Particle &shape, joule_t kineticEnergy, int numberOfPieces);
};

/*! \brief Calculates new velocities after a collision
 *
 *  This function is called when 2 objects are determined to be too close to eachother, and either WorldSettings::isAllElastic() returns true or
 *  both objects have their elastic bools on.
 *
 */
void elasticCollision(Moveable &object1, Moveable &object2, float dt);

PhysicalVector calcForceGravNew(Particle &object1, Moveable &object2, float dt) ;

/*! \brief Will eventually calculate drag force on a single object
 */
void calcDrag(float dt);

#endif /* INTERACTIONS_H_ */

