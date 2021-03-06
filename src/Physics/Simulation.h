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

	void applySideEffectingFunctionsToInnards(
			function<void (const Quadrant &)> quadrantFunctor,
			function<void (const Particle &)> particleFunctor
	)const ;


    hour_t getTimeElapsed() const;
    second_t getOutputViewingTime() const;

    // TODO Take dt as a parameter here.
    void update(hour_t dt);

    MaximumValues getXYMinsAndMaxes() const;

private:
    ParticleList physicalObjects;
    hour_t timeElapsed;
    int stepsElapsed;

	float minX, maxX, minY, maxY;

    float octreeTheta;

    unique_ptr<Quadrant>  quadrant;

    CollisionType collisionType;

    Interactions interactions;

	void updateTimeElapsed(hour_t dt);

	void updateMinsAndMaxes();
    void resetXYMinsAndMaxes();
    void updateXYMinsAndMaxes(PhysicalVector curPos);

	void refreshQuadrant(ParticleList &physicalObjects);

	void calcForcesAll(ParticleList &physicalObjects, hour_t dt);

} ;

#endif
