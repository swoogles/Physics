#include <utility>

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <boost/ref.hpp>
#include "../ShapeFiles/ShapeList.h"
#include "../Parallelization/Quadrant.h"
#include "ForceCalculationMethod.h"
#include "CollisionType.h"
#include <stdexcept>
#include <ShapeFiles/ParticleList.h>
#include "../MaximumValues.h"
#include "lib/units.h"

using namespace units::energy;
using namespace units::force;
//using namespace units::in;

class Simulation {
private:
	ParticleList physicalObjects;
	double timeElapsed;

	float minX, maxX, minY, maxY;

    bool constGravField;
    VecStruct gravField;
    bool gravBetweenObjects;

    float octreeTheta;

    QuadrantPointer_t quadrant;

    ForceCalculationMethod forceCalcMethod;
    CollisionType collisionType;

	void updateTimeElapsed(float dt);

	void updateMinsAndMaxes();
    void resetXYMinsAndMaxes();
    void updateXYMinsAndMaxes(VecStruct curPos);

	void removePhysicalObjects( ParticleList shapesToRemove );

    //! Return gravity field vector in retGravField
	VecStruct getConstGravFieldVal();

	void refreshQuadrant();

	PairCollection calculateForceOnExternalNode(const shared_ptr<Circle> &curObject, Quadrant &curQuadrant, float dt);
	PairCollection calcForceOnObject_Octree(shared_ptr<Circle> curObject, Quadrant &curQuadrant, float dt,
											int recursionLevel);

	void calcForcesAll_LessNaive(float dt);
	void calcForcesAll(float dt);

	ParticleList crackPhysicalObject(MyShape & shape);

  public:
    Simulation(ParticleList physicalObjects, CollisionType collisionType, bool gravityBetweenObjects,
                   ForceCalculationMethod forceCalculationMethod, float octreeTheta);
	Simulation(Simulation && originalSimulation, ParticleList newObjects);

    void addPhysicalObjectToList(shared_ptr<Circle> newShape);

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
