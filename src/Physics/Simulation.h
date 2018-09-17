#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <boost/ref.hpp>
#include <plib/sg.h>
#include "../ShapeFiles/ShapeList.h"
#include "../Parallelization/Quadrant.h"

using boost::numeric::ublas::compressed_vector;
using boost::shared_ptr;
using boost::make_shared;

typedef boost::scoped_ptr<VecStruct> vecPtr;

// TODO This is a great place to use some abstractions. It needs to know a *lot* about its inner details.
class Simulation
{
  private:
    typedef boost::shared_ptr<MyShape> shape_pointer;
    int forceCalcMethod;
    ShapeList physicalObjects;

    int curStep;
    float DT;
    double timeElapsed;
    bool paused;
    float totalMass;

    float minX, maxX, minY, maxY;

    bool allElastic;
    bool allInelastic;

    bool constGravField;
    sgVec4 gravField;
    bool gravBetweenObjects;

    QuadrantPointer_t quadrant;

  public:
	// TODO make private
    ShapeList shapes;
    static const char FORCE_CALC_METHOD_NAIVE_STRING[];
    static const char FORCE_CALC_METHOD_OCTREE_STRING[];
    static const int FORCE_CALC_METHOD_NAIVE = 0;
    static const int FORCE_CALC_METHOD_OCTREE = 1;

    Simulation();

    inline int getCurStep() { return curStep; };
    inline void incCurStep() { curStep+= 1; };

    inline int getForceCalcMethod() { return forceCalcMethod; };
    inline void setForceCalcMethod( const int forceCalcMethod ) { this->forceCalcMethod = forceCalcMethod; };
    void setForceCalcMethodByString( const string& forceCalcMethod );

    inline ShapeList getPhysicalObjects() { return physicalObjects; };
    inline void setPhysicalObjects( ShapeList physicalObjects ) { this->physicalObjects = physicalObjects; };
    inline void addPhysicalObjectToList( shape_pointer newShape ) { physicalObjects.addShapeToList( newShape ); };

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
	//! Set to true to enact a constant gravity vector
	void setConstGravField(bool useGravField);
	//! Set gravity field vector to newGravField
	void setConstGravFieldVal(sgVec4 newGravField);
	//! Return gravity field vector in retGravField
	void getConstGravFieldVal(sgVec4 retGravField);

	//! Returns true if simulation is calculating gravity between objects
	bool isGravBetweenObjects();
	//! Set to true to calculate gravity between objects
	void setGravBetweenObjects(bool);

	// TODO Replaced with "getElasticity" method that returns an enum of ELASTIC, INELASTIC, or MIXED
	//! Returns true if every collision is treated as completely elastic
	bool isAllElastic();
	//! Returns true if every collision is treated as competely inelastic
	bool isAllInelastic();

	//! Makes simulation treat all collisions as completely elastic
	void makeAllElastic();
	//! Makes simulation treat all collisions as completely inelastic
	void makeAllInelastic();

} ;
typedef boost::shared_ptr<Simulation> SimulationPtr_t;
#endif
