/*
 * MyEllipse.h
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#ifndef MYELLIPSE_H_
#define MYELLIPSE_H_

#include "RoundShape.h"

class MyEllipse: public RoundShape {
private:
	float a, b, e;
public:
	MyEllipse();
	virtual ~MyEllipse();

	matrix<double> getPts();

	float getMarkerSize();
	void scaleMembers(float);
	void scaleMembers(float, float);
};

#endif /* MYELLIPSE_H_ */
