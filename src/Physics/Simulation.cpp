#include <utility>

#include "Simulation.h"

Simulation::Simulation(ShapeList physicalObjects, CollisionType collisionType, float dt, bool gravityBetweenObjects,
                       ForceCalculationMethod forceCalculationMethod)
        :physicalObjects(std::move(physicalObjects))
        ,DT(dt)
        ,timeElapsed(0)
        ,minX(FLT_MAX)
        ,maxX(FLT_MIN)
        ,minY(FLT_MAX)
        ,maxY(FLT_MIN)
        ,constGravField(false)
        ,gravBetweenObjects(gravityBetweenObjects)
        ,forceCalcMethod(forceCalculationMethod)
        ,collisionType(collisionType)
{
    this->updateMinsAndMaxes();
    this->refreshQuadrant();
}

Simulation::Simulation(Simulation &&originalSimulation, ShapeList newObjects)
        :physicalObjects(std::move(originalSimulation.physicalObjects))
        ,DT(originalSimulation.DT)
        ,timeElapsed(originalSimulation.timeElapsed)
        ,minX(FLT_MAX)
        ,maxX(FLT_MIN)
        ,minY(FLT_MAX)
        ,maxY(FLT_MIN)
        ,constGravField(originalSimulation.constGravField)
        ,gravBetweenObjects(originalSimulation.gravBetweenObjects)
        ,forceCalcMethod(originalSimulation.forceCalcMethod)
{
    physicalObjects.addList(std::move(newObjects));
}

void Simulation::refreshQuadrant()
{
    vecPtr pos = make_unique<VecStruct>();
    // TODO encapsulate in constructor
    pos->vec[0] = 0;
    pos->vec[1] = 0;
    pos->vec[2] = 0;
    pos->vec[3] = 1;

  float side = 10e5; //Formation Value
//  sgVec3 dimensions = { side, side, side };
  vecPtr dimensions = make_unique<VecStruct>();
  dimensions->vec[0] = side;
  dimensions->vec[1] = side;
  dimensions->vec[2] = side;
  // Uh oh.  I think this is it.
  quadrant = std::make_shared<Quadrant>( 1, std::move(pos), std::move(dimensions) ) ;

  for ( const auto & curShape : physicalObjects.getShapes() ) {
    quadrant->insertShape( curShape );
  }
}

void Simulation::updateXYMinsAndMaxes(sgVec4 curPos) {
    resetXYMinsAndMaxes();
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

void Simulation::getConstGravFieldVal(sgVec4 retGravField) {
	sgCopyVec4(retGravField, gravField);
}

void Simulation::updateMinsAndMaxes() {
    for ( const auto & curShape : getPhysicalObjects().getShapes() ) {
        vecPtr curPos(curShape->getPosNew());
        updateXYMinsAndMaxes(curPos->vec);
    }
}

void Simulation::update()
{
  calcForcesAll();
  physicalObjects.update(DT);
  updateTimeElapsed();

  // TODO This causes another full iteration of all shapes. If it's going to happen, it should be done during one of the earlier iterations.
  updateMinsAndMaxes();
  // This is the first "n" part in "n log(n)"
  refreshQuadrant();
}

void Simulation::removePhysicalObject(shapePointer_t newShape) {
    physicalObjects.removeShapeFromList(std::move(newShape));
}

void Simulation::addPhysicalObjectToList(shapePointer_t newShape) { physicalObjects.addShapeToList(std::move(newShape)); }

ShapeList Simulation::getPhysicalObjects() { return physicalObjects; }

void Simulation::updateTimeElapsed() { timeElapsed += DT; }

double Simulation::getTimeElapsed() { return timeElapsed; }

float Simulation::getDT() const { return DT; }

void Simulation::setDT(float newDT) { DT = newDT; }

QuadrantPointer_t Simulation::getQuadrant() { return quadrant; }

vecPtr calcForceGravNew( shapePointer_t object1, shapePointer_t object2, float dt )
{
    vecPtr gravVec = make_unique<VecStruct>();

    sgVec4 unitVec{};

    vecPtr sepVec(object1->getVectorToObject(object2.get()));

    SGfloat rSquared = sgLengthSquaredVec4(sepVec->vec);

    if (rSquared < .00001) {
        rSquared = .00001;
    }
    float G = 6.67384e-11;
    float forceMagnitude = (G * object1->getMass() * object2->getMass()) / rSquared;

    sgNormaliseVec4(unitVec, sepVec->vec);

    sgScaleVec4(gravVec->vec, unitVec, forceMagnitude);
    sgScaleVec4(gravVec->vec, dt);

    return  gravVec;
}

void elasticCollision( shapePointer_t object1, shapePointer_t object2, float dt) {
    sgVec4 sepVecUnit;

    sgVec4 tempVec, n, vdiff;

    SGfloat multiplier;

    vecPtr sepVec(object1->getVectorToObject(object2.get()));
    sgNormaliseVec4(sepVecUnit, sepVec->vec);

    vecPtr aVel(object1->getVelocity());
    vecPtr bVel(object2->getVelocity());

    sgCopyVec4(n, sepVecUnit);
    sgSubVec4(vdiff, aVel->vec, bVel->vec);

    float c = sgScalarProductVec4(n, vdiff);

    multiplier = -2 * ( object2->getMass() * c ) / (object2->getMass() + object1->getMass());
    sgScaleVec4(tempVec, n, multiplier);

    object1->adjustVelocity(tempVec);

    multiplier = 2 * ( object1->getMass() * c ) / (object2->getMass() + object1->getMass());
    sgScaleVec4(tempVec, n, multiplier);

    object2->adjustVelocity(tempVec);

    // TODO Figure out how necessary these lines are.
    while ( object1->isTouching(object2) ) {
        object1->update(dt/30);
        object2->update(dt/30);
    }
}

// I'm working on this method because I think there are some basic
// structural changes that I can make so that it will be more legible and
// more suitable for later parallelization
// TODO This should return the deleted list, so that it can coexist with the Octree version
void Simulation::calcForcesAll_LessNaive()
{
    shapePointer_t object1, object2;
    SGfloat distance, minSep;

    sgVec4 gravField = {0, 0, 0, 0};

    ShapeList shapeList = this->getPhysicalObjects() ;
    vectorT physicalObjects = shapeList.getShapes();
    ShapeList deleteList;

    if (this->constGravField ) {
        this->getConstGravFieldVal(gravField);
        sgScaleVec4(gravField, 1/DT);
    }

    if (!physicalObjects.empty()) {
        for (size_t i = 0; i < physicalObjects.size()-1; i++) {
            object1 = physicalObjects.at(i);

            object1->adjustMomentum(gravField);

            for (size_t j = i + 1; j < physicalObjects.size(); j++) {
                object2 = physicalObjects.at(j);


                if (this->gravBetweenObjects ) {
                    vecPtr gravVec = calcForceGravNew(object1, object2, DT );

                    object1->adjustMomentum(gravVec->vec);
                    sgNegateVec4(gravVec->vec);
                    object2->adjustMomentum(gravVec->vec);
                }

                distance = object1->getDistanceToObject( object2 );
                minSep = object1->getRadius() + object2->getRadius();

                if (distance < minSep) {
                    if (this->collisionType == CollisionType::ELASTIC) {
                        elasticCollision( object1, object2, DT );
                    }
                    else if (this->collisionType == CollisionType::INELASTIC){
                        object1->mergeWith( *object2 );
                        deleteList.addShapeToList(object2);
                    }
                    else {
                        // TODO throw in some way.
                    }

                }

            }
        }

        // Add unary forces to last object
        object1 = physicalObjects.at(physicalObjects.size()-1);
        object1->adjustMomentum(gravField);

        for ( const auto & curShape : deleteList.getShapes() ) {
            this->removePhysicalObject(curShape);
        }

    }

}



PairCollection Simulation::calculateForceOnExternalNode(const shapePointer_t &curObject, const QuadrantPointer_t &curQuadrant, float dt) {
    //1.
    //a.
    PairCollection deleteList;
    shapePointer_t shapeInQuadrant = curQuadrant->getShapeInQuadrant();

    //b.
    if ( shapeInQuadrant != nullptr && curObject != shapeInQuadrant ) {
        //c.
        if ( curObject->isTouching( shapeInQuadrant ) ) {
            TouchingPair pair(curObject, shapeInQuadrant);
            deleteList.insertIfUnique(pair);
        } else {
            vecPtr gravVec = calcForceGravNew( curObject, shapeInQuadrant, dt);
            curObject->adjustMomentum(gravVec->vec);
        }
    }

    return deleteList;
}
//1.
//a. If the current node is an external node
//b. (and it is not body b),
//c. calculate the force exerted by the current node on b
//d. add this amount to b’s net force.
//2.
//a. Otherwise, calculate the ratio s/d. If s/d < θ, treat this internal node as a single body, and calculate the force it exerts on body b
//b. add this amount to b’s net force.
//3. Otherwise, run the procedure recursively on each of the current node’s children.


// TODO Maybe if I add *pairs* of items to deleteList, I can normalize that and not worry about deleting both sides of a collision.
PairCollection Simulation::calcForceOnObject_Octree(shapePointer_t curObject, QuadrantPointer_t curQuadrant, float dt, int recursionLevel)
{

    if (recursionLevel > 100) {
        exit(1);
    }

    if ( curQuadrant->isExternal() ) {
        return calculateForceOnExternalNode(curObject, curQuadrant, dt);
    }
    else {
        PairCollection deleteList;
        SGfloat distance = curObject->getDistanceToObject( curQuadrant );
        SGfloat theta = 0.5;
        //2.
        //a.
        if ( curQuadrant->getWidth() / distance < theta ) {
            vecPtr gravVec = calcForceGravNew( curObject, curQuadrant, dt);
            //b.
            curObject->adjustMomentum(gravVec->vec);
        } else { //3.
            QuadrantPointer_t targetQuadrant;
            for ( int x = 0; x < 2; x++ ) {
                for ( int y = 0; y < 2; y++ ) {
                    for ( int z = 0; z < 2; z++ ) {
                        targetQuadrant = curQuadrant->getQuadrantFromCell( x, y, z );
                        if ( targetQuadrant != nullptr ) {
                            deleteList.insertUniqueElements(calcForceOnObject_Octree(curObject, targetQuadrant, dt, recursionLevel + 1)) ;
                        }
                    }
                }
            }

        }
        return deleteList;
    }


}

void Simulation::calcForcesAll() {
    switch(this->forceCalcMethod) {
        case ForceCalculationMethod::NAIVE:
            calcForcesAll_LessNaive();
            break;
        case ForceCalculationMethod ::OCTREE:
            PairCollection deleteList;

            for ( const auto & curShape : this->getPhysicalObjects().getShapes() ) {
                deleteList.insertUniqueElements(calcForceOnObject_Octree(curShape, this->getQuadrant(), this->DT, 0));
            }

            for ( const auto & curShape : deleteList.doomed().getShapes() ) {
                this->removePhysicalObject(curShape);
            }
            deleteList.mergePairs();
            break;

            // TODO curSimulation.quadrant needs to die *before* any of my shapes will actually go away!
    }
}
