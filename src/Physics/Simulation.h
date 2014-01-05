/*
 * Simulations.h
 *
 *  Created on: Jul 22, 2011
 *      Author: brasure
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <iostream>
#include <boost/ref.hpp>
#include <plib/sg.h>
#include "../ShapeFiles/ShapeList.h"
#include "../Parallelization/Quadrant.h"

using boost::numeric::ublas::compressed_vector;
using boost::shared_ptr;
using boost::make_shared;

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

    boost::shared_ptr<Quadrant> quadrant;

  public:
    static const char FORCE_CALC_METHOD_NAIVE_STRING[];
    static const char FORCE_CALC_METHOD_OCTREE_STRING[];
    static const int FORCE_CALC_METHOD_NAIVE = 0;
    static const int FORCE_CALC_METHOD_OCTREE = 1;

    Simulation();

    inline int getCurStep() { return curStep; };
    inline void setCurStep( int curStep ) { this->curStep = curStep; };
    inline void incCurStep() { curStep+= 1; };

    inline int getForceCalcMethod() { return forceCalcMethod; };
    inline void setForceCalcMethod( const int forceCalcMethod ) { this->forceCalcMethod = forceCalcMethod; };
    void setForceCalcMethodByString( const string& forceCalcMethod );

    inline ShapeList getPhysicalObjects() { return physicalObjects; };
    inline void setPhysicalObjects( ShapeList physicalObjects ) { this->physicalObjects = physicalObjects; };
    inline void addPhysicalObjectToList( shape_pointer newShape ) { physicalObjects.addShapeToList( newShape ); };

    inline void setDT(float newDT) { DT = newDT; };
    inline float getDT() const { return DT; };

    void setTimeElapsed(double newTimeElapsed) { timeElapsed = newTimeElapsed; };
    void updateTimeElapsed() { timeElapsed += DT; };
    double getTimeElapsed() { return timeElapsed; };

    void Pause() { paused = true; };
    void unPause() { paused = false; };
    bool isPaused() { return paused; };

    void resetXYMinsAndMaxes();
    void updateXYMinsAndMaxes(sgVec4 curPos);

    inline float getMinX() { return minX; }
    inline float getMinY() { return minY; }
    inline float getMaxX() { return maxX; }
    inline float getMaxY() { return maxY; }

    void refreshQuadrant();
    inline boost::shared_ptr<Quadrant> getQuadrant() { return quadrant; }
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

	//! Returns true if every collision is treated as completely elastic
	bool isAllElastic();
	//! Returns true if every collision is treated as competely inelastic
	bool isAllInelastic();
	/*! \brief Returns true if if isAllElastic() and isAllInelastic both return false
	 *
	 *  Indicates if collision action must be determined on a case-by-case basis
	 */
	bool isMixedElasticity();

	//! Makes simulation treat all collisions as completely elastic
	void makeAllElastic();
	//! Makes simulation treat all collisions as completely inelastic
	void makeAllInelastic();
	//! Makes simulation treat collisions based on their characteristics
	void makeMixedElasticity();

	//TODO set up relavent mass functions and color-coding
	/*! \brief Alters the total mass in the system
	 *
	 * Normally used when adding/deleting objects
	 */
	void adjustTotalMass(float dMass);
	//! Returns total amount of mass in current system
	float getTotalMass();
	//! sets total amount of mass in current system
	void setTotalMass(float newMass);

} ;
#endif 
