#include <utility>

/*
 * Simulation.cpp
 *
 *  Created on: Jul 22, 2011
 *      Author: bfrasure
 */

#include "Simulation.h"

Simulation::Simulation(float sideLength)
            :curStep(0)
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

Simulation::Simulation(ShapeList physicalObjects, CollisionType collisionType, float dt, bool gravityBetweenObjects)
        :physicalObjects(std::move(physicalObjects))
        ,curStep(0)
        ,DT(dt)
        ,timeElapsed(0)
        ,paused(true)
        ,totalMass(0)
        ,minX(FLT_MAX)
        ,maxX(FLT_MIN)
        ,minY(FLT_MAX)
        ,maxY(FLT_MIN)
        ,constGravField(false)
        ,gravBetweenObjects(gravityBetweenObjects)
{
    // TODO Get rid of bools completely
    allElastic = collisionType == CollisionType::ELASTIC;
    allInelastic = collisionType == CollisionType::INELASTIC;
    this->updateMinsAndMaxes();
    this->refreshQuadrant();
}

void Simulation::refreshQuadrant()
{
  sgVec4 pos = {0,0,0,1};
  float side = 10e5; //Formation Value
  sgVec3 dimensions = { side, side, side };
  // Uh oh.  I think this is it.
  quadrant = std::make_shared<Quadrant>( 1, boost::ref(pos), boost::ref(dimensions) ) ;

  for ( const auto & curShape : physicalObjects.getShapes() ) {
    quadrant->insertShape( curShape );
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
    for ( const auto & curShape : getPhysicalObjects().getShapes() ) {
        vecPtr curPos(curShape->getPosNew());
        updateXYMinsAndMaxes(curPos->vec);
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

