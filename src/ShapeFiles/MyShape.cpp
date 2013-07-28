/*
 * MyShape.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#include "MyShape.h"
#include "../Physics/WorldSettings.h"

#include <iostream>

using namespace std;
using namespace boost::numeric::ublas;

float MyShape::G = 6.67384e-11;

//TODO Make sure I'm supposed to actually be referencing the 0 here...
boost::numeric::ublas::vector<MyShape *> MyShape::shapes(0);


MyShape::MyShape() {
	numPts = 16;
	//pos.resize(4);
	//toolVec.resize(4);

	pts.resize(numPts, 4);
	//color.resize(3);

	//scaleMat.resize(4,4);
	//rotMat.resize(4,4);

	pos[0] = 0; pos[1] = 0; pos[2] = 0; pos[3] = 1;
	color[0] = 0.25; color[1] = 0.0; color[2] = 0.0;


	sgMakeIdentQuat(orientationQuat);
	mass = 1;
	density = 1;

	for (int i = 0; i < 4; i++) {
		momentum[i] = 0;
		angMomentum[i] = 0;
		angVelocity[i] = 0;
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				//scaleMat(i,j) = 1;
				//rotMat(i,j) = 1;
				orientationMat[i][j] = 1;
			}
			else {
				//scaleMat(i,j) = 0;
				//rotMat(i,j) = 0;
			}
		}
	}

	ptsHighlighted = true;
}

MyShape::~MyShape() {
	//cout << "Killing MyShape" << endl;
}

matrix<double> MyShape::getPts() {
	return pts;
}

void MyShape::printPts() {
	cout << pts << endl;
}

float MyShape::getMarkerSize() {
	return 0;
}



bool MyShape::setKineticEnergy(float newKineticEnergy)
{
  bool moving;
  sgVec4 * curVelocity = getVelocity();
  if ( sgLengthVec4( *curVelocity ) == 0 )
  {
    moving = false;
  }
  else
  {
    sgVec4 newVelocity;
    sgNormaliseVec4(newVelocity, *curVelocity );


    float totalVelocity = sqrt( newKineticEnergy ) / mass * 2;
    sgScaleVec4( newVelocity, totalVelocity );
    setVelocity( newVelocity );
    moving = true;
  }

  return moving;
}


//! Returns kineticEnergy of object
float MyShape::getKineticEnergy()
{
  return kineticEnergy;
}


void MyShape::calcColor() {
  float totalMass=WorldSettings::getTotalMass();
  // cout << "totalMass:" << totalMass << endl;

	float redAmount = 0.25 + mass / (totalMass/3.0);
	if (redAmount > 1.0)
		redAmount = 1.0;

	float greenAmount = mass / (0.8 *totalMass);
	if (greenAmount > 1.0) {
		greenAmount = 1.0;
	}


	color[0] = redAmount;
	color[1] = greenAmount;
}


/*
void MyShape::getUnitVecTo(MyShape * destination, sgVec4 unitv) {
	sgSubVec4(unitv, pos, destination->pos);
	sgNormaliseVec4(unitv);
}
*/

MyShape * MyShape::getShapeFromList( int shapeIndex )
{
  MyShape * returnShape;
  if ( shapes.size() > shapeIndex )
  {
    returnShape = shapes(shapeIndex);
  }

  return returnShape;
}

int MyShape::addShapeToList( MyShape * insertShape )
{
  int curSize = shapes.size();
  shapes.resize(curSize + 1);
  shapes(curSize) = insertShape;
  return curSize;
}

void MyShape::removeShapeFromList( int shapeIndex )
{
  //
  MyShape::shapes(shapeIndex)->~MyShape();
  MyShape::shapes.erase_element(shapeIndex);
  // TODO Decide how to best erase/resize
  // boost::numeric::ublas::vector<MyShape *> newListA;
  // newListA = shapes.subVector(0,i);
  // boost::numeric::ublas::vector<MyShape *> newListB;
  // newListB = shapes.subVector(i, shapes.size() );
  // shapes = newListA + newListB;
}


void MyShape::clearShapes() {
	for (int i = MyShape::shapes.size() - 1; i > -1; i--) {
      cout << "Shape # " << i << endl;
		MyShape::shapes(i)->~MyShape();
		MyShape::shapes.erase_element(i);
	}
	MyShape::shapes.resize(0);
}


void MyShape::setRadius(float) {}
float MyShape::getRadius() { return 1;}

int MyShape::getType() {}
