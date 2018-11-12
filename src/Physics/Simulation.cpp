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
    VecStruct pos(0, 0, 0, true);

    float side = 10e5; //Formation Value
    quadrant = std::make_shared<Quadrant>( 1, pos, side ) ;

    for ( const auto & curShape : physicalObjects.getShapes() ) {
        quadrant->insertShape( curShape );
    }
}

void Simulation::updateXYMinsAndMaxes(sgVec4 curPos) {
    // TODO Use min/max functions instead of comparisons here.
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
    for ( const auto & curShape : this->physicalObjects.getShapes() ) {
        VecStruct curPos(curShape->getPos());
        updateXYMinsAndMaxes(curPos.vec);
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

void Simulation::removePhysicalObjects(ShapeList shapesToRemove) {
    physicalObjects.remove(shapesToRemove);
}

void Simulation::addPhysicalObjectToList(shapePointer_t newShape) { physicalObjects.addShapeToList(std::move(newShape)); }

ShapeList Simulation::getPhysicalObjects() { return physicalObjects; }

void Simulation::updateTimeElapsed() { timeElapsed += DT; }

double Simulation::getTimeElapsed() { return timeElapsed; }

float Simulation::getDT() const { return DT; }

void Simulation::setDT(float newDT) { DT = newDT; }

QuadrantPointer_t Simulation::getQuadrant() { return quadrant; }

VecStruct calcForceGravNew(MyShape &object1, MyShape &object2, float dt)
{
    VecStruct sepVec(object1.getVectorToObject(object2));

    SGfloat rSquared = std::max(sgLengthSquaredVec4(sepVec.vec), .00001f);

    float G = 6.67384e-11;
    float forceMagnitude = (G * object1.getMass() * object2.getMass()) / rSquared;

    return sepVec
            .unit()
            .scaledBy(forceMagnitude)
            .scaledBy(dt);
}

void elasticCollision(MyShape &object1, MyShape &object2, float dt) {
    VecStruct sepVecUnit = object1.getVectorToObject(object2).unit();
    VecStruct n(sepVecUnit);

    float c = n.scalarProduct4(object1.getVelocity().minus(object2.getVelocity()));

    auto combinedMass = object2.getMass() + object1.getMass();
    SGfloat multiplierA = -2 * ( object2.getMass() * c ) / combinedMass;
    VecStruct aVec = n.scaledBy(multiplierA);

    object1.adjustVelocity(aVec);

    SGfloat multiplierB = 2 * ( object1.getMass() * c ) / combinedMass;
    VecStruct bVec = n.scaledBy(multiplierB);

    object2.adjustVelocity(bVec);

    // TODO Figure out how necessary these lines are.
    while ( object1.isTouching(object2) ) {
        object1.update(dt/30);
        object2.update(dt/30);
    }
}

// I'm working on this method because I think there are some basic
// structural changes that I can make so that it will be more legible and
// more suitable for later parallelization
// TODO This should return the deleted list, so that it can coexist with the Octree version
void Simulation::calcForcesAll_LessNaive()
{
    VecStruct gravField(0, 0, 0, false);

    vectorT physicalObjects = this->physicalObjects.getShapes();
    ShapeList deleteList;

    if (this->constGravField ) {
        this->getConstGravFieldVal(gravField.vec);
        sgScaleVec4(gravField.vec, 1/DT);
    }

    if (!physicalObjects.empty()) {
        for (size_t i = 0; i < physicalObjects.size()-1; i++) {
            MyShape & object1 = *physicalObjects.at(i);

            object1.adjustMomentum(gravField);

            for (size_t j = i + 1; j < physicalObjects.size(); j++) {
                MyShape & object2 = *physicalObjects.at(j);


                if (this->gravBetweenObjects ) {
                    VecStruct gravVec = calcForceGravNew(object1, object2, DT );

                    object1.adjustMomentum(gravVec);
                    sgNegateVec4(gravVec.vec);
                    object2.adjustMomentum(gravVec);
                }

                SGfloat distance = object1.getDistanceToObject( object2 );
                SGfloat minSep = object1.getRadius() + object2.getRadius();

                if (distance < minSep) {
                    if (this->collisionType == CollisionType::ELASTIC) {
                        elasticCollision( object1, object2, DT );
                    }
                    else if (this->collisionType == CollisionType::INELASTIC){
                        object1.mergeWith( object2 );
                        deleteList.addShapeToList(physicalObjects.at(j));
                    }
                }
            }
        }

        // Add unary forces to last object
        auto object1 = physicalObjects.at(physicalObjects.size()-1);
        object1->adjustMomentum(gravField);

        this->removePhysicalObjects(deleteList);
    }

}



PairCollection Simulation::calculateForceOnExternalNode(const shapePointer_t &curObject, const QuadrantPointer_t &curQuadrant, float dt) {
    //1. a.
    PairCollection deleteList;
    shapePointer_t shapeInQuadrant = curQuadrant->getShapeInQuadrant();

    //b.
    if ( shapeInQuadrant != nullptr && curObject != shapeInQuadrant ) {
        //c.
        if ( curObject->isTouching( *shapeInQuadrant ) ) {
            TouchingPair pair(curObject, shapeInQuadrant);
            deleteList.insertIfUnique(pair);
        } else {
            VecStruct gravVec = calcForceGravNew( *curObject, *shapeInQuadrant, dt);
            curObject->adjustMomentum(gravVec);
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
    if ( curQuadrant->isExternal() ) {
        return calculateForceOnExternalNode(curObject, curQuadrant, dt);
    }
    else {
        PairCollection deleteList;
        SGfloat distance = curObject->getDistanceToObject( *curQuadrant );
        SGfloat theta = 0.5;
        //2.
        //a.
        if ( curQuadrant->getWidth() / distance < theta ) {
            VecStruct gravVec = calcForceGravNew( *curObject, *curQuadrant, dt);
            //b.
            curObject->adjustMomentum(gravVec);
        } else { //3.
            QuadrantPointer_t targetQuadrant;
            // TODO This should *really* be captured inside the Quadrant class. WTF should Simulations know about these shitty indexes?
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

            for ( const auto & curShape : this->physicalObjects.getShapes() ) {
                deleteList.insertUniqueElements(calcForceOnObject_Octree(curShape, this->getQuadrant(), this->DT, 0));
            }

            this->removePhysicalObjects(deleteList.doomed().getShapes() );
            deleteList.mergePairs();
            break;
    }
}

size_t Simulation::getSize() {
    return this->physicalObjects.getShapes().size();
}

double Simulation::getMass() {
    double mass = 0;
    for (const auto & shape : this->physicalObjects.getShapes()) {
        mass += shape->getMass();
    }
    return mass;
}

