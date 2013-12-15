/*
 * Box.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#include "Box.h"

//Initializing the static variables needed for drawing
GLfloat * Box::cube_vertices = new GLfloat[48];

Box::Box()
{
  width = 1;
  height = 1;
  depth = 1;
}


Box::~Box() {
}

matrix<double> Box::getPts() {
	int ptsIt = 0;
	float xVal = -width/2;
	float yVal = height/2;
	float xDelta = width/(numPts/4);
	float yDelta = width/(numPts/4);


	while (ptsIt < numPts/4) {
		pts(ptsIt, 0) = xVal;
		pts(ptsIt, 1) = yVal;
		pts(ptsIt, 2) = 0;
		pts(ptsIt, 3) = 1;

		xVal += xDelta;
		ptsIt++;
	}

	while (ptsIt < numPts/2) {
		pts(ptsIt, 0) = xVal;
		pts(ptsIt, 1) = yVal;
		pts(ptsIt, 2) = 0;
		pts(ptsIt, 3) = 1;

		yVal -= yDelta;
		ptsIt++;
	}

	while (ptsIt < 3*numPts/4) {
		pts(ptsIt, 0) = xVal;
		pts(ptsIt, 1) = yVal;
		pts(ptsIt, 2) = 0;
		pts(ptsIt, 3) = 1;

		xVal -= xDelta;
		ptsIt++;
	}

	while (ptsIt < numPts) {
		pts(ptsIt, 0) = xVal;
		pts(ptsIt, 1) = yVal;
		pts(ptsIt, 2) = 0;
		pts(ptsIt, 3) = 1;

		yVal += yDelta;
		ptsIt++;
	}

	return pts;
}

void Box::drawUnit() const {
  // YOU FUCKING WHORE!!!!
  // WHY WOULD YOU EVER BE WRITTEN LIKE THIS?!?!?!
  //glutWireCube( width );
  glutWireCube( 1.0 );
}

void Box::drawScale() const {
	glScalef(width, height, depth);
}

float Box::getSideLength() {
	return sideLength;
}

void Box::setSideLength(float inSideLength) {
	width = inSideLength;
	height = inSideLength;
	depth = inSideLength;
}

float Box::getWidth() {
	return width;
}

void Box::setWidth(float inWidth) {
	width = inWidth;
}

float Box::getHeight() {
	return height;
}

void Box::setHeight(float inHeight) {
	height = inHeight;
}

float Box::getDepth() {
	return depth;
}

void Box::setDepth(float inDepth) {
	depth = inDepth;
}

int Box::getType() {
	return 3;
}
