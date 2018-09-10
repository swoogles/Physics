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
            :forceCalcMethod(FORCE_CALC_METHOD_OCTREE)
            ,curStep(0)
            ,DT(1)
            ,timeElapsed(0)
            ,paused(true)
            ,totalMass(0)
            ,minX(FLT_MAX)
            ,maxX(FLT_MIN)
            ,minY(FLT_MAX)
            ,maxY(FLT_MIN)
            ,allElastic(false)
            ,allInelastic(true)
            ,constGravField(false)
            ,gravBetweenObjects(true)
{
}

void Simulation::refreshQuadrant()
{
  int magnitude;
  magnitude = maxX;
  if ( -minX > magnitude )
  {
    magnitude = -minX;
  }

  if ( maxY > magnitude )
  {
    magnitude = maxY;
    if ( -minY > magnitude )
    {
      magnitude = -minY;
    }
  }

  sgVec4 pos = {0,0,0,1};
  // float side = quadrant->getWidth(); //Formation Value
  float side = 10e5; //Formation Value
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
}

void Simulation::updateXYMinsAndMaxes(sgVec4 curPos) {
	if (curPos[0] < minX)
  {
		minX = curPos[0];
  }
	if (curPos[0] > maxX)
  {
		maxX = curPos[0];
  }
	if (curPos[1] < minY)
  {
		minY = curPos[1];
  }
	if (curPos[1] > maxY)
  {
		maxY = curPos[1];
  }
}

void Simulation::getXYMinsAndMaxes( float & minX, float & maxX, float & minY, float & maxY ) {
  minX = this->minX;
  maxX = this->maxX;
  minY = this->minY;
  maxY = this->maxY;
}


void Simulation::resetXYMinsAndMaxes() {
	minX = FLT_MAX;
	maxX = FLT_MIN;

	minY = FLT_MAX;
	maxY = FLT_MIN;
}

bool Simulation::isConstGravField() {
	return constGravField;
}

void Simulation::setConstGravField(bool useGravField) {
	constGravField = useGravField;
}

void Simulation::setConstGravFieldVal(sgVec4 newGravField) {
	sgCopyVec4(gravField, newGravField);
}

void Simulation::getConstGravFieldVal(sgVec4 retGravField) {
	sgCopyVec4(retGravField, gravField);
}

bool Simulation::isGravBetweenObjects() {
	return gravBetweenObjects;
}

void Simulation::setGravBetweenObjects(bool newVal) {
	gravBetweenObjects = newVal;
}

bool Simulation::isAllElastic() {
	return allElastic;
}

bool Simulation::isAllInelastic() {
	return allInelastic;
}

void Simulation::makeAllElastic() {
	allElastic = true;
	allInelastic = false;
}

void Simulation::makeAllInelastic() {
	allInelastic = true;
	allElastic = false;
}

void Simulation::updateMinsAndMaxes() {
    sgVec4 curPos;
    foreach_ ( shape_pointer curShape, getPhysicalObjects().getShapes() )
    {
        curShape->getPos(curPos);
        updateXYMinsAndMaxes(curPos);
    }
}

void Simulation::update()
{
  physicalObjects.update(DT);
  updateTimeElapsed();
  incCurStep();

  updateMinsAndMaxes();
  refreshQuadrant();
}
