#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <boost/ref.hpp>
#include "../Parallelization/Quadrant.h"
#include "ForceCalculationMethod.h"
#include "CollisionType.h"
#include <ShapeFiles/ParticleList.h>
#include "../MaximumValues.h"
#include <Physics/Interactions.h>
#include "lib/units.h"


typedef particle_t physicalObject_t;

class Simulation {
private:
	ParticleList physicalObjects;
	double timeElapsed;

	float minX, maxX, minY, maxY;

    float octreeTheta;

    QuadrantPointer_t quadrant;

    ForceCalculationMethod forceCalcMethod;
    CollisionType collisionType;

    Interactions interactions;

	void updateTimeElapsed(float dt);

	void updateMinsAndMaxes();
    void resetXYMinsAndMaxes();
    void updateXYMinsAndMaxes(PhysicalVector curPos);

	void removePhysicalObjects( ParticleList shapesToRemove );

    void refreshQuadrant();

	PairCollection calculateForceOnExternalNode(const physicalObject_t &curObject, Quadrant &curQuadrant, float dt);
	PairCollection calcForceOnObject_Octree(physicalObject_t curObject, Quadrant &curQuadrant, float dt,
											int recursionLevel);

	void calcForcesAll_LessNaive(float dt);
	void calcForcesAll(float dt);

public:
    Simulation(ParticleList physicalObjects, CollisionType collisionType,
				   ForceCalculationMethod forceCalculationMethod, float octreeTheta);

	void addPhysicalObjectToList(physicalObject_t newShape);

    double getTimeElapsed();

    // TODO Take dt as a parameter here.
	void update(float dt);

    MaximumValues getXYMinsAndMaxes();

	size_t getSize();
	kilogram_t getMass();

	QuadrantPointer_t getQuadrant();
} ;
typedef shared_ptr<Simulation> SimulationPtr_t;
#endif
