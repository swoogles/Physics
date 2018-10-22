/*
 * Simulations.cpp
 *
 *  Created on: Jul 22, 2011
 *      Author: brasure
 */

#include "Simulations.h"

float Simulations::G = 6.67384e-11;
float astronomicalSideLength = 10e5; //Formation Value
float billiardsSideLength = 200;
float smallAstronomicalSideLength = 1.8e4;

void Simulations::simpleOrbit() {

	sgVec4 startPos = { 0, 0, 0, 1 };
  sgVec4 startMom = { 0, -2e-1, 0, 0 };
	float sunRadiusScale = 15;
	float earthRadiusScale = 220;

	float sunVolume = (MASS_SUN*MASS_VAR)/(DENSITY_SUN*CONVERSION_CONST);
	float sunRadius = getSplitBodyRadius(sunVolume, 1);

	float earthVolume = (MASS_EARTH*MASS_VAR)/(DENSITY_EARTH*CONVERSION_CONST);
	float earthRadius = getSplitBodyRadius(earthVolume, 1);

  shapePointer_t curShape;

	sgVec3 sunColor = { 255, 255, 0 };

    curShape = make_shared<Circle>(
            startPos,
            MASS_SUN,
            sunRadius * sunRadiusScale,
            startMom,
            DENSITY_SUN,
            sunColor
    );

	startPos[0] = sunRadius * 214.94;

	sgNegateVec4(startMom);

    sgVec3 earthColor = { 165,42,42 };

    curShape = make_shared<Circle>(
            startPos,
            MASS_EARTH,
            earthRadius*earthRadiusScale,
            startMom,
            DENSITY_EARTH,
            earthColor
    );
}

SimulationPointer_t Simulations::billiards1(int numRows) {
    ShapeList physicalObjects;

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

    float bogusDensity = 1; // TODO This should be calculated from radius & mass

    shapePointer_t curShape;

    sgVec4 cuePos = { (float) numRows, (float) numRows*3, 0, 1};
    curShape = make_shared<Circle>(
            cuePos,
            cueMass,
            ballRadius,
            cueMomentum,
            bogusDensity,
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
                    bogusDensity,
                    newColor
            );
            physicalObjects.addShapeToList( curShape );
        }
        cutOff--;
    }

    // return physicalObjects;
    SimulationPointer_t curSimulation = make_shared<Simulation>(physicalObjects, CollisionType::ELASTIC, 0, false);
    return curSimulation;
}

SimulationPointer_t Simulations::billiards2_ReturnSimulation(int numRows)
{
    ShapeList physicalObjects;

    float cueMass = 100.0;
    float ballMass = 0.156;

    float ballRadius = .95;

    sgVec4 cueVelocity = { 30, -75, 0 };
    sgVec4 cueMomentum;
    sgScaleVec4(cueMomentum, cueVelocity, cueMass);

    sgVec3 newColor = { 1, 0, 1 };

    shapePointer_t shapeForInsertion;
    float bogusDensity = 1; // TODO This should be calculated from radius & mass

    sgVec4 cuePos = { (float) numRows, (float) numRows*3, 0, 1};
    shapeForInsertion = make_shared<Circle>(
            cuePos,
            cueMass,
            ballRadius,
            cueMomentum,
            bogusDensity,
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
                    bogusDensity,
                    newColor
            );

            physicalObjects.addShapeToList( shapeForInsertion );
        }
    }

    SimulationPointer_t curSimulation = make_shared<Simulation>(physicalObjects, CollisionType::ELASTIC, 0, false);
    return curSimulation;
}


SimulationPointer_t Simulations::billiards3_ArbitraryList(int numRows) {
    ShapeList physicalObjects;

    float cueMass = 100.0;
    float ballMass = 0.156;

    float cueRadius = 4;
    float ballRadius = .95;

    sgVec4 cueVelocity = { 30, -75, 20 };
    sgVec4 cueMomentum;
    sgScaleVec4(cueMomentum, cueVelocity, cueMass);

    sgVec3 newColor = { 1, 1, 1 };
    float bogusDensity = 1; // TODO This should be calculated from radius & mass

    shapePointer_t shapeForInsertion;

    sgVec4 cuePos = { (float) numRows, (float) numRows*5, 0, 1};
    shapeForInsertion = make_shared<Circle>(
            cuePos,
            cueMass,
            ballRadius,
            cueMomentum,
            bogusDensity,
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
                        bogusDensity,
                        newColor
                );

                physicalObjects.addShapeToList( shapeForInsertion );
            }
        }
    }

    SimulationPointer_t curSimulation = make_shared<Simulation>(physicalObjects, CollisionType::ELASTIC, 0, false);
    return curSimulation;
}

SimulationPointer_t Simulations::disruption_ArbitraryList()
{
    SimulationPointer_t curSimulation = Simulations::bodyFormation_ArbitraryList( 1000 );

    int numPieces = 1;
    float objectDensity = DENSITY_SUN;
    float bodyVolume = (MASS_SUN)/(objectDensity)/3;
    float pieceRadius = getSplitBodyRadius(bodyVolume, numPieces);
    float pieceMass = pow(pieceRadius, 3.0);
    sgVec4 startMomentum = { 0, 0, 0 };
    sgVec3 newColor = { 1, 0, 1 };

    sgVec4 startPlacement = { -pieceRadius * 30, 0, 0, 1};

    shapePointer_t curShape = make_shared<Circle>(
            startPlacement,
            pieceMass,
            pieceRadius,
            startMomentum,
            objectDensity,
            newColor
    );

    ShapeList disruptingObject(curShape);
    Simulation & sim = *curSimulation;
    Simulation disruptedSimulation(std::move(sim), disruptingObject);
    curSimulation->addPhysicalObjectToList( curShape );
    return curSimulation;
}

SimulationPointer_t Simulations::bodyFormation_NonRandom() 
{
  float dt = 1000;
  int numPieces = 2;

  ShapeList physicalObjects;

	float objectDensity = DENSITY_SUN;
	float bodyVolume = (MASS_SUN * 10.0)/(objectDensity);
	float pieceRadius = getSplitBodyRadius(bodyVolume, numPieces);
	sgVec4 startPlacement;
	sgVec4 startMomentum = { 0, 0, 0 };

	sgVec3 newColor = { 1, 0, 1 };

	float pieceMass = pow(pieceRadius, 3.0);
  pieceMass = pieceMass * (4.0/3.0) * M_PI * (objectDensity);

  float totalMass = 0.0;

  shapePointer_t curShape;

  float offset = 8e4;
  startPlacement[0]= offset;
  startPlacement[1]= offset;
  startPlacement[2]= offset;
  startPlacement[3]= 0;

  shapePointer_t oldCircle = make_shared<Circle>(
            startPlacement,
            pieceMass,
            pieceRadius,
            startMomentum,
            objectDensity,
            newColor
    );

  physicalObjects.addShapeToList( oldCircle );

  startPlacement[0]= -offset;

    curShape = make_shared<Circle>(
            startPlacement,
            pieceMass,
            pieceRadius,
            startMomentum,
            objectDensity,
            newColor
    );

  physicalObjects.addShapeToList( curShape );
  totalMass += curShape->getMass();

  startPlacement[0]= -offset;

  startPlacement[0]= -offset;
  startPlacement[1]= -offset;


    SimulationPointer_t curSimulation = make_shared<Simulation>(physicalObjects, CollisionType::INELASTIC, dt, true);
  return curSimulation;
}

SimulationPointer_t Simulations::QuadrantTestingNonRandom()
{
    ShapeList physicalObjects;

    int numPieces=5;
    float objectDensity = DENSITY_SUN;
    float bodyVolume = (MASS_SUN)/(objectDensity);
    float pieceRadius = getSplitBodyRadius(bodyVolume, numPieces);
    sgVec4 startMomentum = { 0, 0, 0 };

    sgVec3 newColor = { 1, 1, 1 };

    float pieceMass = (pow(pieceRadius, 3.0) / 2) * (4.0/3.0) * M_PI * (objectDensity);

    shapePointer_t curShape;
    float d= 1.8e3;

    //#0
    sgVec4 startPlacement = {  (float) -(7/8.0 * d), + (float) (3/8.0 * d), 1, 1};
    curShape = make_shared<Circle>(
            startPlacement,
            pieceMass,
            pieceRadius,
            startMomentum,
            objectDensity,
            newColor
    );

    physicalObjects.addShapeToList( curShape );

    //#1
    sgVec4 object1Placement = {  (float) +(5/8.0 * d), (float) +(7/8.0 * d), 1, 1};
    curShape = make_shared<Circle>(
            object1Placement,
            pieceMass,
            pieceRadius,
            startMomentum,
            objectDensity,
            newColor
    );

    physicalObjects.addShapeToList( curShape );

    //#2
    sgVec4 object2Placement = {  (float) +(7/8.0 * d), (float) +(7/8.0 * d), 1, 1};
    curShape = make_shared<Circle>(
            object2Placement,
            pieceMass,
            pieceRadius,
            startMomentum,
            objectDensity,
            newColor
    );

    physicalObjects.addShapeToList( curShape );

    //#3
    sgVec4 object3Placement = {  (float) +(7/8.0 * d), (float) +(2/8.0 * d), 1, 1};
    curShape = make_shared<Circle>(
            object3Placement,
            pieceMass,
            pieceRadius,
            startMomentum,
            objectDensity,
            newColor
    );

    physicalObjects.addShapeToList( curShape );

    //#4
    sgVec4 object4Placement = {  (float) -(5/8.0 * d), (float) +(1/8.0 * d), 1, 1};
    curShape = make_shared<Circle>(
            object4Placement,
            pieceMass,
            pieceRadius,
            startMomentum,
            objectDensity,
            newColor
    );

    physicalObjects.addShapeToList( curShape );

    SimulationPointer_t curSimulation = make_shared<Simulation>(physicalObjects, CollisionType ::INELASTIC, 1000, true);
    return curSimulation;
}

SimulationPointer_t Simulations::QuadrantTesting_simplest()
{
    ShapeList physicalObjects;

    int numPieces=5;
    float objectDensity = DENSITY_SUN;
    float bodyVolume = (MASS_SUN)/(objectDensity);
    float pieceRadius = getSplitBodyRadius(bodyVolume, numPieces);
    sgVec4 startMomentum = { 0, 0, 0 };

    sgVec3 newColor = { 1, 1, 1 };

    float pieceMass = 1.0e10;
    shapePointer_t curShape;
    float d= 1.8e3;

    //#1
    sgVec4 object1Placement = {  (float) +(5/8.0 * d), (float) +(7/8.0 * d), 1, 1};

    physicalObjects.addShapeToList(
            make_shared<Circle>(
                    object1Placement,
                    pieceMass,
                    pieceRadius,
                    startMomentum,
                    objectDensity,
                    newColor
            )
            );

    //#2
    sgVec4 object2Placement = {  (float) +(7/8.0 * d), (float) +(7/8.0 * d), 1, 1};

    physicalObjects.addShapeToList(
            make_shared<Circle>(
                    object2Placement,
                    pieceMass,
                    pieceRadius,
                    startMomentum,
                    objectDensity,
                    newColor
            )
    );

    SimulationPointer_t curSimulation = make_shared<Simulation>(physicalObjects, CollisionType::INELASTIC, 1000, true);
    return curSimulation;
}

SimulationPointer_t Simulations::bodyFormation_ArbitraryList(int numPieces)
{
    ShapeList physicalObjects;  // I call functions on this below without ever initializing it first.... Scary.

    const float objectDensity = DENSITY_SUN;
    const float bodyVolume = (MASS_SUN * 10)/(objectDensity);
    const float pieceRadius = getSplitBodyRadius(bodyVolume, numPieces);
    sgVec4 startPlacement, startMomentum;
    sgVec4 target = { 1000, 0, 0, 1};

    sgVec3 newColor = { 1, 1, 1 };

    const float pieceMass = pow(pieceRadius, 3.0) * (4.0/3.0) * M_PI * (objectDensity);

    float totalMass = 0.0;

    srand ( time(NULL) );

    for (int i = 0; i < numPieces; i++) {
        shapePointer_t curShape;

        if (i % 2 == 0) {
            startMomentum[0]=0;startMomentum[1]=0;startMomentum[2]=0;
            randomSplitBodyMomentum(startMomentum, pieceMass);
            randomSplitBodyPlacement(startPlacement, pieceRadius, target);
        }
        else {
            sgNegateVec4(startMomentum);
            sgNegateVec4(startPlacement);
        }

        curShape = make_shared<Circle>(
                startPlacement,
                pieceMass,
                pieceRadius,
                startMomentum,
                objectDensity,
                newColor
        );

        //Check if being placed on previously created object
        while ( physicalObjects.hasConflictsWith( curShape ) ) {
            randomSplitBodyPlacement(startPlacement, pieceRadius, target);
            curShape->setPos( startPlacement );
        }
        physicalObjects.addShapeToList( curShape );

        totalMass += curShape->getMass();
    }

    const SimulationPointer_t curSimulation = make_shared<Simulation>(physicalObjects, CollisionType::INELASTIC, 1000, true);
    return curSimulation;
}

SimulationPointer_t Simulations::bodyFormationGeneric_ArbitraryList(int numPieces, sgVec4 target, sgVec4 groupMomentum) 
{
  ShapeList physicalObjects;

	float objectDensity = DENSITY_SUN;
	float bodyVolume = (MASS_SUN)/(objectDensity);
	float pieceRadius = getSplitBodyRadius(bodyVolume, numPieces);
	sgVec4 startPlacement, startMomentum;

	float pieceMass = pow(pieceRadius, 3.0);
	pieceMass = pieceMass * (4.0/3.0) * M_PI * (objectDensity);

	float totalMass = 0.0;

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
                pieceRadius,
                startMomentum,
                objectDensity,
                newColor
        );


		//Check if being placed on previously created object
		while ( physicalObjects.hasConflictsWith( curShape ) ) {
			randomSplitBodyPlacement(startPlacement, pieceRadius, target);
			curShape->setPos(startPlacement[0], startPlacement[1], startPlacement[2]);
		}
    physicalObjects.addShapeToList( curShape );
		totalMass += pieceMass;
	}

    SimulationPointer_t curSimulation = make_shared<Simulation>(physicalObjects, CollisionType::INELASTIC, 1000, true);
  return curSimulation;
}

SimulationPointer_t Simulations::createSimulation( char simNumber, int numShapes, ForceCalculationMethod forceCalculationMethod)
{
  SimulationPointer_t newSimulation;

	//******CURRENT SIMULATION*****
  if ( simNumber == '0' ) {
	  newSimulation = Simulations::bodyFormation_NonRandom();
  }
  if ( simNumber == '1' ) {
    newSimulation = Simulations::bodyFormation_ArbitraryList( numShapes );
  }
  if ( simNumber == '2' ) {
	  newSimulation = Simulations::disruption_ArbitraryList();
  }
  if ( simNumber == '3' ) {
    newSimulation = Simulations::QuadrantTestingNonRandom();
  }
  if ( simNumber == '4' ) {
    newSimulation = Simulations::billiards1(15);
  }
  if ( simNumber == '5' ) {
    newSimulation = Simulations::billiards2_ReturnSimulation(15);
  }

  if ( simNumber == '6' ) {
    sgVec4 groupMomentum;
    groupMomentum[0]=0;
    groupMomentum[1]=2800;
    groupMomentum[2]=0;
    groupMomentum[3]=1;
    sgVec4 target = { -2000, 0, 0, 1 };
	  Simulations::bodyFormationGeneric_ArbitraryList( numShapes, target, groupMomentum );

    target[0]=-target[0];
    groupMomentum[1]*=-1;

	  Simulations::bodyFormationGeneric_ArbitraryList( numShapes, target, groupMomentum );
  }
  if ( simNumber == '7' ) {
      newSimulation = Simulations::QuadrantTesting_simplest();
  }

  if ( simNumber == '8' ) {
  }

  if ( simNumber == '9' ) {
    newSimulation = Simulations::billiards3_ArbitraryList( 5 );
  }

  newSimulation->setForceCalcMethod(forceCalculationMethod);

  return newSimulation;
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
    randMult = rand()%50;
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
        randMult = rand()%15;
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

