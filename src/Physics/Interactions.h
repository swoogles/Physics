#ifndef INTERACTIONS_H_
#define INTERACTIONS_H_

#include <plib/sg.h>

#include "../Parallelization/Quadrant.h"
#include "../ShapeFiles/PairCollection.h"
#include <ShapeFiles/ParticleList.h>

#include "lib/units.h"

#include <Physics/UnitDefinitions.h>
using namespace units::energy;

using std::size_t;

class Interactions {
public:
    ParticleList crackPhysicalObject(Particle &shape, joule_t kineticEnergy, int numberOfPieces);
    PhysicalVector calcForceGravNew(Particle &object1, Moveable &object2, duration dt) ;
    void elasticCollision(Particle &object1, Particle &object2, duration dt);
};


#endif /* INTERACTIONS_H_ */

