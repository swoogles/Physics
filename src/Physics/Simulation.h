#include <utility>

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <boost/ref.hpp>
#include <plib/sg.h>
#include "../ShapeFiles/ShapeList.h"
#include "../Parallelization/Quadrant.h"
#include "ForceCalculationMethod.h"
#include "CollisionType.h"
#include <stdexcept>

class Simulation {
private:
	int curStep;
    float DT;
    double timeElapsed;
    bool paused;
    float totalMass;

    float minX, maxX, minY, maxY;

    bool constGravField;
    sgVec4 gravField;
    bool gravBetweenObjects;

    QuadrantPointer_t quadrant;

    ForceCalculationMethod forceCalcMethod;
    ShapeList physicalObjects;
    CollisionType collisionType;

	void incCurStep();
	void updateTimeElapsed();

	void updateMinsAndMaxes();
    void resetXYMinsAndMaxes();
    void updateXYMinsAndMaxes(sgVec4 curPos);

	void removePhysicalObject( shapePointer_t newShape );

    //! Return gravity field vector in retGravField
    void getConstGravFieldVal(sgVec4 retGravField);

	void refreshQuadrant();
	QuadrantPointer_t getQuadrant();

	PairCollection calculateForceOnExternalNode(const shapePointer_t &curObject, const QuadrantPointer_t &curQuadrant, float dt);
	PairCollection calcForceOnObject_Octree(shapePointer_t curObject, QuadrantPointer_t curQuadrant, float dt, int recursionLevel);

	void calcForcesAll_LessNaive();
	void calcForcesAll();

  public:
    Simulation(ShapeList physicalObjects, CollisionType collisionType, float dt, bool gravityBetweenObjects,
				   ForceCalculationMethod forceCalculationMethod);;
	Simulation(Simulation && originalSimulation, ShapeList newObjects);
    Simulation(Simulation && originalSimulation);

    ShapeList getPhysicalObjects();
    void addPhysicalObjectToList( shapePointer_t newShape );

    void setDT(float newDT);
    float getDT() const;

    double getTimeElapsed();

    void update();

    void Pause();
    void unPause();
    bool isPaused();

    void getXYMinsAndMaxes( float & minX, float & maxX, float & minY, float & maxY );

} ;
typedef shared_ptr<Simulation> SimulationPtr_t;
#endif
