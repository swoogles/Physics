#include <utility>

#include "Simulation.h"

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
    this->collisionType = collisionType;
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
    if (curPos[0] < minX) {
        minX = curPos[0];
    }
    if (curPos[0] > maxX) {
        maxX = curPos[0];
    }
    if (curPos[1] < minY) {
        minY = curPos[1];
    }
    if (curPos[1] > maxY) {
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

void Simulation::getConstGravFieldVal(sgVec4 retGravField) {
	sgCopyVec4(retGravField, gravField);
}

bool Simulation::isGravBetweenObjects() {
	return gravBetweenObjects;
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

CollisionType Simulation::getCollisionType() const {
    return collisionType;
}

ForceCalculationMethod Simulation::getForceCalcMethod() { return forceCalcMethod; }

void Simulation::setForceCalcMethod(const ForceCalculationMethod forceCalcMethod) { this->forceCalcMethod = forceCalcMethod; }

void Simulation::removePhysicalObject(shapePointer_t newShape) {
    // TODO reinstate?
    physicalObjects.removeShapeFromList(std::move(newShape));
//		physicalObjects.removeShapeFromList(newShape);
}

void Simulation::addPhysicalObjectToList(shapePointer_t newShape) { physicalObjects.addShapeToList(std::move(newShape)); }

ShapeList Simulation::getPhysicalObjects() { return physicalObjects; }

void Simulation::updateTimeElapsed() { timeElapsed += DT; }

double Simulation::getTimeElapsed() { return timeElapsed; }

float Simulation::getDT() const { return DT; }

void Simulation::setDT(float newDT) { DT = newDT; }

Simulation::Simulation(Simulation && originalSimulation) {

}

void Simulation::incCurStep() { curStep+= 1; }

void Simulation::Pause() { paused = true; }

void Simulation::unPause() { paused = false; }

bool Simulation::isPaused() { return paused; }

QuadrantPointer_t Simulation::getQuadrant() { return quadrant; }

