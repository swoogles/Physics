#include "Simulation.h"

Simulation::Simulation(
        ParticleList physicalObjects,
        CollisionType collisionType,
        float octreeTheta)
        :physicalObjects(std::move(physicalObjects))
        ,timeElapsed(0)
        ,minX(FLT_MAX)
        ,maxX(FLT_MIN)
        ,minY(FLT_MAX)
        ,maxY(FLT_MIN)
        ,collisionType(collisionType)
        ,octreeTheta(octreeTheta)
{
    this->updateMinsAndMaxes();
    this->refreshQuadrant();
}

Simulation::Simulation(Simulation originalSimulation, ParticleList newParticles)
    : timeElapsed(originalSimulation.timeElapsed)
            ,minX(FLT_MAX)
            ,maxX(FLT_MIN)
            ,minY(FLT_MAX)
            ,maxY(FLT_MIN)
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

    quadrant = std::make_unique<Quadrant>( 1, pos, side, meter_t(0), PhysicalVector(), kilogram_t(0), PhysicalVector() ) ;
    this->physicalObjects.checkForAllParticles(
            [this, &pos, side](const Particle & curShape) {
                this->quadrant->insert(
                        curShape.radius(),
                        curShape.weightedPosition(),
                        curShape.mass(),
                        curShape.position());
            });
}

void Simulation::updateXYMinsAndMaxes(PhysicalVector curPos) {
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
    this->physicalObjects.applyToAllParticles(
            [this](Particle & curShape) {
                PhysicalVector curPos(curShape.position());
                updateXYMinsAndMaxes(curPos);
            });
}

void Simulation::update(hour_t dt) {
    // This is the first "log(n)" part in "n log(n)"
    calcForcesAll(dt);
    physicalObjects.update(dt);
    updateTimeElapsed(dt);

    // TODO This causes another full iteration of all shapes. If it's going to happen,
    //  it should be done during one of the earlier iterations.
    updateMinsAndMaxes();
    // This is the first "n" part in "n log(n)"
    refreshQuadrant();
}

void Simulation::updateTimeElapsed(hour_t dt) { timeElapsed += dt; }

hour_t Simulation::getTimeElapsed() const { return timeElapsed; }


//1.
//a. If the current node is an external node
//b. (and it is not body b),
//c. calculate the force exerted by the current node on b
//d. add this amount to b’s net force.
//2.
//a. Otherwise, calculate the ratio s/d. If s/d < θ, treat this internal node as a single body, and calculate the force it exerts on body b
//b. add this amount to b’s net force.
//3. Otherwise, run the procedure recursively on each of the current node’s children.


void Simulation::calcForcesAll(hour_t dt) {
    this->physicalObjects.applyToAllParticles(
            [this, dt](Particle & curShape) {
                auto quadrantFunction =
                [this, &curShape, dt](Quadrant & quadrant) {
                    PhysicalVector gravVec = this->interactions.calcForceGravNew( curShape, quadrant, dt);
                    curShape.adjustMomentum(gravVec);
                    //c.
                    if ( quadrant.isExternal() &&  curShape.isTouching(quadrant.getShapePosition(), quadrant.getShapeRadius()) ) {
                        curShape.setTouchingAnotherParticle(true);
                    }
                };
                auto terminalPredicate =
                        [this, &curShape](Quadrant & quadrant) {
                            double distance = curShape.distanceTo(quadrant);
                            return quadrant.getWidth() / distance < octreeTheta  && !quadrant.positionIsInQuadrantBoundaries(curShape.position());
                };
                quadrant->applyToAllChildren(quadrantFunction, terminalPredicate);
            });
}

void Simulation::applySideEffectingFunctionsToInnards(
        function<void(const Quadrant &)> quadrantFunctor,
        function<void (const Particle &)> particleFunctor)
        const {
    quadrant->applyToAllChildrenConstant(quadrantFunctor);
    physicalObjects.checkForAllParticles(particleFunctor);
}

