/*
 * StraightShape.h
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#ifndef STRAIGHTSHAPE_H_
#define STRAIGHTSHAPE_H_

#include "MyShape.h"

class StraightShape: public MyShape {
protected:
	float width, height, depth;
public:
	StraightShape();
	virtual ~StraightShape();

	void scaleMembers(float);
	void scaleMembers(float, float, float);
	float getMarkerSize();

	void resize(float, float, float);

	void drawScale();
};

#endif /* STRAIGHTSHAPE_H_ */
