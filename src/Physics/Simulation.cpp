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
    this->refreshQuadrant(this->physicalObjects);
}

void Simulation::refreshQuadrant(ParticleList &physicalObjects) {
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
    calcForcesAll(this->physicalObjects, dt);
    physicalObjects.update(dt);
    updateTimeElapsed(dt);

    // TODO This causes another full iteration of all shapes. If it's going to happen,
    //  it should be done during one of the earlier iterations.
    updateMinsAndMaxes();
    // This is the first "n" part in "n log(n)"
    refreshQuadrant(this->physicalObjects);
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


void Simulation::calcForcesAll(ParticleList &physicalObjects, hour_t dt) {
    this->physicalObjects.applyToAllParticles(
            [this, dt](Particle & particle) {
                auto quadrantFunction =
                [this, &particle, dt](Quadrant & quadrant) {
                    particle.adjustMomentum(
                            this->interactions.calcForceGravNew(particle, quadrant, dt)
                    );
                    //c.
                    if (quadrant.isExternal() && particle.isTouching(quadrant.getParticlePosition(),
                                                                     quadrant.getParticleRadius()) ) {
                        particle.setTouchingAnotherParticle(true);
                    }
                };
                auto terminalPredicate =
                        [this, &particle](Quadrant & quadrant) {
                            double distance = particle.distanceTo(quadrant);
                            return quadrant.getWidth() / distance < octreeTheta  && !quadrant.positionIsInQuadrantBoundaries(particle.position());
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

