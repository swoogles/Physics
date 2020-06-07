/*
 * Simulations.cpp
 *
 *  Created on: Jul 22, 2011
 *      Author: brasure
 */

#include "Simulations.h"

Simulation Simulations::createSimulation(CraftedSimulation simulation, PhysicsSandboxProperties simulationProperties) {
    if ( simulation == CraftedSimulation::BODY_FORMATION) {
//        return Simulations::bodyFormation(simulationProperties.numShapes, simulationProperties);
        return Simulations::bodyFormationCollision(simulationProperties.numShapes, simulationProperties);
    } else if ( simulation == DISRUPT_GROUP) {
    } else if ( simulation == QUADRANT_TESTING) {
        return Simulations::QuadrantTesting_simplest();
    } else if ( simulation == MULTIPLE_ORBITERS) {
//        return multipleOrbiters(simulationProperties);
    } else {
        exit(1);
    }

}


Simulation Simulations::QuadrantTesting_simplest() {
    ParticleList physicalObjects;

    int numPieces=3;
    kilograms_per_cubic_meter_t objectDensity = AstronomicalValues::DENSITY_SUN;
    kilogram_t pieceMass = AstronomicalValues::MASS_SUN/numPieces;
    PhysicalVector startMomentum(0, 0, 0);

    PhysicalVector newColor (1, 1, 1);

    shared_ptr<Particle> curShape;
    float d= 2.0e4;

    //#1
    PhysicalVector object1Placement(+5/8.0f * d, +7/8.0f * d, 1, true);

    physicalObjects.addShapeToList(
            make_shared<Particle>(
                    object1Placement,
                    pieceMass,
                    startMomentum,
                    objectDensity,
                    newColor
            )
            );

    //#2
    PhysicalVector object2Placement(-7/8.0f * d, +7/8.0f * d, 1, true);

    physicalObjects.addShapeToList(
            make_shared<Particle>(
                    object2Placement,
                    pieceMass,
                    startMomentum,
                    objectDensity,
                    newColor
            )
    );

    PhysicalVector object3Placement(-1/8.0f * d, -5/8.0f * d, 1, true);

    physicalObjects.addShapeToList(
            make_shared<Particle>(
                    object3Placement,
                    pieceMass,
                    startMomentum,
                    objectDensity,
                    newColor
            )
    );

    PhysicalVector object4Placement(1/8.0f * d, -5/8.0f * d, 1, true);

    physicalObjects.addShapeToList(
            make_shared<Particle>(
                    object3Placement,
                    pieceMass,
                    startMomentum,
                    objectDensity,
                    newColor
            )
    );

    return Simulation(physicalObjects, CollisionType::INELASTIC, 0.5);
}

ParticleList Simulations::manipulatedGroup(int numPieces, PhysicsSandboxProperties properties, PhysicalVector origin, PhysicalVector momentum) {
    const float distance = 130000;

    ParticleList particleList = Simulations::bodyPlacement(numPieces, properties, origin);
    particleList.applyToAllParticles([origin, distance, momentum](Particle & particle) {
        particle.setPos(particle.position().plus(origin.scaledBy(distance)));
        particle.adjustMomentum(momentum);

    });
    return particleList;
}

Simulation Simulations::bodyFormation(int numPieces, PhysicsSandboxProperties properties) {
    float distance = 130000;
    PhysicalVector target(1000, 0, 0, true);
    ParticleList physicalObjects;

    auto  blah =
            [this, numPieces, properties, &physicalObjects](PhysicalVector pos, PhysicalVector mom) {
                physicalObjects.addList(
                        manipulatedGroup(numPieces, properties, pos, mom) );
    };
    blah(PhysicalVector(-8, 4, 0), PhysicalVector(0, 0, 0));
    blah(PhysicalVector(-8, -4, 0), PhysicalVector(0,0,0));
    blah(PhysicalVector(-4, 8, 0), PhysicalVector(0,0,0));
    blah(PhysicalVector(-4, 0, 0), PhysicalVector(0,0,0));
    blah(PhysicalVector(-4, -8, 0), PhysicalVector(0,0,0));
    blah(PhysicalVector(0, 4, 0), PhysicalVector(0,0,0));
    blah(PhysicalVector(0, -4, 0), PhysicalVector(0,0,0));
    blah(PhysicalVector(4, 8, 0), PhysicalVector(0,0,0));
    blah(PhysicalVector(4, 0, 0), PhysicalVector(0,0,0));
    blah(PhysicalVector(4, -8, 0), PhysicalVector(0,0,0));
    blah(PhysicalVector(8, 4, 0), PhysicalVector(0,0,0));
    blah(PhysicalVector(8, -4, 0), PhysicalVector(0,0,0));

    return Simulation(physicalObjects, CollisionType::INELASTIC, properties.octreeTheta);
}

Simulation Simulations::bodyFormationCollision(int numPieces, PhysicsSandboxProperties properties) {
    cout << "uh, collision" << endl;
    float distance = 130000;
    PhysicalVector target(1000, 0, 0, true);
    ParticleList physicalObjects;

    auto  blah =
            [this, numPieces, properties, &physicalObjects](PhysicalVector pos, PhysicalVector mom) {
                physicalObjects.addList(
                        manipulatedGroup(numPieces, properties, pos, mom) );
            };
    // 4 in a diamond, 2 approaching from the sides
    /*
    blah(PhysicalVector(-5, 2, 0), PhysicalVector(12,-20,0));
    blah(PhysicalVector(5, -2, 0), PhysicalVector(-12,20,0));
    blah(PhysicalVector(-2, -5, 0), PhysicalVector(20,12,0));
    blah(PhysicalVector(2, 5, 0), PhysicalVector(-20,-12,0));
    blah(PhysicalVector(-13, -5, 0), PhysicalVector(50,2,0));
    blah(PhysicalVector(20, 5, 0), PhysicalVector(-50,-2,0));
     */

    // 2 in a line, 2 meeting diagonally
//    blah(PhysicalVector(0, 5, 0), PhysicalVector(0,-15,0));
//    blah(PhysicalVector(0, 10, 0), PhysicalVector(0,-30,0));
//    blah(PhysicalVector(3, -4, 0), PhysicalVector(-9,12,0));
//    blah(PhysicalVector(-3, -4, 0), PhysicalVector(9,12,0));

    // 2 in a line, 2 meeting diagonally, more slowly
    blah(PhysicalVector(0, 5, 0), PhysicalVector(0,-8,0));
    blah(PhysicalVector(0, 10, 0), PhysicalVector(0,-12,0));
    blah(PhysicalVector(3, -4, 0), PhysicalVector(-5,8,0));
    blah(PhysicalVector(-3, -4, 0), PhysicalVector(5,8,0));

    //
//    blah(PhysicalVector(0, 5, 0), PhysicalVector(0,-15,0));
//    blah(PhysicalVector(0, 10, 0), PhysicalVector(0,-30,0));
//    blah(PhysicalVector(3, -4, 0), PhysicalVector(-9,12,0));
//    blah(PhysicalVector(-3, -4, 0), PhysicalVector(9,12,0));
//    blah(PhysicalVector(-20, 0, 0), PhysicalVector(50,0,0));
//    blah(PhysicalVector(3, -25, 0), PhysicalVector(0,50,0));
//    blah(PhysicalVector(30, 0, 0), PhysicalVector(-50,0,0));
//    blah(PhysicalVector(35, 35, 0), PhysicalVector(-50,-50,0));
//    blah(PhysicalVector(0, 50, 0), PhysicalVector(2,-70,0));

    return Simulation(physicalObjects, CollisionType::INELASTIC, properties.octreeTheta);
}

ParticleList Simulations::bodyPlacement(int numPieces, PhysicsSandboxProperties properties, PhysicalVector origin) {
    ParticleList physicalObjects;  // I call functions on this below without ever initializing it first.... Scary.

    const kilograms_per_cubic_meter_t objectDensity = AstronomicalValues::DENSITY_SUN;
    const kilogram_t pieceMass = (properties.mass*1000.0)/numPieces;

    PhysicalVector newColor (1, 1, 1);

    srand (static_cast<unsigned int>(time(nullptr)));

    PhysicalVector startMomentumVec;
    PhysicalVector startPos;
    for (int i = 0; i < numPieces; i++) {
        if (i % 2 == 0) {
            startMomentumVec = randomSplitBodyMomentum(pieceMass);
            startPos = randomPointInSphere(properties.sandboxWidth, origin);
        }
        else {
            startMomentumVec = startMomentumVec.scaledBy(-1);
            startPos = startPos.scaledBy(-1);
        }

        const shared_ptr<Particle> curShape = make_shared<Particle>(
                startPos,
                pieceMass,
                startMomentumVec,
                objectDensity,
                newColor
        );

        //Check if being placed on previously created object
        while ( physicalObjects.hasConflictsWith( *curShape ) ) {
            cout << "conflict. trying again" << endl;
            PhysicalVector newPosAttempt = randomPointInSphere(properties.sandboxWidth, origin);
            curShape->setPos( newPosAttempt );
        }
        physicalObjects.addShapeToList( curShape );
    }

    return std::move(physicalObjects);
}

PhysicalVector Simulations::randomSplitBodyMomentum(kilogram_t pieceMass) {
    static int randMult;

    double values[] = {0, 0, 0};

    for (auto &value : values) {
        // Set the range of momenta, and have them be half positive/half negative
        randMult = rand()%100;
        if (randMult % 2 == 0)
            randMult *= -1;
        value = randMult * pieceMass.value() * 0.000002;
    }
    return PhysicalVector(values[0], values[1], values[2], false);
}

float randomFloat() {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

/*
 * Algorithm explained and designed here:
 * https://karthikkaranth.me/blog/generating-random-points-in-a-sphere/
 */
PhysicalVector Simulations::randomPointInSphere(double maxDistance, PhysicalVector target) {
    auto u = randomFloat();
    float v = randomFloat();
    auto theta = u * 2.0f * float(M_PI);
    auto phi = acos(2.0f * v - 1.0f);
    auto r = std::cbrt(randomFloat());
    auto sinTheta = sin(theta);
    auto cosTheta = cos(theta);
    auto sinPhi = sin(phi);
    auto cosPhi = cos(phi);
    auto x = r * sinPhi * cosTheta;
    auto y = r * sinPhi * sinTheta;
    auto z = r * cosPhi;
    return PhysicalVector(x, y, z, true).scaledBy(maxDistance).plus(target);
}

