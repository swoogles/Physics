#ifndef SIMULATION_H_
#define SIMULATION_H_

#include "../Parallelization/Quadrant.h"
#include "CollisionType.h"
#include <ShapeFiles/ParticleList.h>
#include "../MaximumValues.h"
#include <Physics/Interactions.h>
#include "lib/units.h"

using namespace units::time;


class Simulation {
public:
    Simulation(ParticleList physicalObjects, CollisionType collisionType, float octreeTheta);

    Simulation(Simulation & originalSimulation, ParticleList newParticles);


    hour_t getTimeElapsed() const;

    // TODO Take dt as a parameter here.
    void update(hour_t dt);

    MaximumValues getXYMinsAndMaxes() const;

    size_t getSize() const;

    QuadrantPointer_t getQuadrant() const;
public:
    ParticleList &getPhysicalObjects() ;

private:
    ParticleList physicalObjects;
    hour_t timeElapsed;

	float minX, maxX, minY, maxY;

    float octreeTheta;

    QuadrantPointer_t quadrant;

    CollisionType collisionType;

    Interactions interactions;

	void updateTimeElapsed(hour_t dt);

	void updateMinsAndMaxes();
    void resetXYMinsAndMaxes();
    void updateXYMinsAndMaxes(PhysicalVector curPos);

	void removePhysicalObjects( ParticleList shapesToRemove );

    void refreshQuadrant();

	void calculateForceOnExternalNode(Particle &curObject, Quadrant &curQuadrant, hour_t dt) const;
	void calcForceOnObject_Octree(Particle &curObject, Quadrant &curQuadrant, hour_t dt,
											int recursionLevel) const;

    void calcForcesAll(hour_t dt);

} ;

#endif
