#include <utility>

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <boost/ref.hpp>
#include "../Parallelization/Quadrant.h"
#include "ForceCalculationMethod.h"
#include "CollisionType.h"
#include <stdexcept>
#include <ShapeFiles/ParticleList.h>
#include "../MaximumValues.h"
#include "lib/units.h"

using namespace units::energy;
using namespace units::force;

typedef particle_t physicalObject_t;
//using namespace units::in;

class Simulation {
private:
	ParticleList physicalObjects;
	double timeElapsed;

	float minX, maxX, minY, maxY;

    bool constGravField;
    PhysicalVector gravField;
    bool gravBetweenObjects;

    float octreeTheta;

    QuadrantPointer_t quadrant;

    ForceCalculationMethod forceCalcMethod;
    CollisionType collisionType;

	void updateTimeElapsed(float dt);

	void updateMinsAndMaxes();
    void resetXYMinsAndMaxes();
    void updateXYMinsAndMaxes(PhysicalVector curPos);

	void removePhysicalObjects( ParticleList shapesToRemove );

    //! Return gravity field vector in retGravField
	PhysicalVector getConstGravFieldVal();

	void refreshQuadrant();

	PairCollection calculateForceOnExternalNode(const physicalObject_t &curObject, Quadrant &curQuadrant, float dt);
	PairCollection calcForceOnObject_Octree(physicalObject_t curObject, Quadrant &curQuadrant, float dt,
											int recursionLevel);

	void calcForcesAll_LessNaive(float dt);
	void calcForcesAll(float dt);

	ParticleList crackPhysicalObject(Particle &shape);

  public:
    Simulation(ParticleList physicalObjects, CollisionType collisionType, bool gravityBetweenObjects,
                   ForceCalculationMethod forceCalculationMethod, float octreeTheta);
	Simulation(Simulation && originalSimulation, ParticleList newObjects);

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
