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

  public:
    Simulation(ShapeList physicalObjects, CollisionType collisionType, float dt, bool gravityBetweenObjects);;
    inline void incCurStep() { curStep+= 1; };

    inline ForceCalculationMethod getForceCalcMethod() { return forceCalcMethod; };
    inline void setForceCalcMethod( const ForceCalculationMethod forceCalcMethod ) { this->forceCalcMethod = forceCalcMethod; };

    inline ShapeList getPhysicalObjects() { return physicalObjects; };
    inline void addPhysicalObjectToList( shapePointer_t newShape ) { physicalObjects.addShapeToList(std::move(newShape)); };
    inline void removePhysicalObject( shapePointer_t newShape ) {
        // TODO reinstate?
        physicalObjects.removeShapeFromList(std::move(newShape));
//		physicalObjects.removeShapeFromList(newShape);
    };

    inline void setDT(float newDT) { DT = newDT; };
    inline float getDT() const { return DT; };

    void updateTimeElapsed() { timeElapsed += DT; };
    double getTimeElapsed() { return timeElapsed; };

    void updateMinsAndMaxes();
    void update();

    void Pause() { paused = true; };
    void unPause() { paused = false; };
    bool isPaused() { return paused; };

    void resetXYMinsAndMaxes();
    void updateXYMinsAndMaxes(sgVec4 curPos);

    void refreshQuadrant();
    inline QuadrantPointer_t getQuadrant() { return quadrant; }
    void getXYMinsAndMaxes( float & minX, float & maxX, float & minY, float & maxY );

	//! Returns true if a ubiquitous force from gravity affects the entire system
	bool isConstGravField();

    //! Return gravity field vector in retGravField
	void getConstGravFieldVal(sgVec4 retGravField);

	//! Returns true if simulation is calculating gravity between objects
	bool isGravBetweenObjects();

} ;
typedef shared_ptr<Simulation> SimulationPtr_t;
#endif
