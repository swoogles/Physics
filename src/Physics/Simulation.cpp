/*
 * Simulation.cpp
 *
 *  Created on: Jul 22, 2011
 *      Author: bfrasure
 */

#include "Simulation.h"

const char Simulation::FORCE_CALC_METHOD_OCTREE_STRING[] = "octree";
const char Simulation::FORCE_CALC_METHOD_NAIVE_STRING[] = "naive";

Simulation::Simulation()
            :forceCalcMethod(FORCE_CALC_METHOD_OCTREE),
            curStep(0),
            paused(true),
            minX(FLT_MAX),
            maxX(FLT_MIN),
            minY(FLT_MAX),
            maxY(FLT_MIN)
{
}

void Simulation::refreshQuadrant()
{
  int magnitude;
  magnitude = maxX;
  if ( minX < 0 )
  {
    if ( -minX > magnitude )
    {
      magnitude = -minX;
    }
  }

  if ( maxY > magnitude )
  {
    magnitude = maxY;
    if ( minY < 0 )
    {
      if ( -minY > magnitude )
      {
        magnitude = -minY;
      }
    }
  }

  sgVec4 pos = {0,0,0,1};
  float side = magnitude * 10; //Formation Value
  sgVec3 dimensions = { side, side, side };
  quadrant = boost::make_shared<Quadrant>( 4, 1, boost::ref(pos), boost::ref(dimensions) ) ;

  foreach_ ( shape_pointer curShape, physicalObjects.getShapes() )
  {
    quadrant->insertShape( curShape );
  }
}

void Simulation::setForceCalcMethodByString( const string& forceCalcMethod )
{
  if ( forceCalcMethod.compare( Simulation::FORCE_CALC_METHOD_OCTREE_STRING ) )
  {
    setForceCalcMethod( Simulation::FORCE_CALC_METHOD_OCTREE );
  }
  else if ( forceCalcMethod.compare( Simulation::FORCE_CALC_METHOD_NAIVE_STRING ) )
  {
    setForceCalcMethod( Simulation::FORCE_CALC_METHOD_NAIVE );
  }
  else
  {
    cout << "Unrecognized string! forceCalcMethod not set. Using default/current value." << endl;
  }
}

void Simulation::updateXYMinsAndMaxes(sgVec4 curPos) {
	if (curPos[0] < minX)
		minX = curPos[0];
	if (curPos[0] > maxX)
		maxX = curPos[0];
	if (curPos[1] < minY)
		minY = curPos[1];
	if (curPos[1] > maxY)
		maxY = curPos[1];
}

void Simulation::resetXYMinsAndMaxes() {
	minX = FLT_MAX;
	maxX = FLT_MIN;

  // TODO Figure out wtf is going wrong when I enable this line....
	// minY = FLT_MAX;
	maxY = FLT_MIN;
}
