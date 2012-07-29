/*
 * Box.h
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#ifndef BOX_H_
#define BOX_H_

#include "StraightShape.h"

class Box: public StraightShape {
private:
	static GLfloat * cube_vertices;

	static GLubyte * frontIndices;
	static GLubyte * backIndices;

	static GLubyte * rightIndices;
	static GLubyte * leftIndices;

	static GLubyte * topIndices;
	static GLubyte * bottomIndices;
public:
	Box();
	virtual ~Box();

	static void init();

	matrix<double> getPts();
	void drawUnit();
};

#endif /* BOX_H_ */
