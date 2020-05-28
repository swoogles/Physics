/*
 * Simulations.cpp
 *
 *  Created on: Jul 22, 2011
 *      Author: brasure
 */

#include "Simulations.h"

Simulation Simulations::createSimulation(CraftedSimulation simulation, PhysicsSandboxProperties simulationProperties) {
    if ( simulation == CraftedSimulation::BODY_FORMATION) {
        return Simulations::bodyFormation(simulationProperties.numShapes, simulationProperties);
    } else if ( simulation == DISRUPT_GROUP) {
//        return Simulations::disruption_ArbitraryList(simulationProperties);
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
    float d= 3e5;

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

    return Simulation(physicalObjects, CollisionType::INELASTIC, 0.5);
}

Simulation Simulations::bodyFormation(int numPieces, PhysicsSandboxProperties properties) {
    ParticleList physicalObjects;  // I call functions on this below without ever initializing it first.... Scary.

    const kilograms_per_cubic_meter_t objectDensity = AstronomicalValues::DENSITY_SUN;
    const kilogram_t pieceMass = (properties.mass*1000.0)/numPieces;
    PhysicalVector target(1000, 0, 0, true);

    PhysicalVector newColor (1, 1, 1);

    srand (static_cast<unsigned int>(time(nullptr)));

    PhysicalVector startMomentumVec;
    PhysicalVector startPos;
    for (int i = 0; i < numPieces; i++) {
        if (i % 2 == 0) {
            startMomentumVec = randomSplitBodyMomentum(pieceMass);
            startPos = randomPointInSphere(properties.sandboxWidth, target);
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
            PhysicalVector newPosAttempt = randomPointInSphere(properties.sandboxWidth, target);
            curShape->setPos( newPosAttempt );
        }
        physicalObjects.addShapeToList( curShape );
    }

    return Simulation(physicalObjects, CollisionType::INELASTIC, properties.octreeTheta);
}

PhysicalVector Simulations::randomSplitBodyPlacement(float pieceRadius, PhysicalVector target) {
  int randMult;
  double largestDistance = 0;

  double values[] = {0, 0, 0};
  for (double &value : values) {
    randMult = rand()%450;
    if (randMult % 2 == 0) {
      randMult *= -1;
    }
      value = randMult * pieceRadius;
    if (value > largestDistance ) {
      largestDistance = value;
    }
  }

  return PhysicalVector(values[0], values[1], values[2], false).plus(target);
}

PhysicalVector Simulations::randomSplitBodyMomentum(kilogram_t pieceMass) {
    static int randMult;

    double values[] = {0, 0, 0};

    for (auto &value : values) {
        // Set the range of momenta, and have them be half positive/half negative
        randMult = rand()%100;
        if (randMult % 2 == 0)
            randMult *= -1;
        value = randMult * pieceMass.value() * 0.000001;
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

