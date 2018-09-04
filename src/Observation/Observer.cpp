/*
 * Observer.cpp
 *
 *  Created on: Jul 20, 2011
 *      Author: brasure
 */

#include "Observer.h"

using namespace std;

compressed_vector<Observer *> Observer::observers(0);
int Observer::curObserver;

void Observer::init() {
	curObserver = 0;
}

Observer::Observer() 
          :id( curObserver++ )
          , autoScale( true )

{
	sgMakeIdentQuat(orientationQuat);
	sgQuatToMatrix(orientationMat, orientationQuat);

	float fov=45.0f; // in degrees
	float aspect= glutGet(GLUT_WINDOW_WIDTH);
	aspect /= glutGet(GLUT_WINDOW_HEIGHT);
	float znear=1.0f;
	float zfar=1e12f;
	perspectiveMat = new float [16];
  Observer::BuildPerspProjMat(perspectiveMat, fov, aspect, znear, zfar);
}

void Observer::getView() {
	glLoadMatrixf(perspectiveMat);

	glTranslatef(pos[0], pos[1], pos[2]);

	//Orientation below translate gives orbit around point
	//Orientation above translate gives pivot at point
	glMultMatrixf( (const GLfloat*)orientationMat);
}

void Observer::zoomIn() {
	pos[2] *= .95;
}

void Observer::zoomOut() {
	pos[2] *= 1.05;
}

void Observer::setAutoScaling(bool shouldScale) {
	autoScale = shouldScale;
}

bool Observer::isAutoScaling() {
	return autoScale;
}

void Observer::setId(int newId) {
	id = newId;
}

int Observer::getId() {
	return id;
}

void Observer::setCurObserver(int newLooker) {
	//TODO Make sure newLooker is a valid observer
	curObserver = newLooker;
}

Observer * Observer::getCurObserver() {
	return observers(curObserver);
}

void Observer::update(float dt) {
  sgVec3 rotVec;

  rotVec[0] = 1; rotVec[1] = 0; rotVec[2] = 0;
  adjustAngle(angVelocity[0], rotVec);
  rotVec[0] = 0; rotVec[1] = 1; rotVec[2] = 0;
  adjustAngle(angVelocity[1], rotVec);
  rotVec[0] = 0; rotVec[1] = 0; rotVec[2] = 1;
  adjustAngle(angVelocity[2], rotVec);
}

// void Observer::setCurObserver(int newLooker) {
void Observer::calcMinPullback(float fov, float minX, float minY, float maxX, float maxY)
{
	  float absMaxX;
	  float absMaxY;
	  float pullBack;

	  if (abs(minX) > maxX)
    {
		  absMaxX = abs(minX);
    }
	  else
    {
		  absMaxX = maxX;
    }

	  if (abs(minY) > maxY)
    {
		  absMaxY = abs(minY);
    }
	  else
    {
		  absMaxY = maxY;
    }

	  if (absMaxY > absMaxX) {
		  pullBack = absMaxY / tan(fov * M_PI / 360);
	  }
	  else {
		  pullBack = absMaxX / tan(fov * M_PI / 360);
	  }

    setPos(0,0,-pullBack*2);
}

void Observer::BuildPerspProjMat(float *m, float fov, float aspect,
float znear, float zfar)
{
  float xymax = znear * tan(fov * M_PI/360.0);
  float ymin = -xymax;
  float xmin = -xymax;

  float width = xymax - xmin;
  float height = xymax - ymin;



  float depth = zfar - znear;
  float q = -(zfar + znear) / depth;
  float qn = -2 * (zfar * znear) / depth;

  float w = 2 * znear / width;
  w = w / aspect;
  float h = 2 * znear / height;

  m[0]  = w;
  m[1]  = 0;
  m[2]  = 0;
  m[3]  = 0;

  m[4]  = 0;
  m[5]  = h;
  m[6]  = 0;
  m[7]  = 0;

  m[8]  = 0;
  m[9]  = 0;
  m[10] = q;
  m[11] = -1;

  m[12] = 0;
  m[13] = 0;
  m[14] = qn;
  m[15] = 0;
}
