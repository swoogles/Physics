/*
 * Circle.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#include "Circle.h"

Circle::Circle() {
	radius = 1;

}

Circle::~Circle() {
}

float Circle::getMarkerSize() {
	return (radius)/50;
}

void Circle::scaleMembers(float scalarFac) {
	radius *= scalarFac;
}

matrix<double> Circle::getPts() {
	int curPt = 0;

	/*
	float circDelta = 2*PI/numPts;
	float xPos = pos(0);
	float yPos = pos(1);
	float zPos = pos(2);
	*/


	for (float i = 0; i <= 2*PI; i += 2*PI/numPts)
	{
		pts(curPt, 0) = cos(i) * radius;
		pts(curPt, 1) = sin(i) * radius;
		pts(curPt, 2) = 0;
		pts(curPt, 3) = 1;

		//row(pts,curPt) = prod(rotMat, row(pts,curPt));

		curPt++;
	}


	return pts;
}

void Circle::drawScale() {
	glScalef(radius, radius, radius);
	//glScalef(1,1,1);
}

void Circle::drawUnit() {
	//glutWireSphere( 1, numPts, numPts);
	glutSolidSphere( 1, numPts, numPts);
}

float Circle::getRadius() {
	return radius;
}

void Circle::setRadius(float inRadius) {
	radius = inRadius;
}


int Circle::getType() {
	return 1;
}

float Circle::getMomentOfInertia() {
	return (2 * mass * (radius * radius))/ 5;
}
