#include "Simulation.h"
#include "Interactions.h"


Simulation::Simulation(ShapeList physicalObjects, CollisionType collisionType, bool gravityBetweenObjects,
                       ForceCalculationMethod forceCalculationMethod, float octreeTheta)
        :physicalObjects(std::move(physicalObjects))
        ,timeElapsed(0)
        ,minX(FLT_MAX)
        ,maxX(FLT_MIN)
        ,minY(FLT_MAX)
        ,maxY(FLT_MIN)
        ,constGravField(false)
        ,gravBetweenObjects(gravityBetweenObjects)
        ,forceCalcMethod(forceCalculationMethod)
        ,collisionType(collisionType)
        ,octreeTheta(octreeTheta)
{
    this->updateMinsAndMaxes();
    this->refreshQuadrant();
}

Simulation::Simulation(Simulation &&originalSimulation, ShapeList newObjects)
        :physicalObjects(std::move(originalSimulation.physicalObjects))
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

    float side = 10e7; //Formation Value . ACK!!!! How miserably hard-coded!!
    quadrant = std::make_shared<Quadrant>( 1, pos, side ) ;

    for ( const auto & curShape : physicalObjects.getShapes() ) {
        quadrant->insertShape( curShape );
    }
}

void Simulation::updateXYMinsAndMaxes(VecStruct curPos) {
    // TODO Use min/max functions instead of comparisons here.
    resetXYMinsAndMaxes();
    maxX = std::max(maxX, curPos.x());
    minX = std::min(minX, curPos.x());

    maxY = std::max(maxY, curPos.y());
    minY = std::min(minY, curPos.y());
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

VecStruct Simulation::getConstGravFieldVal() {
    return gravField;
}

void Simulation::updateMinsAndMaxes() {
    for ( const auto & curShape : this->physicalObjects.getShapes() ) {
        VecStruct curPos(curShape->getPos());
        updateXYMinsAndMaxes(curPos);
    }
}

void Simulation::update(float dt)
{
    calcForcesAll(dt);
  physicalObjects.update(dt);
    updateTimeElapsed(dt);

  // TODO This causes another full iteration of all shapes. If it's going to happen, it should be done during one of the earlier iterations.
  updateMinsAndMaxes();
  // This is the first "n" part in "n log(n)"
  refreshQuadrant();
}

void Simulation::removePhysicalObjects(ShapeList shapesToRemove) {
    physicalObjects.remove(shapesToRemove);
}

void Simulation::addPhysicalObjectToList(shapePointer_t newShape) { physicalObjects.addShapeToList(std::move(newShape)); }

void Simulation::updateTimeElapsed(float dt) { timeElapsed += dt; }

double Simulation::getTimeElapsed() { return timeElapsed; }

QuadrantPointer_t Simulation::getQuadrant() { return quadrant; }

VecStruct calcForceGravNew(MyShape &object1, MyShape &object2, float dt)
{
    VecStruct sepVec(object1.getVectorToObject(object2));

    float rSquared = std::max(sgLengthSquaredVec4(sepVec.vec), .00001f);

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
    float multiplierA = -2 * ( object2.getMass() * c ) / combinedMass;
    VecStruct aVec = n.scaledBy(multiplierA);

    object1.adjustVelocity(aVec);

    float multiplierB = 2 * ( object1.getMass() * c ) / combinedMass;
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
void Simulation::calcForcesAll_LessNaive(float dt)
{
    vectorT physicalObjects = this->physicalObjects.getShapes();
    ShapeList deleteList;

    if (!physicalObjects.empty()) {
        for (size_t i = 0; i < physicalObjects.size()-1; i++) {
            MyShape & object1 = *physicalObjects.at(i);

            if (constGravField) {
                object1.adjustMomentum(this->getConstGravFieldVal().scaledBy(1 / dt));
            }

            for (size_t j = i + 1; j < physicalObjects.size(); j++) {
                MyShape & object2 = *physicalObjects.at(j);

                if (this->gravBetweenObjects ) {
                    VecStruct gravVec = calcForceGravNew(object1, object2, dt );

                    object1.adjustMomentum(gravVec);
                    object2.adjustMomentum(gravVec.scaledBy(-1).vec);
                }

                float distance = object1.distanceTo(object2);
                float minSep = object1.getRadius() + object2.getRadius();

                if (distance < minSep) {
                    if (this->collisionType == CollisionType::ELASTIC) {
                        elasticCollision( object1, object2, dt );
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
        object1->adjustMomentum(this->getConstGravFieldVal().scaledBy(1 / dt));

        this->removePhysicalObjects(deleteList);
    }

}



PairCollection Simulation::calculateForceOnExternalNode(const shapePointer_t &curObject, Quadrant &curQuadrant, float dt) {
    //1. a.
    PairCollection deleteList;
    shapePointer_t shapeInQuadrant = curQuadrant.getShapeInQuadrant();

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
PairCollection Simulation::calcForceOnObject_Octree(shapePointer_t curObject, Quadrant &curQuadrant, float dt,
                                                    int recursionLevel)
{
    if ( curQuadrant.isExternal() ) {
        return calculateForceOnExternalNode(curObject, curQuadrant, dt);
    }
    else {
        PairCollection deleteList;
        float distance = curObject->distanceTo(curQuadrant);
        //2.a
        if ( curQuadrant.getWidth() / distance < octreeTheta ) {
            VecStruct gravVec = calcForceGravNew( *curObject, curQuadrant, dt);
            //b.
            curObject->adjustMomentum(gravVec);
        } else { //3.
            /* This seems to work now; it's just absurdly slow
            for (const auto & curQuadrantNew: curQuadrant->children()) {
                deleteList.insertUniqueElements(calcForceOnObject_Octree(curObject, curQuadrantNew, dt, recursionLevel + 1)) ;
            }
             */
            // TODO This should *really* be captured inside the Quadrant class. WTF should Simulations know about these shitty indexes?
            QuadrantPointer_t targetQuadrant;
            for ( int x = 0; x < 2; x++ ) {
                for ( int y = 0; y < 2; y++ ) {
                    for ( int z = 0; z < 2; z++ ) {
                        targetQuadrant = curQuadrant.getQuadrantFromCell( x, y, z );
                        if ( targetQuadrant != nullptr ) {
                            deleteList.insertUniqueElements(calcForceOnObject_Octree(curObject, *targetQuadrant, dt, recursionLevel + 1)) ;
                        }
                    }
                }
            }

        }
        return deleteList;
    }


}

void Simulation::calcForcesAll(float dt) {
    switch(this->forceCalcMethod) {
        case ForceCalculationMethod::NAIVE:
            calcForcesAll_LessNaive(0);
            break;
        case ForceCalculationMethod ::OCTREE:
            PairCollection deleteList;

            for ( const auto & curShape : this->physicalObjects.getShapes() ) {
                deleteList.insertUniqueElements(calcForceOnObject_Octree(curShape, *this->getQuadrant(), dt, 0));
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

ShapeList Simulation::crackPhysicalObject(MyShape &shape) {
    int numberOfFragments = 2;
    VecStruct initialMomentum(shape.getMomentum());

    return ShapeList();
}

ShapeList Interactions::crackPhysicalObject(MyShape &shape, float kineticEnergy, int numberOfPieces) {
    return ShapeList();
}
