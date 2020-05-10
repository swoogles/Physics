#include "Simulation.h"

Simulation::Simulation(ParticleList physicalObjects, CollisionType collisionType,
                       ForceCalculationMethod forceCalculationMethod, float octreeTheta)
        :physicalObjects(std::move(physicalObjects))
        ,timeElapsed(0)
        ,minX(FLT_MAX)
        ,maxX(FLT_MIN)
        ,minY(FLT_MAX)
        ,maxY(FLT_MIN)
        ,forceCalcMethod(forceCalculationMethod)
        ,collisionType(collisionType)
        ,octreeTheta(octreeTheta)
{
    cout << "Constructing" << endl;
    this->updateMinsAndMaxes();
    this->refreshQuadrant();
}

Simulation::Simulation(Simulation &originalSimulation, ParticleList newParticles)
    : timeElapsed(originalSimulation.timeElapsed)
            ,minX(FLT_MAX)
            ,maxX(FLT_MIN)
            ,minY(FLT_MAX)
            ,maxY(FLT_MIN)
            ,forceCalcMethod(originalSimulation.forceCalcMethod)
            ,collisionType(originalSimulation.collisionType)
            ,octreeTheta(originalSimulation.octreeTheta)
{
    originalSimulation.physicalObjects.addList(std::move(newParticles));
    this->physicalObjects = originalSimulation.physicalObjects;
    this->updateMinsAndMaxes();
    this->refreshQuadrant();

}


void Simulation::refreshQuadrant() {
    PhysicalVector pos(0, 0, 0, true);

    float side = 10e7; //Formation Value . ACK!!!! How miserably hard-coded!!
    quadrant = nullptr;

    for ( const auto & curShape : physicalObjects.getShapes() ) {
        if ( quadrant == nullptr ) {
            quadrant = std::make_shared<Quadrant>( curShape, 1, pos, side ) ;
        } else {
            quadrant->insert(curShape);
        }
    }
}

void Simulation::updateXYMinsAndMaxes(PhysicalVector curPos) {
    // TODO Use min/max functions instead of comparisons here.
    resetXYMinsAndMaxes();
    maxX = std::max(maxX, curPos.x());
    minX = std::min(minX, curPos.x());

    maxY = std::max(maxY, curPos.y());
    minY = std::min(minY, curPos.y());
}

MaximumValues Simulation::getXYMinsAndMaxes() const {
    return {
            this->minX,
            this->minY,
            this->maxX,
            this->maxY
    };
}


void Simulation::resetXYMinsAndMaxes() {
	minX = FLT_MAX;
	maxX = FLT_MIN;

	minY = FLT_MAX;
	maxY = FLT_MIN;
}

void Simulation::updateMinsAndMaxes() {
    for ( const auto & curShape : this->physicalObjects.getShapes() ) {
        PhysicalVector curPos(curShape->position());
        updateXYMinsAndMaxes(curPos);
    }
}

void Simulation::update(hour_t dt) {
    calcForcesAll(dt);
    physicalObjects.update(dt);
    updateTimeElapsed(dt);

    // TODO This causes another full iteration of all shapes. If it's going to happen, it should be done during one of the earlier iterations.
    updateMinsAndMaxes();
    // This is the first "n" part in "n log(n)"
    refreshQuadrant();
}

void Simulation::removePhysicalObjects(ParticleList shapesToRemove) {
    physicalObjects.remove(shapesToRemove);
}

void Simulation::updateTimeElapsed(hour_t dt) { timeElapsed += dt; }

hour_t Simulation::getTimeElapsed() const { return timeElapsed; }

QuadrantPointer_t Simulation::getQuadrant() const { return quadrant; }


// I'm working on this method because I think there are some basic
// structural changes that I can make so that it will be more legible and
// more suitable for later parallelization
// TODO This should return the deleted list, so that it can coexist with the Octree version
void Simulation::calcForcesAll_LessNaive(hour_t dt) {
    particleVector physicalObjects = this->physicalObjects.getShapes();
    ParticleList deleteList;

    if (!physicalObjects.empty()) {
        for (size_t i = 0; i < physicalObjects.size()-1; i++) {
            Particle & object1 = *physicalObjects.at(i);

            for (size_t j = i + 1; j < physicalObjects.size(); j++) {
                Particle & object2 = *physicalObjects.at(j);

                PhysicalVector gravVec = this->interactions.calcForceGravNew(object1, object2, dt );

                object1.adjustMomentum(gravVec);
                object2.adjustMomentum(gravVec.scaledBy(-1));

                if (object1.isTouching(object2)) {
                    if (this->collisionType == CollisionType::ELASTIC) {
                        this->interactions.elasticCollision( object1, object2, dt );
                    }
                    else if (this->collisionType == CollisionType::INELASTIC){
                        object1.mergeWith( object2 );
                        deleteList.addShapeToList(physicalObjects.at(j));
                    }
                }
            }
        }

        this->removePhysicalObjects(deleteList);
    }

}

PairCollection Simulation::calculateForceOnExternalNode(
        const physicalObject_t &curObject,
        Quadrant &curQuadrant,
        hour_t dt) const {
    //1. a.
    PairCollection deleteList;
    physicalObject_t shapeInQuadrant = curQuadrant.getShapeInQuadrant();

    //b.
    if ( curObject != shapeInQuadrant ) {
        PhysicalVector gravVec = this->interactions.calcForceGravNew( *curObject, *shapeInQuadrant, dt);
        curObject->adjustMomentum(gravVec);
        //c.
        if ( curObject->isTouching( *shapeInQuadrant ) ) {
            TouchingPair pair(curObject, shapeInQuadrant);
            deleteList.insertIfUnique(pair);
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
PairCollection Simulation::calcForceOnObject_Octree(
        physicalObject_t curObject,
        Quadrant &curQuadrant,
        hour_t dt,
        int recursionLevel) const {
    if ( curQuadrant.isExternal() ) {
        return calculateForceOnExternalNode(curObject, curQuadrant, dt);
    }
    else {
        PairCollection deleteList;
        double distance = curObject->distanceTo(curQuadrant);
        //2.a
        if ( curQuadrant.getWidth() / distance < octreeTheta ) {
            PhysicalVector gravVec = this->interactions.calcForceGravNew( *curObject, curQuadrant, dt);
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

void Simulation::calcForcesAll(hour_t dt) {
    switch(this->forceCalcMethod) {
        case ForceCalculationMethod::NAIVE:
            calcForcesAll_LessNaive(dt);
            break;
        case ForceCalculationMethod ::OCTREE:
            PairCollection deleteList;

//            cout << this->physicalObjects.getShapes().at(0)->position() << endl;
            for ( const auto & curShape : this->physicalObjects.getShapes() ) {
                deleteList.insertUniqueElements(calcForceOnObject_Octree(curShape, *this->getQuadrant(), dt, 0));
            }

            this->removePhysicalObjects(deleteList.doomed().getShapes() );
            deleteList.mergePairs();
            break;
    }
}

size_t Simulation::getSize() const {
    return this->physicalObjects.getShapes().size();
}

// TODO Update this very soon
kilogram_t Simulation::getMass() const {
    auto mass = kilogram_t(0);
    for (const auto & shape : this->physicalObjects.getShapes()) {
        mass += shape->mass();
    }
    return mass;
}

