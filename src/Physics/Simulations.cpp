/*
 * Simulations.cpp
 *
 *  Created on: Jul 22, 2011
 *      Author: brasure
 */

#include "Simulations.h"

float astronomicalSideLength = 10e5; //Formation Value
float billiardsSideLength = 200;
float smallAstronomicalSideLength = 1.8e4;

void Simulations::simpleOrbit() {

	sgVec4 startPos = { 0, 0, 0, 1 };
  sgVec4 startMom = { 0, -2e-1, 0, 0 };

	float sunVolume = (AstronomicalValues::MASS_SUN*MASS_VAR)/(AstronomicalValues::DENSITY_SUN*CONVERSION_CONST);
	float sunRadius = getSplitBodyRadius(sunVolume, 1);


  shapePointer_t curShape;

	sgVec3 sunColor = { 255, 255, 0 };

    curShape = make_shared<Circle>(
            startPos,
            AstronomicalValues::MASS_SUN,
            startMom,
            AstronomicalValues::DENSITY_SUN,
            sunColor
    );

	startPos[0] = sunRadius * 214.94;

	sgNegateVec4(startMom);

    sgVec3 earthColor = { 165,42,42 };

    curShape = make_shared<Circle>(
            startPos,
            MASS_EARTH,
            startMom,
            DENSITY_EARTH,
            earthColor
    );
}

SimulationPointer_t Simulations::billiards1(int numRows, ForceCalculationMethod forceCalculationMethod) {
    ShapeList physicalObjects;

    float dt = 1.0;

    int numPieces = 0;
    float cueMass = 100.0;

    float ballMass = 0.156;
    float ballRadius = .95;

    for (int i = 1; i < numRows+1; i++)
        numPieces+=i;

    sgVec4 cueVelocity;
    cueVelocity[0] = 30;
    cueVelocity[1] = -75;
    cueVelocity[2] = 0;

    sgVec4 cueMomentum;
    sgScaleVec4(cueMomentum, cueVelocity, cueMass);

    sgVec3 newColor;
    newColor[0] = 1;
    newColor[1] = 1;
    newColor[2] = 1;

    shapePointer_t curShape;

    sgVec4 cuePos = { (float) numRows, (float) numRows*3, 0, 1};
    curShape = make_shared<Circle>(
            cuePos,
            cueMass,
            ballRadius,
            cueMomentum,
            newColor
    );

    physicalObjects.addShapeToList( curShape );

    sgVec4 ballMomentum = { 0, 0, 0, 0};
    int cutOff = numRows*2;
    for (int i = 1; i < numRows+1; i++) {
        for (int j = i; j < cutOff; j+= 2) {
            sgVec4 ballPos = { (float) (j* 1.7), (float) (i*2.5), 0, 1};

            curShape = make_shared<Circle>(
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
    return make_unique<Simulation>(physicalObjects, CollisionType::ELASTIC, dt, false, forceCalculationMethod);
}

SimulationPointer_t Simulations::billiards2_ReturnSimulation(int numRows, ForceCalculationMethod forceCalculationMethod)
{
    ShapeList physicalObjects;

    float dt = 1.0;

    float cueMass = 100.0;
    float ballMass = 0.156;

    float ballRadius = .95;

    sgVec4 cueVelocity = { 30, -75, 0 };
    sgVec4 cueMomentum;
    sgScaleVec4(cueMomentum, cueVelocity, cueMass);

    sgVec3 newColor = { 1, 0, 1 };

    shapePointer_t shapeForInsertion;

    sgVec4 cuePos = { (float) numRows, (float) numRows*3, 0, 1};
    shapeForInsertion = make_shared<Circle>(
            cuePos,
            cueMass,
            ballRadius,
            cueMomentum,
            newColor
    );
    physicalObjects.addShapeToList( shapeForInsertion );

    sgVec4 ballMomentum = { 0, 0, 0, 0};
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numRows; j++) {
            sgVec4 ballPos = { (float) j* 3, (float) i*3, 0, 1};
            newColor[1] = -( ( -.5 + (j/float(numRows)) ) * ( -.5 + (j/float(numRows)) ) )+ 1.0;

            shapeForInsertion = make_shared<Circle>(
                    ballPos,
                    ballMass,
                    ballRadius,
                    ballMomentum,
                    newColor
            );

            physicalObjects.addShapeToList( shapeForInsertion );
        }
    }
    return make_unique<Simulation>(physicalObjects, CollisionType::ELASTIC, dt, false, forceCalculationMethod);
}


SimulationPointer_t Simulations::billiards3_ArbitraryList(int numRows, ForceCalculationMethod forceCalculationMethod) {
    ShapeList physicalObjects;

    float dt = 1.0;

    float cueMass = 100.0;
    float ballMass = 0.156;

    float ballRadius = .95;

    sgVec4 cueVelocity = { 30, -75, 20 };
    sgVec4 cueMomentum;
    sgScaleVec4(cueMomentum, cueVelocity, cueMass);

    sgVec3 newColor = { 1, 1, 1 };

    shapePointer_t shapeForInsertion;

    sgVec4 cuePos = { (float) numRows, (float) numRows*5, 0, 1};
    shapeForInsertion = make_shared<Circle>(
            cuePos,
            cueMass,
            ballRadius,
            cueMomentum,
            newColor
    );
    physicalObjects.addShapeToList( shapeForInsertion );

    newColor[0] = 0;
    newColor[2] = 0;

    sgVec4 ballMomentum = { 0, 0, 0, 0};
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numRows; j++) {
            for (int z = 0; z < numRows; z++) {
                sgVec4 ballPos = { (float) (j* 4), (float) (i*4), (float) (z*4), 1};
                newColor[1] = -( ( -.5 + (z/float(numRows)) ) * ( -.5 + (z/float(numRows)) ) )+ 1.0;


                shapeForInsertion = make_shared<Circle>(
                        ballPos,
                        ballMass,
                        ballRadius,
                        ballMomentum,
                        newColor
                );

                physicalObjects.addShapeToList( shapeForInsertion );
            }
        }
    }
    return make_unique<Simulation>(physicalObjects, CollisionType::ELASTIC, dt, false, forceCalculationMethod);
}

SimulationPointer_t Simulations::disruption_ArbitraryList(ForceCalculationMethod forceCalculationMethod, float dt)
{
    SimulationPointer_t curSimulation = Simulations::bodyFormation_ArbitraryList(1000, forceCalculationMethod, dt);

    int numPieces = 1;
    float objectDensity = AstronomicalValues::DENSITY_SUN;
    float pieceMass = (AstronomicalValues::MASS_SUN)/numPieces;
    sgVec4 startMomentum = { 0, 0, 0 };
    sgVec3 newColor = { 1, 0, 1 };

    sgVec4 startPlacement = { -1e7 , 0, 0, 1};

    shapePointer_t curShape = make_shared<Circle>(
            startPlacement,
            pieceMass,
            startMomentum,
            objectDensity,
            newColor
    );

    ShapeList disruptingObject(curShape);
//    Simulation & sim = *curSimulation;
    // TODO actually use this, once I can prevent the initial sharedPtr from killing it when this function exits
//    Simulation disruptedSimulation(std::move(sim), disruptingObject);
    curSimulation->addPhysicalObjectToList( curShape );
    return curSimulation;
}

SimulationPointer_t Simulations::QuadrantTesting_simplest(ForceCalculationMethod forceCalculationMethod)
{
    ShapeList physicalObjects;

    int numPieces=2;
    float objectDensity = AstronomicalValues::DENSITY_SUN;
    float pieceMass = (AstronomicalValues::MASS_SUN)/(numPieces);
    sgVec4 startMomentum = { 0, 0, 0 };

    sgVec3 newColor = { 1, 1, 1 };

    shapePointer_t curShape;
    float d= 3e4;

    //#1
    sgVec4 object1Placement = {  (float) +(5/8.0 * d), (float) +(7/8.0 * d), 1, 1};

    physicalObjects.addShapeToList(
            make_shared<Circle>(
                    object1Placement,
                    pieceMass,
                    startMomentum,
                    objectDensity,
                    newColor
            )
            );

    //#2
    sgVec4 object2Placement = {  (float) -(7/8.0 * d), (float) +(7/8.0 * d), 1, 1};

    physicalObjects.addShapeToList(
            make_shared<Circle>(
                    object2Placement,
                    pieceMass,
                    startMomentum,
                    objectDensity,
                    newColor
            )
    );

    sgVec4 object3Placement = {  (float) -(1/8.0 * d), (float) -(5/8.0 * d), 1, 1};

    physicalObjects.addShapeToList(
            make_shared<Circle>(
                    object3Placement,
                    pieceMass,
                    startMomentum,
                    objectDensity,
                    newColor
            )
    );

    return make_unique<Simulation>(physicalObjects, CollisionType::INELASTIC, 1e4, true, forceCalculationMethod);
}

SimulationPointer_t
Simulations::bodyFormation_ArbitraryList(int numPieces, ForceCalculationMethod forceCalculationMethod, float dt)
{
    ShapeList physicalObjects;  // I call functions on this below without ever initializing it first.... Scary.

    const float objectDensity = AstronomicalValues::DENSITY_SUN;
    const float pieceMass = (AstronomicalValues::MASS_SUN )/(numPieces);
    const float pieceRadius = MyShape::calcRadius(pieceMass, objectDensity);
    sgVec4 startPlacement, startMomentum;
    sgVec4 target = { 1000, 0, 0, 1};

    sgVec3 newColor = { 1, 1, 1 };


    srand ( time(NULL) );

    for (int i = 0; i < numPieces; i++) {
        if (i % 2 == 0) {
            startMomentum[0]=0;startMomentum[1]=0;startMomentum[2]=0;
            randomSplitBodyMomentum(startMomentum, pieceMass);
            randomSplitBodyPlacement(startPlacement, pieceRadius, target);
        }
        else {
            sgNegateVec4(startMomentum);
            sgNegateVec4(startPlacement);
        }

        const shapePointer_t curShape = make_shared<Circle>(
                startPlacement,
                pieceMass,
                startMomentum,
                objectDensity,
                newColor
        );

        //Check if being placed on previously created object
        while ( physicalObjects.hasConflictsWith( *curShape ) ) {
            randomSplitBodyPlacement(startPlacement, pieceRadius, target);
            curShape->setPos( startPlacement );
        }
        physicalObjects.addShapeToList( curShape );
    }

    return make_unique<Simulation>(physicalObjects, CollisionType::INELASTIC, dt, true, forceCalculationMethod);
}

SimulationPointer_t Simulations::bodyFormationGeneric_ArbitraryList(int numPieces, float *target, float *groupMomentum,
                                                                    ForceCalculationMethod forceCalculationMethod)
{
  ShapeList physicalObjects;

	float objectDensity = AstronomicalValues::DENSITY_SUN;
	float pieceMass = (AstronomicalValues::MASS_SUN)/(numPieces);
	sgVec4 startPlacement, startMomentum;

	float pieceRadius = 100; // TOTALLY ARBITRARY VALUE!!
	pieceMass = pieceMass * (4.0/3.0) * M_PI * (objectDensity);

	srand ( time(NULL) );

    sgVec3 newColor;
    newColor[0] = 1;
    newColor[1] = 1;
    newColor[2] = 1;


    shapePointer_t curShape;
	for (int i = 0; i < numPieces; i++) {

		if (i % 2 == 0) {
			randomSplitBodyMomentum(startMomentum, pieceMass);
			randomSplitBodyPlacement(startPlacement, pieceRadius, target);
		}
		else {
			sgNegateVec4(startMomentum);
			sgNegateVec4(startPlacement);
		}

    // Apply general group momentum to individual pieces momentum
    sgAddVec4( startMomentum, groupMomentum );

        curShape = make_shared<Circle>(
                startPlacement,
                pieceMass,
                startMomentum,
                objectDensity,
                newColor
        );


		//Check if being placed on previously created object
		while ( physicalObjects.hasConflictsWith( *curShape ) ) {
			randomSplitBodyPlacement(startPlacement, curShape->getRadius(), target);
			curShape->setPos(startPlacement[0], startPlacement[1], startPlacement[2]);
		}
    physicalObjects.addShapeToList( curShape );
	}

    return make_unique<Simulation>(physicalObjects, CollisionType::INELASTIC, 1000, true, forceCalculationMethod);
}

SimulationPointer_t
Simulations::createSimulation(char simNumber, int numShapes, ForceCalculationMethod forceCalculationMethod, float dt)
{
	//******CURRENT SIMULATION*****
  if ( simNumber == '0' ) {
  } else if ( simNumber == '1' ) {
    return Simulations::bodyFormation_ArbitraryList(numShapes, forceCalculationMethod, dt);
  } else if ( simNumber == '2' ) {
	  return Simulations::disruption_ArbitraryList(forceCalculationMethod, dt);
  } else if ( simNumber == '3' ) {
  } else if ( simNumber == '4' ) {
    return Simulations::billiards1(15, forceCalculationMethod);
  } else if ( simNumber == '5' ) {
    return Simulations::billiards2_ReturnSimulation(15, forceCalculationMethod);
  } else if ( simNumber == '6' ) {
    sgVec4 groupMomentum;
    groupMomentum[0]=0;
    groupMomentum[1]=2800;
    groupMomentum[2]=0;
    groupMomentum[3]=1;
    sgVec4 target = { -2000, 0, 0, 1 };
      Simulations::bodyFormationGeneric_ArbitraryList(numShapes, target, groupMomentum, forceCalculationMethod);

    target[0]=-target[0];
    groupMomentum[1]*=-1;

      Simulations::bodyFormationGeneric_ArbitraryList(numShapes, target, groupMomentum, forceCalculationMethod);
  } else if ( simNumber == '7' ) {
      return Simulations::QuadrantTesting_simplest(forceCalculationMethod);
  } else if ( simNumber == '8' ) {
      return Simulations::billiards3_ArbitraryList(5, forceCalculationMethod);
  } else {
      exit(1);
  }

}

float Simulations::getSplitBodyRadius(float volume, int numPieces ) {
  float pieceVol = volume/numPieces;
  float pieceRadius = pieceVol/(M_PI * 4.0/3.0);
  pieceRadius = pow(pieceRadius, 1.0/3.0);

  return pieceRadius;
}

void Simulations::randomSplitBodyPlacement(sgVec4 startPos, float pieceRadius, sgVec4 target) {
  int randMult;
  float largestDistance = 0;

  for (int i = 0; i < 3; i++)
  {
    randMult = rand()%450;
    if (randMult % 2 == 0) {
      randMult *= -1;
    }
    startPos[i] = randMult * pieceRadius;
    if ( startPos[i] > largestDistance )
    {
      largestDistance = startPos[i];
    }
  }

  startPos[3] = 1;

  sgAddVec4( startPos, target );
}

void Simulations::randomSplitBodyPlacementInZone(sgVec4 startPos, sgVec4 volume, sgVec4 target ) {
  int randMult;
  int dimensionInteger;

  for (int i = 0; i < 3; i++)
  {
    dimensionInteger = (int) volume[i]/2;

    randMult = rand()%(dimensionInteger/2);
    if (randMult % 2 == 0) {
      randMult *= -1;
    }
    startPos[i] = randMult;
  }

  sgAddVec4( startPos, target );
}

void Simulations::randomSplitBodyMomentum(sgVec4 startMom, float pieceMass) {
  static int randMult;

  static bool switchB = false;

  if (switchB)
  {
    for (int i = 0; i < 3; i++) {

      if (switchB)
      {
        // Set the range of momenta, and have them be half positive/half negative
        randMult = rand()%3;
        if (randMult % 2 == 0)
          randMult *= -1;


      }
      else {
        randMult *= -1;
      }
      startMom[i] = randMult * pieceMass * 0.00050; // Good mix
    }
  }


  if (switchB) {
    switchB = false;
  }
  else {
    switchB = true;
  }
  startMom[3] = 0;
}

