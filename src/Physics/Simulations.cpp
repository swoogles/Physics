/*
 * Simulations.cpp
 *
 *  Created on: Jul 22, 2011
 *      Author: brasure
 */

#include "Simulations.h"

SimulationPointer_t
Simulations::createSimulation(char simNumber, PhysicsSandboxProperties simulationProperties)
{
    ForceCalculationMethod forceCalculationMethod = simulationProperties.forceCalculationMethod;
    //******CURRENT SIMULATION*****
    if ( simNumber == '0' ) {
    } else if ( simNumber == '1' ) {
        return Simulations::bodyFormation_ArbitraryList(simulationProperties.numShapes, simulationProperties);
    } else if ( simNumber == '2' ) {
        return Simulations::disruption_ArbitraryList(simulationProperties);
    } else if ( simNumber == '3' ) {
    } else if ( simNumber == '4' ) {
        return Simulations::billiards1(15, forceCalculationMethod);
    } else if ( simNumber == '5' ) {
        return Simulations::billiards2_ReturnSimulation(15, forceCalculationMethod);
    } else if ( simNumber == '7' ) {
        return Simulations::QuadrantTesting_simplest(forceCalculationMethod);
    } else if ( simNumber == '8' ) {
        return Simulations::billiards3_ArbitraryList(5, forceCalculationMethod);
    } else {
        exit(1);
    }

}


SimulationPointer_t Simulations::billiards1(int numRows, ForceCalculationMethod forceCalculationMethod) {
    ParticleList physicalObjects;

    int numPieces = 0;
    kilogram_t cueMass = kilogram_t(100.0);

    float ballMass = 0.156;
    float ballRadius = .95;

    for (int i = 1; i < numRows+1; i++)
        numPieces+=i;

    PhysicalVector cueVelocity(30, -75, 0);
    PhysicalVector cueMomentum = cueVelocity.scaledBy(cueMass.value());

    PhysicalVector newColor(1, 1, 1, false);

    shared_ptr<Particle> curShape;

    PhysicalVector cuePos(numRows, numRows*3, 0, true);
    curShape = make_shared<Particle>(
            cuePos,
            cueMass.value(),
            ballRadius,
            cueMomentum,
            newColor
    );

    physicalObjects.addShapeToList( curShape );

    PhysicalVector ballMomentum(0, 0, 0, false);
    int cutOff = numRows*2;
    for (int i = 1; i < numRows+1; i++) {
        for (int j = i; j < cutOff; j+= 2) {
            PhysicalVector ballPos( j* 1.7,  (i*2.5), 0, true);

            curShape = make_shared<Particle>(
                    ballPos,
                    ballMass,
                    ballRadius,
                    ballMomentum,
                    newColor
            );
            physicalObjects.addShapeToList( curShape );
        }
        cutOff--;
    }

    // TODO Fix places where a default 0 got passed to Simulation dt
    return make_unique<Simulation>(physicalObjects, CollisionType::ELASTIC, false, forceCalculationMethod, 0.5);
}

SimulationPointer_t Simulations::billiards2_ReturnSimulation(int numRows, ForceCalculationMethod forceCalculationMethod)
{
    ParticleList physicalObjects;

    float cueMass = 100.0;
    float ballMass = 0.156;

    float ballRadius = .95;

    PhysicalVector cueVelocity(30, -75, 0);
    PhysicalVector cueMomentum = cueVelocity.scaledBy(cueMass);

    PhysicalVector newColor(1, 0, 1, false);

    shared_ptr<Particle> shapeForInsertion;

    PhysicalVector cuePos(numRows, numRows*3, 0, true);
    shapeForInsertion = make_shared<Particle>(
            cuePos,
            cueMass,
            ballRadius,
            cueMomentum,
            newColor
    );
    physicalObjects.addShapeToList( shapeForInsertion );

    PhysicalVector ballMomentum(0, 0, 0);
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numRows; j++) {
            PhysicalVector ballPos( j* 1.3,  (i*3), 0, true);

            float greenValue = -( ( -.5 + (j/float(numRows)) ) * ( -.5 + (j/float(numRows)) ) )+ 1.0;
            PhysicalVector ballColor(1, greenValue, 1, false);

            shapeForInsertion = make_shared<Particle>(
                    ballPos,
                    ballMass,
                    ballRadius,
                    ballMomentum,
                    ballColor
            );

            physicalObjects.addShapeToList( shapeForInsertion );
        }
    }
    return make_unique<Simulation>(physicalObjects, CollisionType::ELASTIC, false, forceCalculationMethod, 0.5);
}


SimulationPointer_t Simulations::billiards3_ArbitraryList(int numRows, ForceCalculationMethod forceCalculationMethod) {
    ParticleList physicalObjects;

    float cueMass = 100.0;
    float ballMass = 0.156;

    float ballRadius = .95;

    PhysicalVector cueVelocity(30, -75, 0);
    PhysicalVector cueMomentum = cueVelocity.scaledBy(cueMass);

    PhysicalVector newColor(1, 1, 1, false);

    shared_ptr<Particle> shapeForInsertion;

    PhysicalVector cuePos (numRows, numRows*5, 0, true);
    shapeForInsertion = make_shared<Particle>(
            cuePos,
            cueMass,
            ballRadius,
            cueMomentum,
            newColor
    );
    physicalObjects.addShapeToList( shapeForInsertion );

    PhysicalVector ballMomentum(0, 0, 0);
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numRows; j++) {
            for (int z = 0; z < numRows; z++) {
                PhysicalVector ballPos( j* 4,  (i*4),  (z*4), true);

                float greenValue = -( ( -.5 + (z/float(numRows)) ) * ( -.5 + (z/float(numRows)) ) )+ 1.0;
                PhysicalVector ballColor(0, greenValue, 0, false);

                shapeForInsertion = make_shared<Particle>(
                        ballPos,
                        ballMass,
                        ballRadius,
                        ballMomentum,
                        ballColor
                );

                physicalObjects.addShapeToList( shapeForInsertion );
            }
        }
    }
    return make_unique<Simulation>(physicalObjects, CollisionType::ELASTIC, false, forceCalculationMethod, 0.5);
}

SimulationPointer_t Simulations::disruption_ArbitraryList(PhysicsSandboxProperties properties)
{
    SimulationPointer_t curSimulation = Simulations::bodyFormation_ArbitraryList(1000, properties);

    int numPieces = 1;
    kilograms_per_cubic_meter_t objectDensity = AstronomicalValues::DENSITY_SUN;
    kilogram_t pieceMass = properties.mass / numPieces;
    PhysicalVector startMomentum(0, 0, 0);
    PhysicalVector newColor (1, 0, 1);

    PhysicalVector startPlacement (-1e7f , 0, 0, true);

    shared_ptr<Particle> curShape = make_shared<Particle>(
            startPlacement,
            pieceMass,
            startMomentum,
            objectDensity,
            newColor
    );

    ParticleList disruptingObject(curShape);
//    Simulation & sim = *curSimulation;
    // TODO actually use this, once I can prevent the initial sharedPtr from killing it when this function exits
//    Simulation disruptedSimulation(std::move(sim), disruptingObject);
    curSimulation->addPhysicalObjectToList( curShape );
    return curSimulation;
}

SimulationPointer_t Simulations::QuadrantTesting_simplest(ForceCalculationMethod forceCalculationMethod)
{
    ParticleList physicalObjects;

    int numPieces=2;
    kilograms_per_cubic_meter_t objectDensity = AstronomicalValues::DENSITY_SUN;
    kilogram_t pieceMass = (kilogram_t(AstronomicalValues::MASS_SUN))/(numPieces);
    PhysicalVector startMomentum(0, 0, 0);

    PhysicalVector newColor (1, 1, 1);

    shared_ptr<Particle> curShape;
    float d= 3e4;

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

    return make_unique<Simulation>(physicalObjects, CollisionType::INELASTIC, true, forceCalculationMethod, 0.5);
}

SimulationPointer_t
Simulations::bodyFormation_ArbitraryList(int numPieces, PhysicsSandboxProperties properties)
{
    ParticleList physicalObjects;  // I call functions on this below without ever initializing it first.... Scary.

    const kilograms_per_cubic_meter_t objectDensity = AstronomicalValues::DENSITY_SUN;
    const kilogram_t pieceMass = (properties.mass*1000.0)/numPieces;
    PhysicalVector target(1000, 0, 0, true);

    PhysicalVector newColor (1, 1, 1);

    srand ( time(NULL) );

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

    return make_unique<Simulation>(physicalObjects, CollisionType::INELASTIC, true, properties.forceCalculationMethod, properties.octreeTheta);
}

SimulationPointer_t Simulations::bodyFormationGeneric_ArbitraryList(PhysicsSandboxProperties properties,
                                                                    PhysicalVector target,
                                                                    PhysicalVector groupMomentum)
{
  ParticleList physicalObjects;

	kilograms_per_cubic_meter_t objectDensity = AstronomicalValues::DENSITY_SUN;
	kilogram_t pieceMass = properties.mass / properties.numShapes;

	srand ( time(NULL) );

    PhysicalVector newColor (1, 1, 1);

    shared_ptr<Particle> curShape;
    PhysicalVector startMomentumVec;
    PhysicalVector startPos;
	for (int i = 0; i < properties.numShapes; i++) {

		if (i % 2 == 0) {
            startMomentumVec = randomSplitBodyMomentum(pieceMass);
            startPos = randomPointInSphere(properties.sandboxWidth, target);
		}
		else {
            startMomentumVec = startMomentumVec.scaledBy(-1);
            startPos = startPos.scaledBy(-1);
		}

        curShape = make_shared<Particle>(
                startPos,
                pieceMass,
                startMomentumVec.plus(groupMomentum), // Apply general group momentum to individual pieces momentum
                objectDensity,
                newColor
        );


		//Check if being placed on previously created object
		while ( physicalObjects.hasConflictsWith( *curShape ) ) {
            PhysicalVector newPosAttempt = randomPointInSphere(properties.sandboxWidth, target);
			curShape->setPos(newPosAttempt);
		}
    physicalObjects.addShapeToList( curShape );
	}

    return make_unique<Simulation>(physicalObjects, CollisionType::INELASTIC, true, properties.forceCalculationMethod, properties.octreeTheta);
}

PhysicalVector Simulations::randomSplitBodyPlacement(float pieceRadius, PhysicalVector target) {
  int randMult;
  float largestDistance = 0;

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

  static bool switchB = false;
  double values[] = {0, 0, 0};

  if (switchB) {
    for (auto &value : values) {

      if (switchB) {
        // Set the range of momenta, and have them be half positive/half negative
        randMult = rand()%100;
        if (randMult % 2 == 0)
          randMult *= -1;
      } else {
        randMult *= -1;
      }
        value = randMult * pieceMass.value() * 0.001000; // Good mix
    }
  }


    switchB = !switchB;
    return PhysicalVector(values[0], values[1], values[2], false);
}

/*
function getPoint() {
    var x = Math.random() - 0.5;
    var y = Math.random() - 0.5;
    var z = Math.random() - 0.5;

    var mag = Math.sqrt(x*x + y*y + z*z);
    x /= mag; y /= mag; z /= mag;

    var d = Math.random();
    return {x: x*d, y: y*d, z: z*d};
}
 */



/*
function getPoint() {
    var u = Math.random();
    var v = Math.random();
    var theta = u * 2.0 * Math.PI;
    var phi = Math.acos(2.0 * v - 1.0);
    var r = Math.cbrt(Math.random());
    var sinTheta = Math.sin(theta);
    var cosTheta = Math.cos(theta);
    var sinPhi = Math.sin(phi);
    var cosPhi = Math.cos(phi);
    var x = r * sinPhi * cosTheta;
    var y = r * sinPhi * sinTheta;
    var z = r * cosPhi;
    return {x: x, y: y, z: z};
}
 */

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
    auto theta = u * 2.0f * M_PI;
    auto phi = acos(2.0f * (float)v - 1.0f);
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

