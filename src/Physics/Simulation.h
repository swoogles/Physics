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

// TODO This is a great place to use some abstractions. It needs to know a *lot* about its inner details.
class Simulation
{
  private:
    ForceCalculationMethod forceCalcMethod;
    ShapeList physicalObjects;
    CollisionType collisionType;
public:
	CollisionType getCollisionType() const;

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

	PairCollection calculateForceOnExternalNode(const shapePointer_t &curObject, const QuadrantPointer_t &curQuadrant, float dt);
	PairCollection calcForceOnObject_Octree(shapePointer_t curObject, QuadrantPointer_t curQuadrant, float dt, int recursionLevel);

	void calcForcesAll_LessNaive();
	void calcForcesAll();

  public:
    Simulation(ShapeList physicalObjects, CollisionType collisionType, float dt, bool gravityBetweenObjects);;
    Simulation(Simulation && originalSimulation);
    void incCurStep();

    ForceCalculationMethod getForceCalcMethod();
    void setForceCalcMethod( const ForceCalculationMethod forceCalcMethod );

    ShapeList getPhysicalObjects();
    void addPhysicalObjectToList( shapePointer_t newShape );
    void removePhysicalObject( shapePointer_t newShape );

    void setDT(float newDT);
    float getDT() const;

    void updateTimeElapsed();
    double getTimeElapsed();

    void updateMinsAndMaxes();
    void update();

    void Pause();
    void unPause();
    bool isPaused();

    void resetXYMinsAndMaxes();
    void updateXYMinsAndMaxes(sgVec4 curPos);

    void refreshQuadrant();
    QuadrantPointer_t getQuadrant();
    void getXYMinsAndMaxes( float & minX, float & maxX, float & minY, float & maxY );

	//! Returns true if a ubiquitous force from gravity affects the entire system
	bool isConstGravField();

    //! Return gravity field vector in retGravField
	void getConstGravFieldVal(sgVec4 retGravField);

	//! Returns true if simulation is calculating gravity between objects
	bool isGravBetweenObjects();

	typedef shared_ptr<Simulation> SimulationPtr_t;

	static float G;
} ;
typedef shared_ptr<Simulation> SimulationPtr_t;
#endif
