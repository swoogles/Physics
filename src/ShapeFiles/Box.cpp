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

Box::Box(sgVec4 pos, float sideLength, sgVec3 color) {
	sgCopyVec4(this->pos, pos);
	width = sideLength;
	height = sideLength;
	depth = sideLength;
	sgCopyVec3(this->color, color);
}

Box::~Box() {
}

void Box::drawUnit(){
  // YOU FUCKING WHORE!!!!
  // WHY WOULD YOU EVER BE WRITTEN LIKE THIS?!?!?!
  //glutWireCube( width );
  glutWireCube( 1.0 );
}

void Box::drawScale(){
	glScalef(width, height, depth);
}

void Box::setSideLength(float inSideLength) {
	width = inSideLength;
	height = inSideLength;
	depth = inSideLength;
}

float Box::getWidth() {
	return width;
}

int Box::getType() {
	return 3;
}
