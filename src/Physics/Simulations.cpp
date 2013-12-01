/*
 * Simulations.cpp
 *
 *  Created on: Jul 22, 2011
 *      Author: brasure
 */

#include "Simulations.h"

compressed_vector< shared_ptr<MyShape> > Simulations::physicalObjects(0);
int Simulations::curStep;

using namespace std;

void testAutoScale() {
	int numObjects = 5;
	for (int i = 0; i < numObjects; i++ ) {

	}
}


void Simulations::largeGridAlternating() {
	sgVec4 startPos;

	int gridSide = 18;

  shape_pointer curShape;

	for (int i = 0; i < gridSide; i++)
	{
		for (int j = 0; j < gridSide; j++) {
      //int curIndex = i*gridSide + j;
			if ( (i*gridSide + j) % 2 == 0) {
				curShape = make_shared<Circle>();
				startPos[2] = 5;

			}
			else {
				curShape = make_shared<Box>();
				startPos[2] = -5;

			}
			startPos[0] = -gridSide + 2*j;
			startPos[1] = -gridSide + 2*i;
			startPos[3] = 1;

			curShape->setPos(startPos);
			curShape->setMass(.6e10);
      MyShape::addShapeToList( curShape );
		}
	}
}

void Simulations::simpleOrbit() {

	sgVec4 startPos, startMom;
	float sunRadiusScale = 15;
	float earthRadiusScale = 220;

	float sunDensity = DENSITY_SUN;
	float sunMass = MASS_SUN;
	float sunVolume = (sunMass*MASS_VAR)/(sunDensity*CONVERSION_CONST);
	float sunRadius = getSplitBodyRadius(sunVolume, 1);


	float earthDensity = DENSITY_EARTH;
	float earthMass = MASS_EARTH;
	float earthVolume = (earthMass*MASS_VAR)/(earthDensity*CONVERSION_CONST);
	float earthRadius = getSplitBodyRadius(earthVolume, 1);

	startMom[0] = 0;
	startMom[1] = -2e-1;//-2.9e2; //-2.9 * earthMass;
	startMom[2] = 0;
	startMom[3] = 0;

	startPos[0] = 0;
	startPos[1] = 0;
	startPos[2] = 0;
	startPos[3] = 1;

  shape_pointer curShape;

	curShape = make_shared<Circle>();
	curShape->setPos(startPos);
	curShape->setRadius(sunRadius * sunRadiusScale);
	curShape->setMomentum(startMom);
	curShape->setDensity(sunDensity);
	curShape->setMass(sunMass);

	startPos[0] = sunRadius * 214.94;
	startPos[1] = 0;
	startPos[2] = 0;
	startPos[3] = 1;

	sgNegateVec4(startMom);

	curShape = make_shared<Circle>();
	curShape->setPos(startPos);
	curShape->setRadius(earthRadius*earthRadiusScale);
	curShape->setMomentum(startMom);
	curShape->setDensity(earthDensity);
	curShape->setMass(earthMass);

	cout << "earthMass: " << curShape->getMass() << endl;

	cout << "numBodies: " << MyShape::shapes.size() << endl;
}

  shared_ptr<Simulation> Simulations::billiards1(int numRows) {
	WorldSettings::setDT(.003);
	WorldSettings::makeAllElastic();
	WorldSettings::setGravBetweenObjects(false);
	WorldSettings::setConstGravField(false);
	WorldSettings::setTimeElapsed(0);

  shared_ptr<Simulation> curSimulation = make_shared<Simulation>();
  curSimulation->setDT(0);
  curSimulation->setTimeElapsed(0);
  ShapeList physicalObjects;

	sgVec4 gField;
	gField[0] = -.2; gField[1] = 0; gField[2] = 0;

	WorldSettings::setConstGravFieldVal(gField);

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

	sgVec3 newColor;
	newColor[0] = 1;
	newColor[1] = 1;
	newColor[2] = 1;

  shape_pointer curShape;

	curShape = make_shared<Circle>();
	curShape->setPos(numRows, numRows*3, 0);
	curShape->setMass(cueMass);
	curShape->setRadius(ballRadius);
	curShape->setVelocity(cueVelocity);
	curShape->setColor(newColor);

  physicalObjects.addShapeToList( curShape );

	int cutOff = numRows*2;
	for (int i = 1; i < numRows+1; i++) {
		for (int j = i; j < cutOff; j+= 2) {
			curShape = make_shared<Circle>();
			curShape->setPos(j*1.7, i*2.5, 0);
			curShape->setMass(ballMass);
			curShape->setRadius(ballRadius);
      physicalObjects.addShapeToList( curShape );
		}
		cout << endl;
		cutOff--;
	}

	cout << "NumPieces: " << numPieces << endl;

  // return physicalObjects;
  curSimulation->setPhysicalObjects( physicalObjects );
  return curSimulation;
}

shared_ptr<Simulation> Simulations::billiards2_ReturnSimulation(int numRows)
{
	WorldSettings::setDT(.003);
	WorldSettings::makeAllElastic();
	//WorldSettings::makeAllInelastic();
	WorldSettings::setGravBetweenObjects(false);
	WorldSettings::setConstGravField(false);
	WorldSettings::setTimeElapsed(0);

  shared_ptr<Simulation> curSimulation = make_shared<Simulation>();
  curSimulation->setDT(.003);
  curSimulation->setTimeElapsed(0);
  ShapeList physicalObjects;
  
	sgVec4 gField;
	gField[0] = -.2; gField[1] = 0; gField[2] = 0;

	WorldSettings::setConstGravFieldVal(gField);

	int numPieces = 0;
	float cueMass = 100.0;
	float ballMass = 0.156;

	float ballRadius = .95;

  numPieces= numRows*numRows;

	sgVec4 cueVelocity;
	cueVelocity[0] = 30;
	//cueVelocity[1] = -15.05;
	cueVelocity[1] = -75;
	cueVelocity[2] = 0;

	sgVec3 newColor;
	newColor[0] = 1;
	newColor[1] = 0;
	newColor[2] = 1;
    
  shape_pointer shapeForInsertion;

	shapeForInsertion = make_shared<Circle>();
	shapeForInsertion->setPos(numRows, numRows*3, 0);
	shapeForInsertion->setMass(cueMass);
	shapeForInsertion->setRadius(ballRadius);
	shapeForInsertion->setVelocity(cueVelocity);
	shapeForInsertion->setColor(newColor);
  physicalObjects.addShapeToList( shapeForInsertion );

	// MyShape::shapes.resize(MyShape::shapes.size() + numPieces );

	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numRows; j++) {
			shapeForInsertion = make_shared<Circle>();
			shapeForInsertion->setPos(j*3, i*3, 0);
			shapeForInsertion->setMass(ballMass);
			shapeForInsertion->setRadius(ballRadius);
      newColor[1] = -( ( -.5 + (j/float(numRows)) ) * ( -.5 + (j/float(numRows)) ) )+ 1.0;
      shapeForInsertion->setColor(newColor);

      physicalObjects.addShapeToList( shapeForInsertion );
		}
		cout << endl;
	}

	cout << "NumPieces: " << numPieces << endl;

  curSimulation->setPhysicalObjects( physicalObjects );
  return curSimulation;

}


  shared_ptr<Simulation> Simulations::billiards3_ArbitraryList(int numRows) {
	WorldSettings::setDT(.003);
	WorldSettings::makeAllElastic();
	//WorldSettings::makeAllInelastic();
	WorldSettings::setGravBetweenObjects(false);
	WorldSettings::setConstGravField(false);
	WorldSettings::setTimeElapsed(0);

  shared_ptr<Simulation> curSimulation = make_shared<Simulation>();
  curSimulation->setDT(.003);
  curSimulation->setTimeElapsed(0);
  ShapeList physicalObjects;

	sgVec4 gField;
	gField[0] = -.2; gField[1] = 0; gField[2] = 0;

	WorldSettings::setConstGravFieldVal(gField);

	int numPieces = 0;
	float cueMass = 100.0;
	float ballMass = 0.156;

	float cueRadius = 4;
	float ballRadius = .95;

  numPieces= numRows*numRows*numRows;

	sgVec4 cueVelocity;
	cueVelocity[0] = 30;
	cueVelocity[1] = -75;
	cueVelocity[2] = 20;

	sgVec3 newColor;
	newColor[0] = 1;
	newColor[1] = 1;
	newColor[2] = 1;

  shape_pointer shapeForInsertion;

	shapeForInsertion = make_shared<Circle>();
	shapeForInsertion->setPos(numRows, numRows*5, 0);
	shapeForInsertion->setMass(cueMass);
	shapeForInsertion->setRadius(cueRadius);
	shapeForInsertion->setVelocity(cueVelocity);
	shapeForInsertion->setColor(newColor);
  physicalObjects.addShapeToList( shapeForInsertion );

	newColor[0] = 0;
	newColor[2] = 0;

	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numRows; j++) {
      for (int z = 0; z < numRows; z++) {

			shapeForInsertion = make_shared<Circle>();
			shapeForInsertion->setPos(j*4, i*4, z*4);
			shapeForInsertion->setMass(ballMass);
			shapeForInsertion->setRadius(ballRadius);
      newColor[1] = -( ( -.5 + (z/float(numRows)) ) * ( -.5 + (z/float(numRows)) ) )+ 1.0;
      shapeForInsertion->setColor(newColor);

      physicalObjects.addShapeToList( shapeForInsertion );
      }
		}
		cout << endl;
	}

	cout << "NumPieces: " << numPieces << endl;

  // return physicalObjects;
  curSimulation->setPhysicalObjects( physicalObjects );
  return curSimulation;
}

shared_ptr<Quadrant> Simulations::octreeDemonstration(int numRows) 
{
  WorldSettings::setDT(1000);
  WorldSettings::makeAllElastic();
  //WorldSettings::makeAllInelastic();
  WorldSettings::setGravBetweenObjects(true);
  WorldSettings::setConstGravField(false);
	// WorldSettings::setAutoScaling(true);
	WorldSettings::setTimeElapsed(0);
	WorldSettings::setTotalMass(0);

  sgVec4 gField;
  gField[0] = -.2; gField[1] = 0; gField[2] = 0;

  WorldSettings::setConstGravFieldVal(gField);

  float width = 250;
  float height = 250;
  float depth = 250;

  sgVec4 pos;
  pos[0] = 0;
  pos[1] = 0;
  pos[2] = 0;
  pos[3] = 1;

  sgVec4 target;
  target[0]=0;
  target[1]=0;
  target[2]=0;
  target[3]=1;

  sgVec3 dimensions;
  dimensions[0] = width;
  dimensions[1] = height;
  dimensions[2] = depth;

  shared_ptr<Quadrant> mainQuadrant = make_shared<Quadrant>( 4, 1, boost::ref(pos), boost::ref(dimensions) ) ;

	sgVec4 startPlacement;
  startPlacement[0] = 15;
  startPlacement[1] = 20;
  startPlacement[0] = 15;

  int curShapeIdx;
  shape_pointer curShapeInsert;

	float totalMass = 0.0;

  sgVec3 curShapeColor;
  curShapeColor[0]=1.0;
  curShapeColor[1]=1.0;
  curShapeColor[2]=1.0;

  for (int i = 0; i < 15; i++)
  {
    curShapeIdx = MyShape::shapes.size();
    randomSplitBodyPlacementInZone(startPlacement, dimensions, target);
    curShapeInsert = make_shared<Circle>();
    curShapeInsert->setPos( startPlacement );
    curShapeInsert->setColor( curShapeColor );
    curShapeInsert->setMass(1);
    curShapeInsert->setRadius(.5);

    MyShape::addShapeToList( curShapeInsert );
    while ( isConflict( curShapeIdx ) )
    {
      randomSplitBodyPlacementInZone(startPlacement, dimensions, target);
      curShapeInsert->setPos( startPlacement );
    }
    mainQuadrant->insertShape( curShapeInsert );
  }

  WorldSettings::adjustTotalMass( totalMass );

  dimensions[0] = width*2;
  dimensions[1] = height*2;
  dimensions[2] = depth*2;
  // mainQuadrant = make_shared<Quadrant>( Quadrant(4, 1, pos, dimensions) );

  return mainQuadrant;
}


shared_ptr<Simulation> Simulations::simpleCollision_ArbitraryList() {
  float dt = .01;
	WorldSettings::setDT( dt );

	//float objectDensity = 5;
	float aMass = 1;
	float bMass = 1;
	float aRadius = 1;
	float bRadius = 1;

	sgVec4 startPlacement, startAngMom;

	// Object A
	startPlacement[0] = -1;
	startPlacement[1] = -6;
	startPlacement[2] = 0;
	startPlacement[3] = 1;

	startAngMom[0] = 0;
	startAngMom[1] = 0;
	startAngMom[2] = 0;

  ShapeList physicalObjects;

  shared_ptr<Simulation> curSimulation = make_shared<Simulation>();
  curSimulation->setDT( dt );
  curSimulation->setTimeElapsed(0);
  shape_pointer curShape;

	curShape = make_shared<Circle>();
	curShape->setPos(startPlacement);
	curShape->setMass(aMass);
	curShape->setRadius(aRadius*2);
	curShape->setVelocity(0, 25, 0);
	curShape->setAngVelocity(startAngMom);
  // MyShape::addShapeToList( curShape );
  physicalObjects.addShapeToList( curShape );

	// Object B
	startPlacement[0] = 2.5;
	//startPlacement[1] = -.9; //45 Degree placement
	startPlacement[1] = 0;
	startPlacement[2] = 0;

	curShape = make_shared<Circle>();
	curShape->setPos(startPlacement);
	curShape->setMass(bMass);
	curShape->setRadius(bRadius*2);
	curShape->setVelocity(0, -25, 0);
  // MyShape::addShapeToList( curShape );
  physicalObjects.addShapeToList( curShape );

  // return physicalObjects;
  curSimulation->setPhysicalObjects( physicalObjects );
  return curSimulation;

}

shared_ptr<Simulation> Simulations::disruption_ArbitraryList() {
  
  // ShapeList physicalObjects = Simulations::bodyFormation_ArbitraryList( 1000 );
  shared_ptr<Simulation> curSimulation = Simulations::bodyFormation_ArbitraryList( 1000 );
  // curSimulation->setPhysicalObjects( physicalObjects );

  int numPieces = 1;
	float objectDensity = DENSITY_SUN;
	float bodyVolume = (MASS_SUN)/(objectDensity)/3;
	float pieceRadius = getSplitBodyRadius(bodyVolume, numPieces);
	float pieceMass = pow(pieceRadius, 3.0);
	sgVec4 startMomentum;
  startMomentum[0] = pieceMass/35 ; 
  startMomentum[1] = 0;
  startMomentum[2] = 0;

  shape_pointer curShape = make_shared<Circle>();
  curShape->setPos(-pieceRadius * 30, 0, 0);
  curShape->setMass(pieceMass);
  curShape->setRadius(pieceRadius);
  curShape->setMomentum(startMomentum);
  curShape->setDensity(objectDensity);

  // physicalObjects.addShapeToList( curShape );

  // return physicalObjects;
  curSimulation->addPhysicalObjectToList( curShape );
  return curSimulation;
}

shared_ptr<Simulation> Simulations::bodyFormation_NonRandom() {
  float dt = 1000;
	WorldSettings::setDT( dt );
	WorldSettings::makeAllInelastic();
	WorldSettings::setGravBetweenObjects(true);
	WorldSettings::setConstGravField(false);
	WorldSettings::setAutoScaling(true);
	WorldSettings::setTimeElapsed(0);
	WorldSettings::setTotalMass(0);

  int numPieces = 2;

  shared_ptr<Simulation> curSimulation = make_shared<Simulation>();
  curSimulation->setDT( dt );
  curSimulation->setTimeElapsed(0);
  ShapeList physicalObjects;

	float objectDensity = DENSITY_SUN;
	float bodyVolume = (MASS_SUN * 10.0)/(objectDensity);
	float pieceRadius = getSplitBodyRadius(bodyVolume, numPieces);
	sgVec4 startPlacement, startMomentum;

	sgVec3 newColor;
	newColor[0] = 1;
	newColor[1] = 0;
	newColor[2] = 1;

  startMomentum[0] = 0;
  startMomentum[1] = 0;
  startMomentum[2] = 0;

	float pieceMass = pow(pieceRadius, 3.0);
  pieceMass = pieceMass * (4.0/3.0) * M_PI * (objectDensity);

  float totalMass = 0.0;

  shape_pointer curShape;
  float offset = 8e2;
  startPlacement[0]= offset;
  startPlacement[1]= offset;
  startPlacement[2]= 0;
  startPlacement[3]= 0;

  curShape = make_shared<Circle>();
  curShape = make_shared<Circle>();
  curShape->setPos( startPlacement );
  curShape->setMass(pieceMass);
  curShape->setRadius(pieceRadius);
  curShape->setMomentum(startMomentum);
  curShape->setDensity(objectDensity);
  curShape->setColor(newColor);

  physicalObjects.addShapeToList( curShape );
  totalMass += curShape->getMass();

  startPlacement[0]= -offset;
  // startPlacement[1]= -offset;

  curShape = make_shared<Circle>();
  // curShape = make_shared<Circle>();
  curShape->setPos( startPlacement );
  curShape->setMass(pieceMass);
  curShape->setRadius(pieceRadius);
  curShape->setMomentum(startMomentum);
  curShape->setDensity(objectDensity);
  curShape->setColor(newColor);

  physicalObjects.addShapeToList( curShape );
  totalMass += curShape->getMass();

  startPlacement[0]= -offset;
  startPlacement[1]= -offset;

  curShape = make_shared<Circle>();
  curShape->setPos( startPlacement );
  curShape->setMass(pieceMass);
  curShape->setRadius(pieceRadius);
  curShape->setMomentum(startMomentum);
  curShape->setDensity(objectDensity);
  curShape->setColor(newColor);

  // physicalObjects.addShapeToList( curShape );
  totalMass += curShape->getMass();

  WorldSettings::adjustTotalMass( totalMass );

  curSimulation->setPhysicalObjects( physicalObjects );
  return curSimulation;
}

shared_ptr<Simulation> Simulations::QuadrantTestingNonRandom() {
	WorldSettings::setDT(1000);
	WorldSettings::makeAllInelastic();
	WorldSettings::setGravBetweenObjects(true);
	WorldSettings::setConstGravField(false);
	WorldSettings::setAutoScaling(true);
	WorldSettings::setTimeElapsed(0);
	WorldSettings::setTotalMass(0);

  shared_ptr<Simulation> curSimulation = make_shared<Simulation>();
  curSimulation->setDT(1000);
  curSimulation->setTimeElapsed(0);
  ShapeList physicalObjects;

  int numPieces=5;
	float objectDensity = DENSITY_SUN;
	float bodyVolume = (MASS_SUN)/(objectDensity);
	float pieceRadius = getSplitBodyRadius(bodyVolume, numPieces);
	sgVec4 startMomentum;
  startMomentum[0]=0;startMomentum[1]=0;startMomentum[2]=0;

	sgVec3 newColor;
	newColor[0] = 1;
	newColor[1] = 1;
	newColor[2] = 1;

	float pieceMass = pow(pieceRadius, 3.0);
	pieceMass = pieceMass * (4.0/3.0) * M_PI * (objectDensity);

	float totalMass = 0.0;

  shape_pointer curShape;
  float d= 1e5;

  //#0
  curShape = make_shared<Circle>();
  curShape->setPos( -(7/8.0 * d), +(3/8.0 * d), 1 );
  curShape->setMass(pieceMass);
  curShape->setRadius(pieceRadius);
  curShape->setMomentum(startMomentum);
  curShape->setDensity(objectDensity);
  curShape->setColor(newColor);

  physicalObjects.addShapeToList( curShape );
  totalMass += curShape->getMass();

  //#1
  curShape = make_shared<Circle>();
  curShape->setPos( +(5/8.0 * d), +(7/8.0 * d), 1 );
  curShape->setMass(pieceMass);
  curShape->setRadius(pieceRadius);
  curShape->setMomentum(startMomentum);
  curShape->setDensity(objectDensity);
  curShape->setColor(newColor);

  physicalObjects.addShapeToList( curShape );
  totalMass += curShape->getMass();

  //#2
  curShape = make_shared<Circle>();
  curShape->setPos( +(7/8.0 * d), +(7/8.0 * d), 1 );
  curShape->setMass(pieceMass);
  curShape->setRadius(pieceRadius);
  curShape->setMomentum(startMomentum);
  curShape->setDensity(objectDensity);
  curShape->setColor(newColor);

  physicalObjects.addShapeToList( curShape );
  totalMass += curShape->getMass();

  //#3
  curShape = make_shared<Circle>();
  curShape->setPos( +(7/8.0 * d), +(5/8.0 * d), 1 );
  curShape->setMass(pieceMass);
  curShape->setRadius(pieceRadius);
  curShape->setMomentum(startMomentum);
  curShape->setDensity(objectDensity);
  curShape->setColor(newColor);

  physicalObjects.addShapeToList( curShape );
  totalMass += curShape->getMass();

  //#4
  curShape = make_shared<Circle>();
  curShape->setPos( -(5/8.0 * d), +(1/8.0 * d), 1 );
  curShape->setMass(pieceMass);
  curShape->setRadius(pieceRadius);
  curShape->setMomentum(startMomentum);
  curShape->setDensity(objectDensity);
  curShape->setColor(newColor);

  physicalObjects.addShapeToList( curShape );
  totalMass += curShape->getMass();
  WorldSettings::adjustTotalMass( totalMass );

  curSimulation->setPhysicalObjects( physicalObjects );
  return curSimulation;
}

shared_ptr<Simulation> Simulations::bodyFormation_ArbitraryList(int numPieces) {
	WorldSettings::setDT(1000);
	WorldSettings::makeAllInelastic();
	WorldSettings::setGravBetweenObjects(true);
	WorldSettings::setConstGravField(false);
	WorldSettings::setAutoScaling(true);
	WorldSettings::setTimeElapsed(0);
	WorldSettings::setTotalMass(0);

  shared_ptr<Simulation> curSimulation = make_shared<Simulation>();
  curSimulation->setDT(1000);
  curSimulation->setTimeElapsed(0);
  ShapeList physicalObjects;

	float objectDensity = DENSITY_SUN;
	float bodyVolume = (MASS_SUN * 10)/(objectDensity);
	float pieceRadius = getSplitBodyRadius(bodyVolume, numPieces);
	sgVec4 startPlacement, startMomentum, target;

  target[0]=-1000;
  target[1]=0;
  target[2]=0;
  target[3]=1;

	sgVec3 newColor;
	newColor[0] = 1;
	newColor[1] = 1;
	newColor[2] = 1;

	float pieceMass = pow(pieceRadius, 3.0);
	pieceMass = pieceMass * (4.0/3.0) * M_PI * (objectDensity);

	float totalMass = 0.0;

	srand ( time(NULL) );

	for (int i = 0; i < numPieces; i++) {
    shape_pointer curShape;

		if (i % 2 == 0) {
      startMomentum[0]=0;startMomentum[1]=0;startMomentum[2]=0;
			randomSplitBodyMomentum(startMomentum, pieceMass);
			randomSplitBodyPlacement(startPlacement, pieceRadius, target);
		}
		else {
			sgNegateVec4(startMomentum);
			sgNegateVec4(startPlacement);
		}

    curShape = make_shared<Circle>();
		// curShape = make_shared<Circle>();
    curShape->setPos( startPlacement );
		curShape->setMass(pieceMass);
		curShape->setRadius(pieceRadius);
		curShape->setMomentum(startMomentum);
		curShape->setDensity(objectDensity);
    curShape->setColor(newColor);

    physicalObjects.addShapeToList( curShape );
		//Check if being placed on previously created object
		while ( isConflict_ArbitraryList(physicalObjects.getShapes(), i) ) {
			randomSplitBodyPlacement(startPlacement, pieceRadius, target);
      curShape->setPos( startPlacement );
		}

		totalMass += curShape->getMass();
	}
  WorldSettings::adjustTotalMass( totalMass );

  curSimulation->setPhysicalObjects( physicalObjects );
  return curSimulation;
}

shared_ptr<Simulation> Simulations::bodyFormationGeneric_ArbitraryList(int numPieces, sgVec4 target, sgVec4 groupMomentum) {
	WorldSettings::setDT(1000);
	WorldSettings::makeAllInelastic();
	WorldSettings::setGravBetweenObjects(true);
	WorldSettings::setConstGravField(false);
	WorldSettings::setAutoScaling(true);
	WorldSettings::setTimeElapsed(0);
	WorldSettings::setTotalMass(0);

  shared_ptr<Simulation> curSimulation = make_shared<Simulation>();
  curSimulation->setDT(.003);
  curSimulation->setTimeElapsed(0);
  ShapeList physicalObjects;

	float objectDensity = DENSITY_SUN;
	float bodyVolume = (MASS_SUN)/(objectDensity);
	float pieceRadius = getSplitBodyRadius(bodyVolume, numPieces);
	sgVec4 startPlacement, startMomentum;

	float pieceMass = pow(pieceRadius, 3.0);
	pieceMass = pieceMass * (4.0/3.0) * M_PI * (objectDensity);

	float totalMass = 0.0;

	srand ( time(NULL) );

  shape_pointer curShape;
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

		curShape = make_shared<Circle>();
    cout << "StartPos: " << startPlacement[0] << endl;
		curShape->setPos(startPlacement[0], startPlacement[1], startPlacement[2]);
		curShape->setMass(pieceMass);
		curShape->setRadius(pieceRadius);
		curShape->setMomentum(startMomentum);
		curShape->setDensity(objectDensity);

    physicalObjects.addShapeToList( curShape );

		//Check if being placed on previously created object
		while ( isConflict(i) ) {
			randomSplitBodyPlacement(startPlacement, pieceRadius, target);
			curShape->setPos(startPlacement[0], startPlacement[1], startPlacement[2]);
		}
		totalMass += pieceMass;
	}
  WorldSettings::adjustTotalMass( totalMass );

  // return physicalObjects;
  curSimulation->setPhysicalObjects( physicalObjects );
  return curSimulation;
}

shared_ptr<MyShape> Simulations::getShapeFromList( unsigned int shapeIndex )
{
  shape_pointer returnShape;
  if ( physicalObjects.size() > shapeIndex )
  {
    returnShape = physicalObjects(shapeIndex);
  }

  return returnShape;
}

int Simulations::addShapeToList( shape_pointer insertShape )
{
  int curSize = physicalObjects.size();
  physicalObjects.resize(curSize + 1);
  physicalObjects(curSize) = insertShape;
  return curSize;
}

shared_ptr<Simulation> Simulations::createSimulation( char simNumber )
// shared_ptr<Simulation> Simulations::createSimulation( char simNumber )
{
  shared_ptr<Simulation> newSimulation;

	//******CURRENT SIMULATION*****
  if ( simNumber == '0' ) {
    // Simulations::largeGridAlternating();
	  newSimulation = Simulations::bodyFormation_NonRandom();
  }
  if ( simNumber == '1' ) {
    newSimulation = Simulations::bodyFormation_ArbitraryList( 500 );
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
    sgVec4 target;
    target[0]=-2000;
    target[1]=0;
    target[2]=0;
    target[3]=1;
	  Simulations::bodyFormationGeneric_ArbitraryList( 650, target, groupMomentum );

    target[0]=-target[0];
    groupMomentum[0]=0;
    groupMomentum[1]=-2800;
    groupMomentum[2]=0;

	  Simulations::bodyFormationGeneric_ArbitraryList( 650, target, groupMomentum );
  }
  if ( simNumber == '7' ) {
    // TODO see if this can just be deleted at this point
    // globalQuadrant = Simulations::octreeDemonstration(10);
  }

  if ( simNumber == '8' ) {
  }

  if ( simNumber == '9' ) {
    newSimulation = Simulations::billiards3_ArbitraryList( 5 );
  }


  return newSimulation;

}
