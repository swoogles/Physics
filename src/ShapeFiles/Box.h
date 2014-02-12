/*
 * Box.h
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#ifndef BOX_H_
#define BOX_H_

#include "StraightShape.h"
#include <iostream>

using namespace std;

class Box: public StraightShape {
private:
	static GLfloat * cube_vertices;

	static GLubyte * frontIndices;
	static GLubyte * backIndices;

	static GLubyte * rightIndices;
	static GLubyte * leftIndices;

	static GLubyte * topIndices;
	static GLubyte * bottomIndices;

  float depth;
  float width;
  float height;
public:
	Box();
	virtual ~Box();

	matrix<double> getPts();
	void drawUnit();
  void drawScale();

  float getSideLength();
  void setSideLength(float);
  float getWidth();
  void setWidth(float inWidth);
  float getHeight();
  void setHeight(float inHeight);
  float getDepth();
  void setDepth(float inDepth);

	int getType();
};

#endif /* BOX_H_ */
