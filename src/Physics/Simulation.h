/*
 * Simulations.h
 *
 *  Created on: Jul 22, 2011
 *      Author: brasure
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <iostream>
#include <boost/numeric/ublas/vector_sparse.hpp>
#include <boost/ref.hpp>
#include <plib/sg.h>
//#include "../ShapeFiles/MyShape.h"
#include "../ShapeFiles/Circle.h"
#include "../ShapeFiles/Box.h"
#include "../ShapeFiles/ShapeList.h"
// #include "../Parallelization/Quadrant.h"
// #include "Interactions.h"

using namespace boost::numeric::ublas;
using boost::numeric::ublas::compressed_vector;

class Simulation
{
  private:
    typedef boost::shared_ptr<MyShape> shape_pointer;
    // static boost::numeric::ublas::compressed_vector< shape_pointer > physicalObjects;
    int curStep;
    ShapeList physicalObjects;
    int forceCalcMethod;

    float DT;
    double timeElapsed;

    bool paused;

    // float totalMass;

    // bool allElastic;
    // bool allInelastic;

    // bool constGravField;
    bool gravBetweenObjects;

    // sgVec4 gravField;

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


inline bool isGravBetweenObjects() { return gravBetweenObjects; };

inline void setGravBetweenObjects(bool newVal) { gravBetweenObjects = newVal; };

} ;
#endif 
