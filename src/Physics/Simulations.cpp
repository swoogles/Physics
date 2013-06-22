/*
 * Simulations.cpp
 *
 *  Created on: Jul 22, 2011
 *      Author: brasure
 */

#include "Simulations.h"

using namespace std;

void testAutoScale() {
	int numObjects = 5;
	for (int i = 0; i < numObjects; i++ ) {

	}
}


void largeGridAlternating() {
	sgVec4 sP;

	int gridSide = 18;

	for (int i = 0; i < gridSide; i++)
	{
		for (int j = 0; j < gridSide; j++) {
			MyShape::shapes.resize(MyShape::shapes.size()+1);
			if ( (i*gridSide + j) % 2 == 0) {
				MyShape::shapes(i*gridSide + j) = new Circle;
				sP[2] = 5;

			}
			else {
				MyShape::shapes(i*gridSide + j) = new Box;
				sP[2] = -5;

			}


			sP[0] = -gridSide + 2*j;
			sP[1] = -gridSide + 2*i;
			sP[3] = 1;

			//cout << "About to set stuff \n";
			//cout << "Shape #: " << (i*gridSide + j) << endl;

			MyShape::shapes(i*gridSide + j)->setPos(sP[0], sP[1], sP[2]);
			//MyShape::shapes(i*4 + j)->setPos(sP);

			//cout << "set pos \n";
			//MyShape::shapes(i)->setPos(-1+2*i, -1+2*i, 0);
			MyShape::shapes(i*gridSide + j)->setMass(.6e10);
			//cout << "Done making shape" << endl;
		}
	}
}



void simpleOrbit() {

	sgVec4 startPos, startMom;
	sgVec4 * tempMom;
	float sunRadiusScale = 15;
	float earthRadiusScale = 220;

	float sunDensity = DENSITY_SUN;
	float sunMass = MASS_SUN;
	float sunVolume = (sunMass*MASS_VAR)/(sunDensity*CONVERSION_CONST);
	//sunVolume *= VOLUME_VAR;
	float sunRadius = getSplitBodyRadius(sunVolume, 1);

	cout << "sunVolume: " << sunVolume << endl;
	cout << "sunRadius: " << sunRadius << endl;

	float earthDensity = DENSITY_EARTH;
	float earthMass = MASS_EARTH;
	float earthVolume = (earthMass*MASS_VAR)/(earthDensity*CONVERSION_CONST);
	//earthVolume *= VOLUME_VAR;
	float earthRadius = getSplitBodyRadius(earthVolume, 1);
	cout << "earthVolume: " << earthVolume << endl;
	cout << "earthRadius: " << earthRadius << endl;

	startMom[0] = 0;
	startMom[1] = -2e-1;//-2.9e2; //-2.9 * earthMass;
	startMom[2] = 0;
	startMom[3] = 0;

	int curShape = 0;

	startPos[0] = 0;
	startPos[1] = 0;
	startPos[2] = 0;
	startPos[3] = 1;

	MyShape::shapes.resize(MyShape::shapes.size()+1);
	MyShape::shapes(curShape) = new Circle;
	MyShape::shapes(curShape)->setPos(startPos[0], startPos[1], startPos[2]);
	MyShape::shapes(curShape)->setRadius(sunRadius * sunRadiusScale);
	//MyShape::shapes(curShape)->setMomentum(startMom);
	MyShape::shapes(curShape)->setMomentum(startMom[0], startMom[1], startMom[2]);
	MyShape::shapes(curShape)->setDensity(sunDensity);
	MyShape::shapes(curShape)->setMass(sunMass);

	tempMom = MyShape::shapes(curShape)->getMomentum();
	cout << "sunMom: " << (*tempMom)[0] << ", " << (*tempMom)[1] << endl;
	delete tempMom;

	cout << "sunMass: " << MyShape::shapes(curShape)->getMass() << endl;

	curShape++;


	startPos[0] = sunRadius * 214.94;
	startPos[1] = 0;
	startPos[2] = 0;
	startPos[3] = 1;

	sgNegateVec4(startMom);

	MyShape::shapes.resize(MyShape::shapes.size()+1);
	MyShape::shapes(curShape) = new Circle;
	MyShape::shapes(curShape)->setPos(startPos[0], startPos[1], startPos[2]);
	MyShape::shapes(curShape)->setRadius(earthRadius*earthRadiusScale);
	MyShape::shapes(curShape)->setMomentum(startMom);
	MyShape::shapes(curShape)->setMomentum(startMom[0], startMom[1], startMom[2]);
	MyShape::shapes(curShape)->setDensity(earthDensity);
	MyShape::shapes(curShape)->setMass(earthMass);


	tempMom = MyShape::shapes(curShape)->getMomentum();
	cout << "earthMom: " << (*tempMom)[0] << ", " << (*tempMom)[1] << endl;
	delete tempMom;

	cout << "earthMass: " << MyShape::shapes(curShape)->getMass() << endl;

	cout << "numBodies: " << MyShape::shapes.size() << endl;
	cout << "earth #" << curShape << endl;
}

void billiards(int numRows) {
	WorldSettings::setDT(.003);
	WorldSettings::makeAllElastic();
	//WorldSettings::makeAllInelastic();
	WorldSettings::setGravBetweenObjects(false);
	WorldSettings::setConstGravField(false);
	WorldSettings::setTimeElapsed(0);

	sgVec4 gField;
	gField[0] = -.2; gField[1] = 0; gField[2] = 0;

	WorldSettings::setConstGravFieldVal(gField);

	//TODO setup to accept number of rows, rather than balls
	//unsigned int numRows = 31; //500 Ball Craziness
	//unsigned int numRows = 5; //Standard 15 balls
	//unsigned int numRows = 9;
	unsigned int numPieces = 0;
	float cueMass = 100.0;
	//float cueMass = 0.156;
	float ballMass = 0.156;

	//float ballRadius = 0.057;
	float cueRadius = 2;
	float ballRadius = .95;

	for (int i = 1; i < numRows+1; i++)
		numPieces+=i;

	MyShape::shapes.resize(MyShape::shapes.size() + numPieces + 1);

	sgVec4 cueVelocity;
	cueVelocity[0] = 30;
	//cueVelocity[1] = -15.05;
	cueVelocity[1] = -75;
	cueVelocity[2] = 0;

	sgVec3 newColor;
	newColor[0] = 1;
	newColor[1] = 1;
	newColor[2] = 1;

	MyShape::shapes(0) = new Circle;
	MyShape::shapes(0)->setPos(numRows, numRows*3, 0);
	MyShape::shapes(0)->setMass(cueMass);
	//MyShape::shapes(0)->setRadius(cueRadius);
	MyShape::shapes(0)->setRadius(ballRadius);
	MyShape::shapes(0)->setVelocity(cueVelocity);
	MyShape::shapes(0)->setColor(newColor);

	unsigned int curShape = 1;
	unsigned int cutOff = numRows*2;
	for (unsigned int i = 1; i < numRows+1; i++) {
		for (unsigned int j = i; j < cutOff; j+= 2) {
			MyShape::shapes(curShape) = new Circle;
			MyShape::shapes(curShape)->setPos(j*1.7, i*2.5, 0);
			//MyShape::shapes(curShape)->setPos(j*1.1, i*2.0, 0);
			MyShape::shapes(curShape)->setMass(ballMass);
			MyShape::shapes(curShape)->setRadius(ballRadius);
			//cout << j << " ";
			curShape++;
		}
		cout << endl;
		cutOff--;
	}

	/*
	for (unsigned int i = 1; i < MyShape::shapes.size(); i++) {
		MyShape::shapes(0) = new Circle;
	}
	*/

	cout << "NumPieces: " << numPieces << endl;
	cout << "Type: " << MyShape::shapes(0)->getType() << endl;

}

void billiards2(int numRows) {
	WorldSettings::setDT(.003);
	WorldSettings::makeAllElastic();
	//WorldSettings::makeAllInelastic();
	WorldSettings::setGravBetweenObjects(false);
	WorldSettings::setConstGravField(false);
	WorldSettings::setTimeElapsed(0);

	sgVec4 gField;
	gField[0] = -.2; gField[1] = 0; gField[2] = 0;

	WorldSettings::setConstGravFieldVal(gField);

	//TODO setup to accept number of rows, rather than balls
	//unsigned int numRows = 31; //500 Ball Craziness
	//unsigned int numRows = 5; //Standard 15 balls
	//unsigned int numRows = 9;
	unsigned int numPieces = 0;
	float cueMass = 100.0;
	//float cueMass = 0.156;
	float ballMass = 0.156;

	//float ballRadius = 0.057;
	float cueRadius = 2;
	float ballRadius = .95;

		numPieces= numRows*numRows;

	MyShape::shapes.resize(MyShape::shapes.size() + numPieces + 1);

	sgVec4 cueVelocity;
	cueVelocity[0] = 30;
	//cueVelocity[1] = -15.05;
	cueVelocity[1] = -75;
	cueVelocity[2] = 0;

	sgVec3 newColor;
	newColor[0] = 1;
	newColor[1] = 1;
	newColor[2] = 1;

	MyShape::shapes(0) = new Circle;
	MyShape::shapes(0)->setPos(numRows, numRows*3, 0);
	MyShape::shapes(0)->setMass(cueMass);
	//MyShape::shapes(0)->setRadius(cueRadius);
	MyShape::shapes(0)->setRadius(ballRadius);
	MyShape::shapes(0)->setVelocity(cueVelocity);
	MyShape::shapes(0)->setColor(newColor);

	unsigned int curShape = 1;
	for (unsigned int i = 0; i < numRows; i++) {
		for (unsigned int j = 0; j < numRows; j++) {
			MyShape::shapes(curShape) = new Circle;
			MyShape::shapes(curShape)->setPos(j*3, i*3, 0);
			//MyShape::shapes(curShape)->setPos(j*1.1, i*2.0, 0);
			MyShape::shapes(curShape)->setMass(ballMass);
			MyShape::shapes(curShape)->setRadius(ballRadius);
      newColor[1] = -( ( -.5 + (j/float(numRows)) ) * ( -.5 + (j/float(numRows)) ) )+ 1.0;
      MyShape::shapes(curShape)->setColor(newColor);
			//cout << j << " ";
			curShape++;
		}
		cout << endl;
	}

	/*
	for (unsigned int i = 1; i < MyShape::shapes.size(); i++) {
		MyShape::shapes(0) = new Circle;
	}
	*/

	cout << "NumPieces: " << numPieces << endl;
	cout << "Type: " << MyShape::shapes(0)->getType() << endl;

}

void billiards3(int numRows) {
	WorldSettings::setDT(.003);
	WorldSettings::makeAllElastic();
	//WorldSettings::makeAllInelastic();
	WorldSettings::setGravBetweenObjects(false);
	WorldSettings::setConstGravField(false);
	WorldSettings::setTimeElapsed(0);

	sgVec4 gField;
	gField[0] = -.2; gField[1] = 0; gField[2] = 0;

	WorldSettings::setConstGravFieldVal(gField);

	//TODO setup to accept number of rows, rather than balls
	//unsigned int numRows = 31; //500 Ball Craziness
	//unsigned int numRows = 5; //Standard 15 balls
	//unsigned int numRows = 9;
	unsigned int numPieces = 0;
	float cueMass = 100.0;
	//float cueMass = 0.156;
	float ballMass = 0.156;

	//float ballRadius = 0.057;
	float cueRadius = 4;
	float ballRadius = .95;

		numPieces= numRows*numRows*numRows;

	MyShape::shapes.resize(MyShape::shapes.size() + numPieces + 1);

	sgVec4 cueVelocity;
	cueVelocity[0] = 30;
	//cueVelocity[1] = -15.05;
	cueVelocity[1] = -75;
	cueVelocity[2] = 20;

	sgVec3 newColor;
	newColor[0] = 1;
	newColor[1] = 1;
	newColor[2] = 1;

	MyShape::shapes(0) = new Circle;
	MyShape::shapes(0)->setPos(numRows, numRows*5, 0);
	MyShape::shapes(0)->setMass(cueMass);
	MyShape::shapes(0)->setRadius(cueRadius);
	//MyShape::shapes(0)->setRadius(ballRadius);
	MyShape::shapes(0)->setVelocity(cueVelocity);
	MyShape::shapes(0)->setColor(newColor);

	newColor[0] = 0;
	newColor[2] = 0;

	unsigned int curShape = 1;
	for (unsigned int i = 0; i < numRows; i++) {
		for (unsigned int j = 0; j < numRows; j++) {
      for (unsigned int z = 0; z < numRows; z++) {
			MyShape::shapes(curShape) = new Circle;
			MyShape::shapes(curShape)->setPos(j*4, i*4, z*4);
			//MyShape::shapes(curShape)->setPos(j*1.1, i*2.0, 0);
			MyShape::shapes(curShape)->setMass(ballMass);
			MyShape::shapes(curShape)->setRadius(ballRadius);
      //newColor[0] = 1-(z/float(numRows));
      newColor[1] = -( ( -.5 + (z/float(numRows)) ) * ( -.5 + (z/float(numRows)) ) )+ 1.0;
      //newColor[2] = z/float(numRows);
      MyShape::shapes(curShape)->setColor(newColor);
			//cout << j << " ";
			curShape++;
      }
		}
		cout << endl;
	}

	/*
	for (unsigned int i = 1; i < MyShape::shapes.size(); i++) {
		MyShape::shapes(0) = new Circle;
	}
	*/

	cout << "NumPieces: " << numPieces << endl;
	cout << "Type: " << MyShape::shapes(0)->getType() << endl;

}

void octreeDemonstration(int numRows) {
	WorldSettings::setDT(.003);
	WorldSettings::makeAllElastic();
	//WorldSettings::makeAllInelastic();
	WorldSettings::setGravBetweenObjects(false);
	WorldSettings::setConstGravField(false);
	WorldSettings::setTimeElapsed(0);

	sgVec4 gField;
	gField[0] = -.2; gField[1] = 0; gField[2] = 0;

	WorldSettings::setConstGravFieldVal(gField);

	//TODO setup to accept number of rows, rather than balls
	//unsigned int numRows = 31; //500 Ball Craziness
	//unsigned int numRows = 5; //Standard 15 balls
	//unsigned int numRows = 9;
	unsigned int numPieces = 0;

	float cueMass = 100.0;
	//float cueMass = 0.156;
	float ballMass = 0.156;

	//float ballRadius = 0.057;
	float cueRadius = 2;
	float ballRadius = .95;

	//numPieces= numRows*numRows;

  cout << "Goddamnit" << endl;
	//MyShape::shapes.resize(MyShape::shapes.size() + numPieces + 1);

	sgVec4 cueVelocity;
	cueVelocity[0] = 30;
	//cueVelocity[1] = -15.05;
	cueVelocity[1] = -75;
	cueVelocity[2] = 0;

	sgVec3 newColor;
	newColor[0] = 1;
	newColor[1] = 1;
	newColor[2] = 1;

	//MyShape::shapes(0) = new Circle;
	//MyShape::shapes(0)->setPos(numRows, numRows*3, 0);
	//MyShape::shapes(0)->setMass(cueMass);
	//MyShape::shapes(0)->setRadius(cueRadius);
	//MyShape::shapes(0)->setRadius(ballRadius);
	//MyShape::shapes(0)->setVelocity(cueVelocity);
	//MyShape::shapes(0)->setColor(newColor);

	//unsigned int curShape = 1;
	//for (unsigned int i = 0; i < numRows; i++) {
		//for (unsigned int j = 0; j < numRows; j++) {
			//MyShape::shapes(curShape) = new Box;
			//MyShape::shapes(curShape)->setPos(j*3, i*3, 0);
			//MyShape::shapes(curShape)->setPos(j*1.1, i*2.0, 0);
			//MyShape::shapes(curShape)->setMass(ballMass);
			//MyShape::shapes(curShape)->resize(ballRadius, ballRadius, ballRadius);
      //newColor[1] = -( ( -.5 + (j/float(numRows)) ) * ( -.5 + (j/float(numRows)) ) )+ 1.0;
     //MyShape::shapes(curShape)->setColor(newColor);
			//cout << j << " ";
			////curShape++;
		//}
		//cout << endl;
	//}

    float width = 10;
    float height = 10;
    float depth = 10;

    sgVec4 pos;
    sgVec3 dimensions;
    dimensions[0] = width;
    dimensions[1] = height;
    dimensions[2] = depth;


    Quadrant mainQuadrant(4, 1, pos, dimensions);
    mainQuadrant.subdivide(1,1,1, 8);
    Quadrant * targetQuadrant = mainQuadrant.quadOctree->at(1,1,1);
    cout << "Level: " << targetQuadrant->level << endl;
    targetQuadrant->thisShape->draw();
    targetQuadrant->subdivide(4,4,4, 8);
    targetQuadrant = targetQuadrant->quadOctree->at(4,4,4);
    targetQuadrant->thisShape->draw();
    cout << "Level: " << targetQuadrant->level << endl;


    cout << "NumPieces: " << numPieces << endl;
    //cout << "Type: " << MyShape::shapes(0)->getType() << endl;

}


void simpleCollision() {
	WorldSettings::setDT(.01);

	unsigned int numPieces = 2;
	//float objectDensity = 5;
	float aMass = 1;
	float bMass = 1;
	float aRadius = 1;
	float bRadius = 1;

	sgVec4 startPlacement, startAngMom;

	for (unsigned int i = 0; i < numPieces; i++) {
		MyShape::shapes.resize(MyShape::shapes.size()+1);
	}

	// Object A
	startPlacement[0] = -1;
	startPlacement[1] = -6;
	startPlacement[2] = 0;
	startPlacement[3] = 1;

	startAngMom[0] = 0;
	startAngMom[1] = 0;
	startAngMom[2] = 0;

	MyShape::shapes(0) = new Circle;
	MyShape::shapes(0)->setPos(startPlacement[0], startPlacement[1], startPlacement[2]);
	MyShape::shapes(0)->setMass(aMass);
	MyShape::shapes(0)->setRadius(aRadius*2);
	MyShape::shapes(0)->setVelocity(0, 25, 0);
	MyShape::shapes(0)->setAngVelocity(startAngMom);

	// Object B
	startPlacement[0] = 2.5;
	//startPlacement[1] = -.9; //45 Degree placement
	startPlacement[1] = 0;
	startPlacement[2] = 0;
	startPlacement[3] = 1;
	MyShape::shapes(1) = new Circle;
	MyShape::shapes(1)->setPos(startPlacement[0], startPlacement[1], startPlacement[2]);
	MyShape::shapes(1)->setMass(bMass);
	MyShape::shapes(1)->setRadius(bRadius*2);
	MyShape::shapes(1)->setVelocity(0, -25, 0);


}

void disruption() {
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

void bodyFormation(unsigned int numPieces) {
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
		MyShape::shapes.resize(MyShape::shapes.size()+1);

		if (i % 2 == 0) {
			randomSplitBodyMomentum(startMomentum, pieceMass);
			randomSplitBodyPlacement(startPlacement, pieceRadius, target);
		}
		else {
			sgNegateVec4(startMomentum);
			sgNegateVec4(startPlacement);
		}

		MyShape::shapes(i) = new Circle;
		MyShape::shapes(i)->setPos(startPlacement[0], startPlacement[1], startPlacement[2]);
		MyShape::shapes(i)->setMass(pieceMass);
		MyShape::shapes(i)->setRadius(pieceRadius);
		MyShape::shapes(i)->setMomentum(startMomentum);
		MyShape::shapes(i)->setDensity(objectDensity);

		//Check if being placed on previously created object
		while ( isConflict(i) ) {
			randomSplitBodyPlacement(startPlacement, pieceRadius, target);
			MyShape::shapes(i)->setPos(startPlacement[0], startPlacement[1], startPlacement[2]);
		}
		totalMass += MyShape::shapes(i)->getMass();
	}
  WorldSettings::adjustTotalMass( totalMass );
}

void bodyFormationGeneric(unsigned int numPieces, sgVec4 target, sgVec4 groupMomentum) {
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

  int targetSize = MyShape::shapes.size() + numPieces;
	for (unsigned int i = MyShape::shapes.size(); i < targetSize; i++) {
		MyShape::shapes.resize(MyShape::shapes.size()+1);

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

		MyShape::shapes(i) = new Circle;
    cout << "StartPos: " << startPlacement[0] << endl;
		MyShape::shapes(i)->setPos(startPlacement[0], startPlacement[1], startPlacement[2]);
		MyShape::shapes(i)->setMass(pieceMass);
		MyShape::shapes(i)->setRadius(pieceRadius);
		MyShape::shapes(i)->setMomentum(startMomentum);
		MyShape::shapes(i)->setDensity(objectDensity);

		//Check if being placed on previously created object
		while ( isConflict(i) ) {
			randomSplitBodyPlacement(startPlacement, pieceRadius, target);
			MyShape::shapes(i)->setPos(startPlacement[0], startPlacement[1], startPlacement[2]);
		}
		totalMass += MyShape::shapes(i)->getMass();
	}
  WorldSettings::adjustTotalMass( totalMass );
}
