/*
 * Box.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#include "Box.h"

//Initializing the static variables needed for drawing
GLfloat * Box::cube_vertices = new GLfloat[48];


GLubyte * Box::frontIndices = new GLubyte[4];
GLubyte * Box::backIndices = new GLubyte[4];
GLubyte * Box::rightIndices = new GLubyte[4];
GLubyte * Box::leftIndices = new GLubyte[4];
GLubyte * Box::topIndices = new GLubyte[4];
GLubyte * Box::bottomIndices = new GLubyte[4];

Box::Box() {
  width = 1;
  height = 1;
  depth = 1;

}

void Box::init() {
	int i = 0;

	//Vert 0
	cube_vertices[i++]=-1.0; cube_vertices[i++]=-1.0;cube_vertices[i++]=1.0;
	cube_vertices[i++]=1.0; cube_vertices[i++]=0.0;cube_vertices[i++]=0.0;
	//Vert 1
	cube_vertices[i++]=1.0; cube_vertices[i++]=-1.0;cube_vertices[i++]=1.0;
	cube_vertices[i++]=0.5; cube_vertices[i++]=0.5;cube_vertices[i++]=0.0;
	//Vert 2
	cube_vertices[i++]=1.0; cube_vertices[i++]=1.0;cube_vertices[i++]=1.0;
	cube_vertices[i++]=0.0; cube_vertices[i++]=1.0;cube_vertices[i++]=0.0;
	//Vert 3
	cube_vertices[i++]=-1.0; cube_vertices[i++]=1.0;cube_vertices[i++]=1.0;
	cube_vertices[i++]=0.0; cube_vertices[i++]=0.5;cube_vertices[i++]=0.5;
	//Vert 4
	cube_vertices[i++]=-1.0; cube_vertices[i++]=-1.0;cube_vertices[i++]=-1.0;
	cube_vertices[i++]=0.0; cube_vertices[i++]=0.0;cube_vertices[i++]=1.0;
	//Vert 5
	cube_vertices[i++]=1.0; cube_vertices[i++]=-1.0;cube_vertices[i++]=-1.0;
	cube_vertices[i++]=0.5; cube_vertices[i++]=0.0;cube_vertices[i++]=0.5;
	//Vert 6
	cube_vertices[i++]=1.0; cube_vertices[i++]=1.0;cube_vertices[i++]=-1.0;
	cube_vertices[i++]=1.0; cube_vertices[i++]=0.0;cube_vertices[i++]=0.0;
	//Vert 7
	cube_vertices[i++]=-1.0; cube_vertices[i++]=1.0;cube_vertices[i++]=-1.0;
	cube_vertices[i++]=0.5; cube_vertices[i++]=0.5;cube_vertices[i++]=0.0;

	i = 0;

	frontIndices[i++] = 0; frontIndices[i++] = 1;
	frontIndices[i++] = 2; frontIndices[i++] = 3;

	i = 0;
	//backIndices = new GLubyte[4];
	backIndices[i++] = 5; backIndices[i++] = 4;
	backIndices[i++] = 7; backIndices[i++] = 6;

	i = 0;
	//rightIndices = new GLubyte[4];
	rightIndices[i++] = 1; rightIndices[i++] = 5;
	rightIndices[i++] = 6; rightIndices[i++] = 2;

	i = 0;
	//leftIndices = new GLubyte[4];
	leftIndices[i++] = 4; leftIndices[i++] = 0;
	leftIndices[i++] = 3; leftIndices[i++] = 7;

	i = 0;
	//topIndices = new GLubyte[4];
	topIndices[i++] = 3; topIndices[i++] = 2;
	topIndices[i++] = 6; topIndices[i++] = 7;

	i = 0;
	//bottomIndices = new GLubyte[4];
	bottomIndices[i++] = 4; bottomIndices[i++] = 5;
	bottomIndices[i++] = 1; bottomIndices[i++] = 0;

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

		//row(pts, ptsIt) = prod(rotMat, row(pts, ptsIt) );

		xVal += xDelta;
		ptsIt++;
	}

	while (ptsIt < numPts/2) {
		pts(ptsIt, 0) = xVal;
		pts(ptsIt, 1) = yVal;
		pts(ptsIt, 2) = 0;
		pts(ptsIt, 3) = 1;

		//row(pts, ptsIt) = prod(rotMat, row(pts, ptsIt) );

		yVal -= yDelta;
		ptsIt++;
	}

	while (ptsIt < 3*numPts/4) {
		pts(ptsIt, 0) = xVal;
		pts(ptsIt, 1) = yVal;
		pts(ptsIt, 2) = 0;
		pts(ptsIt, 3) = 1;

		//row(pts, ptsIt) = prod(rotMat, row(pts, ptsIt) );

		xVal -= xDelta;
		ptsIt++;
	}

	while (ptsIt < numPts) {
		pts(ptsIt, 0) = xVal;
		pts(ptsIt, 1) = yVal;
		pts(ptsIt, 2) = 0;
		pts(ptsIt, 3) = 1;

		//row(pts, ptsIt) = prod(rotMat, row(pts, ptsIt) );

		yVal += yDelta;
		ptsIt++;
	}



	return pts;
}

void Box::drawUnit() {
  glutWireCube( width );
}

void Box::drawScale() {
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

/*
void Box::drawUnit() {
	  glEnableClientState( GL_VERTEX_ARRAY ) ;
	  glEnableClientState( GL_COLOR_ARRAY ) ;

	  glVertexPointer( 3, GL_FLOAT, 6*sizeof(GLfloat), cube_vertices ) ;
	  glColorPointer(  3, GL_FLOAT, 6*sizeof(GLfloat), &cube_vertices[3] ) ;

	  glDrawElements( GL_QUADS, 4, GL_UNSIGNED_BYTE, frontIndices ) ;
	  glDrawElements( GL_QUADS, 4, GL_UNSIGNED_BYTE, backIndices ) ;
	  glDrawElements( GL_QUADS, 4, GL_UNSIGNED_BYTE, rightIndices ) ;
	  glDrawElements( GL_QUADS, 4, GL_UNSIGNED_BYTE, leftIndices ) ;
	  glDrawElements( GL_QUADS, 4, GL_UNSIGNED_BYTE, topIndices ) ;
	  glDrawElements( GL_QUADS, 4, GL_UNSIGNED_BYTE, bottomIndices ) ;

	  glDisableClientState( GL_VERTEX_ARRAY);
	  glDisableClientState( GL_COLOR_ARRAY);
}
*/
