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
        return Simulations::bodyFormationCollision(simulationProperties);
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

    PhysicalVector white(1, 1, 1);

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
                    white
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
                    white
            )
    );

    PhysicalVector object3Placement(-1/8.0f * d, -5/8.0f * d, 1, true);

    physicalObjects.addShapeToList(
            make_shared<Particle>(
                    object3Placement,
                    pieceMass,
                    startMomentum,
                    objectDensity,
                    white
            )
    );

    PhysicalVector object4Placement(1/8.0f * d, -5/8.0f * d, 1, true);

    physicalObjects.addShapeToList(
            make_shared<Particle>(
                    object3Placement,
                    pieceMass,
                    startMomentum,
                    objectDensity,
                    white
            )
    );

    return Simulation(physicalObjects, CollisionType::INELASTIC, 0.5);
}

ParticleList
Simulations::manipulatedGroup(ParticleGroupProperties properties, PhysicalVector origin, PhysicalVector momentum) {
    const float distance = 130000;

    ParticleList particleList = Simulations::bodyPlacement(properties, origin);
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
    PhysicalVector white(1, 1, 1);
    float momentumMultiplier = 0.000002;
    ParticleGroupProperties  groupProperties(properties.numShapes, properties.sandboxWidth, properties.mass, white, momentumMultiplier);

    auto  blah =
            [this, numPieces, properties, &physicalObjects, groupProperties](PhysicalVector groupColor, PhysicalVector pos, PhysicalVector mom) {
                physicalObjects.addList(
                        manipulatedGroup(groupProperties, pos, mom));
    };
    PhysicalVector groupColor (1, 1, 1);
    blah(groupColor, PhysicalVector(-8, 4, 0), PhysicalVector(0, 0, 0));
    blah(groupColor, PhysicalVector(-8, -4, 0), PhysicalVector(0,0,0));
    blah(groupColor, PhysicalVector(-4, 8, 0), PhysicalVector(0,0,0));
    blah(groupColor, PhysicalVector(-4, 0, 0), PhysicalVector(0,0,0));
    blah(groupColor, PhysicalVector(-4, -8, 0), PhysicalVector(0,0,0));
    blah(groupColor, PhysicalVector(0, 4, 0), PhysicalVector(0,0,0));
    blah(groupColor, PhysicalVector(0, -4, 0), PhysicalVector(0,0,0));
    blah(groupColor, PhysicalVector(4, 8, 0), PhysicalVector(0,0,0));
    blah(groupColor, PhysicalVector(4, 0, 0), PhysicalVector(0,0,0));
    blah(groupColor, PhysicalVector(4, -8, 0), PhysicalVector(0,0,0));
    blah(groupColor, PhysicalVector(8, 4, 0), PhysicalVector(0,0,0));
    blah(groupColor, PhysicalVector(8, -4, 0), PhysicalVector(0,0,0));

    return Simulation(physicalObjects, CollisionType::INELASTIC, properties.octreeTheta);
}

Simulation Simulations::bodyFormationCollision(PhysicsSandboxProperties properties) {
    cout << "uh, collision" << endl;
    float distance = 130000;
    PhysicalVector target(1000, 0, 0, true);
    ParticleList physicalObjects;

    float momentumMultiplier = 0.000002;
    PhysicalVector white(1, 1, 1);
    PhysicalVector blue(0, 1, 1);
    ParticleGroupProperties  blueGroupProperties(properties.numShapes*10, properties.sandboxWidth, properties.mass, blue, momentumMultiplier);
    PhysicalVector orange(1, 0.5, 0);
    ParticleGroupProperties  orangeGroupProperties(properties.numShapes/100, properties.sandboxWidth, properties.mass, orange, momentumMultiplier);


    ParticleGroupProperties  groupProperties(properties.numShapes, properties.sandboxWidth, properties.mass, white, momentumMultiplier);

    auto  blah =
            [this, properties, &physicalObjects](PhysicalVector pos, PhysicalVector mom, ParticleGroupProperties  groupProperties) {
                physicalObjects.addList(
                        manipulatedGroup(groupProperties, pos, mom));
            };
    // 4 in a diamond, 2 approaching from the sides
//    blah(PhysicalVector(-5, 2, 0), PhysicalVector(12,-20,0), groupProperties);
//    blah(PhysicalVector(5, -2, 0), PhysicalVector(-12,20,0), groupProperties);
//    blah(PhysicalVector(-2, -5, 0), PhysicalVector(20,12,0), groupProperties);
//    blah(PhysicalVector(2, 5, 0), PhysicalVector(-20,-12,0), groupProperties);

//    blah(PhysicalVector(-13, -5, 0), PhysicalVector(50,2,0), blueGroupProperties);
//    blah(PhysicalVector(20, 5, 0), PhysicalVector(-50,-2,0), blueGroupProperties);

    // 2 in a line, 2 meeting diagonally
//    blah(PhysicalVector(0, 5, 0), PhysicalVector(0,-15,0));
//    blah(PhysicalVector(0, 10, 0), PhysicalVector(0,-30,0));
//    blah(PhysicalVector(3, -4, 0), PhysicalVector(-9,12,0));
//    blah(PhysicalVector(-3, -4, 0), PhysicalVector(9,12,0));

    // 2 in a line, 2 meeting diagonally, more slowly
//    blah(PhysicalVector(0, 5, 0), PhysicalVector(0,-8,0));
//    blah(PhysicalVector(0, 10, 0), PhysicalVector(0,-12,0));
//    blah(PhysicalVector(3, -4, 0), PhysicalVector(-5,8,0));
//    blah(PhysicalVector(-3, -4, 0), PhysicalVector(5,8,0));

    //
    blah(PhysicalVector(0, 5, 0), PhysicalVector(0,-15,0), groupProperties);
    blah(PhysicalVector(0, 10, 0), PhysicalVector(0,-30,0), groupProperties);
    blah(PhysicalVector(3, -4, 0), PhysicalVector(-9,12,0), groupProperties);
    blah(PhysicalVector(-3, -4, 0), PhysicalVector(9,12,0), blueGroupProperties);
    blah(PhysicalVector(-20, 0, 0), PhysicalVector(50,0,0), blueGroupProperties);
    blah(PhysicalVector(3, -25, 0), PhysicalVector(0,50,0), blueGroupProperties);
    blah(PhysicalVector(30, 0, 0), PhysicalVector(-50,0,0), orangeGroupProperties);
    blah(PhysicalVector(35, 35, 0), PhysicalVector(-50,-50,0), orangeGroupProperties);
    blah(PhysicalVector(0, 50, 0), PhysicalVector(2,-70,0), orangeGroupProperties);

    return Simulation(physicalObjects, CollisionType::INELASTIC, properties.octreeTheta);
}

ParticleList Simulations::bodyPlacement(ParticleGroupProperties properties, PhysicalVector origin) {
    ParticleList physicalObjects;  // I call functions on this below without ever initializing it first.... Scary.

    const kilograms_per_cubic_meter_t objectDensity = AstronomicalValues::DENSITY_SUN;
    const kilogram_t pieceMass = (properties.mass*1000.0)/properties.numShapes;

    srand (static_cast<unsigned int>(time(nullptr)));

    float momentumMultiplier = 0.000002;
    PhysicalVector startMomentumVec;
    PhysicalVector startPos;
    for (int i = 0; i < properties.numShapes; i++) {
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
                properties.color
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
        value = randMult * pieceMass.value() *
                0.000002;
//                0.000001;
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

