/*
 * Simulations.cpp
 *
 *  Created on: Jul 22, 2011
 *      Author: brasure
 */

#include "Simulations.h"

boost::numeric::ublas::vector<MyShape *> Simulations::physicalObjects(0);

using namespace std;

void testAutoScale() {
	int numObjects = 5;
	for (int i = 0; i < numObjects; i++ ) {

	}
}


void Simulations::largeGridAlternating() {
	sgVec4 startPos;

	int gridSide = 18;

  MyShape * curShape;

	for (int i = 0; i < gridSide; i++)
	{
		for (int j = 0; j < gridSide; j++) {
      int curIndex = i*gridSide + j;
			if ( (i*gridSide + j) % 2 == 0) {
				curShape = new Circle;
				startPos[2] = 5;

			}
			else {
				curShape = new Box;
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

  MyShape * curShape;

	curShape = new Circle;
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

	curShape = new Circle;
	curShape->setPos(startPos);
	curShape->setRadius(earthRadius*earthRadiusScale);
	curShape->setMomentum(startMom);
	curShape->setDensity(earthDensity);
	curShape->setMass(earthMass);

	cout << "earthMom: " << curShape->getMomentum() << endl;
	cout << "earthMass: " << curShape->getMass() << endl;

	cout << "numBodies: " << MyShape::shapes.size() << endl;
}

void Simulations::billiards(int numRows) {
	WorldSettings::setDT(.003);
	WorldSettings::makeAllElastic();
	WorldSettings::setGravBetweenObjects(false);
	WorldSettings::setConstGravField(false);
	WorldSettings::setTimeElapsed(0);

	sgVec4 gField;
	gField[0] = -.2; gField[1] = 0; gField[2] = 0;

	WorldSettings::setConstGravFieldVal(gField);

	unsigned int numPieces = 0;
	float cueMass = 100.0;
	float cueRadius = 2;

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

  MyShape * curShape;

	curShape = new Circle;
	curShape->setPos(numRows, numRows*3, 0);
	curShape->setMass(cueMass);
	curShape->setRadius(ballRadius);
	curShape->setVelocity(cueVelocity);
	curShape->setColor(newColor);

  MyShape::addShapeToList( curShape );

	unsigned int cutOff = numRows*2;
	for (unsigned int i = 1; i < numRows+1; i++) {
		for (unsigned int j = i; j < cutOff; j+= 2) {
			curShape = new Circle;
			curShape->setPos(j*1.7, i*2.5, 0);
			curShape->setMass(ballMass);
			curShape->setRadius(ballRadius);
      MyShape::addShapeToList( curShape );
		}
		cout << endl;
		cutOff--;
	}

	cout << "NumPieces: " << numPieces << endl;
}

void Simulations::billiards2(int numRows) {
	WorldSettings::setDT(.003);
	WorldSettings::makeAllElastic();
	//WorldSettings::makeAllInelastic();
	WorldSettings::setGravBetweenObjects(false);
	WorldSettings::setConstGravField(false);
	WorldSettings::setTimeElapsed(0);

	sgVec4 gField;
	gField[0] = -.2; gField[1] = 0; gField[2] = 0;

	WorldSettings::setConstGravFieldVal(gField);

	unsigned int numPieces = 0;
	float cueMass = 100.0;
	float ballMass = 0.156;

	float cueRadius = 2;
	float ballRadius = .95;

  numPieces= numRows*numRows;

	sgVec4 cueVelocity;
	cueVelocity[0] = 30;
	//cueVelocity[1] = -15.05;
	cueVelocity[1] = -75;
	cueVelocity[2] = 0;

	sgVec3 newColor;
	newColor[0] = 1;
	newColor[1] = 1;
	newColor[2] = 1;
    
  MyShape * shapeForInsertion;

	shapeForInsertion = new Circle;
	shapeForInsertion->setPos(numRows, numRows*3, 0);
	shapeForInsertion->setMass(cueMass);
	shapeForInsertion->setRadius(ballRadius);
	shapeForInsertion->setVelocity(cueVelocity);
	shapeForInsertion->setColor(newColor);
  MyShape::addShapeToList( shapeForInsertion );

	// MyShape::shapes.resize(MyShape::shapes.size() + numPieces );

	for (unsigned int i = 0; i < numRows; i++) {
		for (unsigned int j = 0; j < numRows; j++) {
			shapeForInsertion = new Circle;
			shapeForInsertion->setPos(j*3, i*3, 0);
			shapeForInsertion->setMass(ballMass);
			shapeForInsertion->setRadius(ballRadius);
      newColor[1] = -( ( -.5 + (j/float(numRows)) ) * ( -.5 + (j/float(numRows)) ) )+ 1.0;
      shapeForInsertion->setColor(newColor);

      MyShape::addShapeToList( shapeForInsertion );
		}
		cout << endl;
	}

	cout << "NumPieces: " << numPieces << endl;
	cout << "Type: " << MyShape::shapes(0)->getType() << endl;

}

void Simulations::billiards3(int numRows) {
	WorldSettings::setDT(.003);
	WorldSettings::makeAllElastic();
	//WorldSettings::makeAllInelastic();
	WorldSettings::setGravBetweenObjects(false);
	WorldSettings::setConstGravField(false);
	WorldSettings::setTimeElapsed(0);

	sgVec4 gField;
	gField[0] = -.2; gField[1] = 0; gField[2] = 0;

	WorldSettings::setConstGravFieldVal(gField);

	unsigned int numPieces = 0;
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

  MyShape * shapeForInsertion;

	shapeForInsertion = new Circle;
	shapeForInsertion->setPos(numRows, numRows*5, 0);
	shapeForInsertion->setMass(cueMass);
	shapeForInsertion->setRadius(cueRadius);
	shapeForInsertion->setVelocity(cueVelocity);
	shapeForInsertion->setColor(newColor);
  MyShape::addShapeToList( shapeForInsertion );

	newColor[0] = 0;
	newColor[2] = 0;

	unsigned int curShape = 1;
	for (unsigned int i = 0; i < numRows; i++) {
		for (unsigned int j = 0; j < numRows; j++) {
      for (unsigned int z = 0; z < numRows; z++) {

			shapeForInsertion = new Circle;
			shapeForInsertion->setPos(j*4, i*4, z*4);
			shapeForInsertion->setMass(ballMass);
			shapeForInsertion->setRadius(ballRadius);
      newColor[1] = -( ( -.5 + (z/float(numRows)) ) * ( -.5 + (z/float(numRows)) ) )+ 1.0;
      shapeForInsertion->setColor(newColor);

      MyShape::addShapeToList( shapeForInsertion );
      }
		}
		cout << endl;
	}

	cout << "NumPieces: " << numPieces << endl;
	cout << "Type: " << MyShape::shapes(0)->getType() << endl;

}

Quadrant * Simulations::octreeDemonstration(int numRows) {
  WorldSettings::setDT(.003);
  WorldSettings::makeAllElastic();
  //WorldSettings::makeAllInelastic();
  WorldSettings::setGravBetweenObjects(false);
  WorldSettings::setConstGravField(false);
  WorldSettings::setTimeElapsed(0);

  sgVec4 gField;
  gField[0] = -.2; gField[1] = 0; gField[2] = 0;

  WorldSettings::setConstGravFieldVal(gField);

  unsigned int numPieces = 4;

  // float width = 40;
  // float height = 40;
  // float depth = 40;

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


  Quadrant * mainQuadrant = new Quadrant(4, 1, pos, dimensions);

  int levels = 1;
  mainQuadrant->subDivideAll(levels,8);

	sgVec4 startPlacement, startMomentum;
  startPlacement[0] = 15;
  startPlacement[1] = 20;
  startPlacement[0] = 15;

  int curShapeIdx;
  MyShape * curShapeInsert;

	float totalMass = 0.0;

  sgVec3 curShapeColor;
  curShapeColor[0]=1.0;
  curShapeColor[1]=1.0;
  curShapeColor[2]=1.0;

	sgVec4 cueVelocity;
	cueVelocity[0] = 0;
	cueVelocity[1] = -.5;
	cueVelocity[2] = 0;

  for (int i = 0; i < 10000; i++)
  {
    curShapeIdx = MyShape::shapes.size();
    randomSplitBodyPlacementInZone(startPlacement, dimensions, target);
    curShapeInsert = new Circle;
    curShapeInsert->setPos( startPlacement );
    curShapeInsert->setColor( curShapeColor );
    curShapeInsert->setMass(1);
    curShapeInsert->setRadius(.5);
    // if ( i % 5 == 0 )
    // {
      //curShapeInsert->setVelocity( cueVelocity );
    // }

    MyShape::addShapeToList( curShapeInsert );
    while ( isConflict( curShapeIdx ) )
    {
      randomSplitBodyPlacementInZone(startPlacement, dimensions, target);
      curShapeInsert->setPos( startPlacement );
    }
    mainQuadrant->insertShape( curShapeInsert );
  }

  WorldSettings::adjustTotalMass( totalMass );

  return mainQuadrant;
}


void Simulations::simpleCollision() {
	WorldSettings::setDT(.01);

	unsigned int numPieces = 2;
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

  MyShape * curShape;

	curShape = new Circle;
	curShape->setPos(startPlacement);
	curShape->setMass(aMass);
	curShape->setRadius(aRadius*2);
	curShape->setVelocity(0, 25, 0);
	curShape->setAngVelocity(startAngMom);
  MyShape::addShapeToList( curShape );

	// Object B
	startPlacement[0] = 2.5;
	//startPlacement[1] = -.9; //45 Degree placement
	startPlacement[1] = 0;
	startPlacement[2] = 0;

	curShape = new Circle;
	curShape->setPos(startPlacement);
	curShape->setMass(bMass);
	curShape->setRadius(bRadius*2);
	curShape->setVelocity(0, -25, 0);
  MyShape::addShapeToList( curShape );


}

void Simulations::disruption() {
  bodyFormation( 1000 );
  MyShape::shapes.resize(MyShape::shapes.size()+1);
  int size = MyShape::shapes.size();
  int cur = size - 1;

  int numPieces = 1;
	float objectDensity = DENSITY_SUN;
	float bodyVolume = (MASS_SUN)/(objectDensity)/3;
	float pieceRadius = getSplitBodyRadius(bodyVolume, numPieces);
	float pieceMass = pow(pieceRadius, 3.0);
	sgVec4 startMomentum;
  startMomentum[0] = pieceMass/35 ; 
  startMomentum[1] = 0;
  startMomentum[2] = 0;

  MyShape::shapes(cur) = new Circle;
  MyShape::shapes(cur)->setPos(-pieceRadius * 30, 0, 0);
  MyShape::shapes(cur)->setMass(pieceMass);
  MyShape::shapes(cur)->setRadius(pieceRadius);
  MyShape::shapes(cur)->setMomentum(startMomentum);
  MyShape::shapes(cur)->setDensity(objectDensity);
}

void Simulations::bodyFormation(unsigned int numPieces) {
	WorldSettings::setDT(1000);
	WorldSettings::makeAllInelastic();
	WorldSettings::setGravBetweenObjects(true);
	WorldSettings::setConstGravField(false);
	WorldSettings::setAutoScaling(true);
	WorldSettings::setTimeElapsed(0);
	WorldSettings::setTotalMass(0);

	float objectDensity = DENSITY_SUN;
	float bodyVolume = (MASS_SUN * 3)/(objectDensity);
	float pieceRadius = getSplitBodyRadius(bodyVolume, numPieces);
	sgVec4 startPlacement, startMomentum, target;

  target[0]=-1000;
  target[1]=0;
  target[2]=0;
  target[3]=1;

	float pieceMass = pow(pieceRadius, 3.0);
	pieceMass = pieceMass * (4.0/3.0) * M_PI * (objectDensity);

	float totalMass = 0.0;

	srand ( time(NULL) );

	for (unsigned int i = 0; i < numPieces; i++) {
    MyShape * curShape;

		if (i % 2 == 0) {
			randomSplitBodyMomentum(startMomentum, pieceMass);
			randomSplitBodyPlacement(startPlacement, pieceRadius, target);
		}
		else {
			sgNegateVec4(startMomentum);
			sgNegateVec4(startPlacement);
		}

		curShape = new Circle;
    curShape->setPos( startPlacement );
		curShape->setMass(pieceMass);
		curShape->setRadius(pieceRadius);
		curShape->setMomentum(startMomentum);
		curShape->setDensity(objectDensity);

    MyShape::addShapeToList( curShape );
		//Check if being placed on previously created object
		while ( isConflict(i) ) {
			randomSplitBodyPlacement(startPlacement, pieceRadius, target);
      curShape->setPos( startPlacement );
		}

		totalMass += curShape->getMass();
	}
  WorldSettings::adjustTotalMass( totalMass );
}

void Simulations::bodyFormationGeneric(unsigned int numPieces, sgVec4 target, sgVec4 groupMomentum) {
	WorldSettings::setDT(1000);
	WorldSettings::makeAllInelastic();
	WorldSettings::setGravBetweenObjects(true);
	WorldSettings::setConstGravField(false);
	WorldSettings::setAutoScaling(true);
	WorldSettings::setTimeElapsed(0);
	WorldSettings::setTotalMass(0);

	float objectDensity = DENSITY_SUN;
	float bodyVolume = (MASS_SUN)/(objectDensity);
	float pieceRadius = getSplitBodyRadius(bodyVolume, numPieces);
	sgVec4 startPlacement, startMomentum;

	float pieceMass = pow(pieceRadius, 3.0);
	pieceMass = pieceMass * (4.0/3.0) * M_PI * (objectDensity);

	float totalMass = 0.0;

	srand ( time(NULL) );

  MyShape * curShape;
  int targetSize = MyShape::shapes.size() + numPieces;
	for (unsigned int i = MyShape::shapes.size(); i < targetSize; i++) {

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

		curShape = new Circle;
    cout << "StartPos: " << startPlacement[0] << endl;
		curShape->setPos(startPlacement[0], startPlacement[1], startPlacement[2]);
		curShape->setMass(pieceMass);
		curShape->setRadius(pieceRadius);
		curShape->setMomentum(startMomentum);
		curShape->setDensity(objectDensity);

    MyShape::addShapeToList( curShape );

		//Check if being placed on previously created object
		while ( isConflict(i) ) {
			randomSplitBodyPlacement(startPlacement, pieceRadius, target);
			curShape->setPos(startPlacement[0], startPlacement[1], startPlacement[2]);
		}
		totalMass += MyShape::shapes(i)->getMass();
	}
  WorldSettings::adjustTotalMass( totalMass );
}

MyShape * Simulations::getShapeFromList( int shapeIndex )
{
  MyShape * returnShape;
  if ( physicalObjects.size() > shapeIndex )
  {
    returnShape = physicalObjects(shapeIndex);
  }

  return returnShape;
}

int Simulations::addShapeToList( MyShape * insertShape )
{
  int curSize = physicalObjects.size();
  physicalObjects.resize(curSize + 1);
  physicalObjects(curSize) = insertShape;
  return curSize;
}
