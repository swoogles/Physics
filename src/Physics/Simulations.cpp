/*
 * Simulations.cpp
 *
 *  Created on: Jul 22, 2011
 *      Author: brasure
 */

#include "Simulations.h"

SimulationPointer_t Simulations::createSimulation(char simNumber, PhysicsSandboxProperties simulationProperties) {
    ForceCalculationMethod forceCalculationMethod = simulationProperties.forceCalculationMethod;
    if ( simNumber == '0' ) {
    } else if ( simNumber == '1' ) {
        return Simulations::bodyFormation_ArbitraryList(simulationProperties.numShapes, simulationProperties);
    } else if ( simNumber == '2' ) {
        return Simulations::disruption_ArbitraryList(simulationProperties);
    } else if ( simNumber == '3' ) {
        return Simulations::QuadrantTesting_simplest(forceCalculationMethod);
    } else if ( simNumber == '4') {
        return multipleOrbiters(simulationProperties);
    } else {
        exit(1);
    }

}


SimulationPointer_t Simulations::disruption_ArbitraryList(PhysicsSandboxProperties properties) {
    SimulationPointer_t curSimulation = Simulations::bodyFormation_ArbitraryList(properties.numShapes, properties);

    kilograms_per_cubic_meter_t objectDensity = AstronomicalValues::DENSITY_SUN;
    kilogram_t pieceMass = properties.mass * 250;
    PhysicalVector startMomentum(0, 52000, 0);
    PhysicalVector newColor (1, 0, 1);

    PhysicalVector startPlacement (2.2e5f , 0, 0, true);
    ParticleList newParticles;

    shared_ptr<Particle> curShape = make_shared<Particle>(
            startPlacement,
            pieceMass,
            startMomentum,
            objectDensity,
            newColor
    );
    newParticles.addShapeToList(curShape);

    shared_ptr<Particle> opposingBody = make_shared<Particle>(
            startPlacement.scaledBy(-1),
            pieceMass,
            startMomentum.scaledBy(-1),
            objectDensity,
            newColor
    );
    newParticles.addShapeToList(opposingBody);
    return make_unique<Simulation>(*curSimulation, newParticles);
}


SimulationPointer_t Simulations::multipleOrbiters(PhysicsSandboxProperties properties){
    SimulationPointer_t curSimulation = Simulations::bodyFormation_ArbitraryList(properties.numShapes, properties);

    kilograms_per_cubic_meter_t objectDensity = AstronomicalValues::DENSITY_SUN;
    kilogram_t pieceMass = properties.mass * 150;
    PhysicalVector startMomentum(0, 45000, 0);
    PhysicalVector newColor (1, 0, 1);

    PhysicalVector startPlacement (2.1e5f , 0, 0, true);
    ParticleList newParticles;

    shared_ptr<Particle> curShape = make_shared<Particle>(
            startPlacement,
            pieceMass,
            startMomentum.scaledBy(1.2),
            objectDensity,
            newColor
    );
    newParticles.addShapeToList(curShape);

    shared_ptr<Particle> curShapeOpposite = make_shared<Particle>(
            startPlacement.scaledBy(-1),
            pieceMass,
            startMomentum.scaledBy(-1.2),
            objectDensity,
            newColor
    );
    newParticles.addShapeToList(curShapeOpposite);

    PhysicalVector startPlacement2 (1.3e5f , -2.0e5f, 0, true);
    shared_ptr<Particle> opposingBody = make_shared<Particle>(
            startPlacement2,
            pieceMass,
            startMomentum,
            objectDensity,
            newColor
    );
    newParticles.addShapeToList(opposingBody);

    shared_ptr<Particle> opposingBodyOpposite = make_shared<Particle>(
            startPlacement2.scaledBy(-1),
            pieceMass,
            startMomentum.scaledBy(-1),
            objectDensity,
            newColor
    );
    newParticles.addShapeToList(opposingBodyOpposite);

    return make_unique<Simulation>(*curSimulation, newParticles);
}

SimulationPointer_t Simulations::QuadrantTesting_simplest(ForceCalculationMethod forceCalculationMethod) {
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

    return make_unique<Simulation>(physicalObjects, CollisionType::INELASTIC, forceCalculationMethod, 0.5);
}

SimulationPointer_t Simulations::bodyFormation_ArbitraryList(int numPieces, PhysicsSandboxProperties properties) {
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
            PhysicalVector newPosAttempt = randomPointInSphere(properties.sandboxWidth, target);
            curShape->setPos( newPosAttempt );
        }
        physicalObjects.addShapeToList( curShape );
    }

    return make_unique<Simulation>(physicalObjects, CollisionType::INELASTIC, properties.forceCalculationMethod, properties.octreeTheta);
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
        value = randMult * pieceMass.value() * 0.000003;
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

