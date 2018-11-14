#include <utility>

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <boost/ref.hpp>
#include "../ShapeFiles/ShapeList.h"
#include "../Parallelization/Quadrant.h"
#include "ForceCalculationMethod.h"
#include "CollisionType.h"
#include <stdexcept>

class Simulation {
private:
	ShapeList physicalObjects;
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

	void removePhysicalObjects( ShapeList shapesToRemove );

    //! Return gravity field vector in retGravField
	VecStruct getConstGravFieldVal();

	void refreshQuadrant();

	PairCollection calculateForceOnExternalNode(const shapePointer_t &curObject, const QuadrantPointer_t &curQuadrant, float dt);
	PairCollection calcForceOnObject_Octree(shapePointer_t curObject, QuadrantPointer_t curQuadrant, float dt, int recursionLevel);

	void calcForcesAll_LessNaive(float dt);
	void calcForcesAll(float dt);

  public:
    Simulation(ShapeList physicalObjects, CollisionType collisionType, bool gravityBetweenObjects,
                   ForceCalculationMethod forceCalculationMethod, float octreeTheta);
	Simulation(Simulation && originalSimulation, ShapeList newObjects);

    void addPhysicalObjectToList( shapePointer_t newShape );

    double getTimeElapsed();

    // TODO Take dt as a parameter here.
	void update(float dt);

	void getXYMinsAndMaxes( float & minX, float & maxX, float & minY, float & maxY );

	size_t getSize();
	double getMass();

	QuadrantPointer_t getQuadrant();
} ;
typedef shared_ptr<Simulation> SimulationPtr_t;
#endif
