//
// Created by bfrasure on 12/10/18.
//

#include "Interactions.h"


ParticleList Interactions::crackPhysicalObject(Particle &shape, joule_t kineticEnergy, int numberOfPieces) {
//    float energyPerFragment =
    return ParticleList();
}

PhysicalVector Interactions::calcForceGravNew(Particle &object1, Moveable &object2, duration dt) const {
    PhysicalVector sepVec(object1.vectorTo(object2));

    double rSquared = std::max(sgLengthSquaredVec4(sepVec.vec), .00001f);

    double G = 6.67384e-11;
    double forceMagnitude = (G * object1.mass() * object2.mass()).value() / rSquared;

    return sepVec
            .unit()
            .scaledBy(forceMagnitude)
            .scaledBy(dt.value());
}

void Interactions::elasticCollision(Particle &object1, Particle &object2, duration dt) {
    PhysicalVector sepVecUnit = object1.vectorTo(object2).unit();
    PhysicalVector n(sepVecUnit);

    double c = n.scalarProduct4(object1.velocity().minus(object2.velocity()));

    auto combinedMass = (object2.mass() + object1.mass()).value();
    double multiplierA = -2 * (object2.mass().value() * c ) / combinedMass;
    PhysicalVector aVec = n.scaledBy(multiplierA);

    object1.adjustVelocity(aVec);

    double multiplierB = 2 * (object1.mass().value() * c ) / combinedMass;
    PhysicalVector bVec = n.scaledBy(multiplierB);

    object2.adjustVelocity(bVec);

    // TODO Figure out how necessary these lines are.
    while ( object1.isTouching(object2) ) {
        object1.update(dt/30);
        object2.update(dt/30);
    }
}

