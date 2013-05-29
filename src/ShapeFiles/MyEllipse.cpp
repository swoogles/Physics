/*
 * MyEllipse.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#include "MyEllipse.h"

MyEllipse::MyEllipse() {
	a = 1.0;
	b = 1.0;
	e = b/a;
}

MyEllipse::~MyEllipse() {
}

matrix<double> MyEllipse::getPts() {
	int curPt = 0;
	float sinalpha, cosalpha;

	for (float i = 0; i <= 2*PI; i += 2*PI/numPts) {
		sinalpha = sin(i);
		cosalpha = cos(i);

		pts(curPt, 0) = (a * cosalpha - b * sinalpha);
		pts(curPt, 1) = (a * cosalpha + b * sinalpha);
		pts(curPt, 2) = 0;
		pts(curPt, 3) = 1;

		//row(pts, curPt) = prod( rotMat, row(pts, curPt) );

		curPt++;
	}

	return pts;
}

// Old getPts()
/*
matrix<double> myEllipse::getPts() {
	int curPt = 0;
	float beta = angle;
	float sinbeta = sin(beta);
	float cosbeta = cos(beta);
	float sinalpha, cosalpha;
	//float sinalphaB, cosalphaB;


	for (float i = 0; i <= 2*PI; i += 2*PI/numPts) {
		sinalpha = sin(i);
		cosalpha = cos(i);

		//sinalphaB = sin(i+PI/8);
		//cosalphaB = cos(i+PI/8);

		pts(curPt, 0) = pos(0) + (a * cosalpha * cosbeta - b * sinalpha*sinbeta);
		pts(curPt, 1) = pos(1) + (a * cosalpha * sinbeta + b * sinalpha*cosbeta);
		pts(curPt, 2) = pos(2) + 0;
		curPt++;
	}

	return pts;
}
*/

float MyEllipse::getMarkerSize() {
	return (a)/50;
}

void MyEllipse::scaleMembers(float scalarFac) {
	a *= scalarFac;
	b *= scalarFac;
}

void MyEllipse::scaleMembers(float xFac, float yFac) {
	a *= xFac;
	b *= yFac;
}
