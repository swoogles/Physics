/*
 * Observer.cpp
 *
 *  Created on: Jul 20, 2011
 *      Author: brasure
 */

#include "Observer.h"

using namespace std;

boost::numeric::ublas::compressed_vector<Observer *> Observer::observers(0);
int Observer::curObserver;

void Observer::init() {
	curObserver = 0;
	//Observer::setCurObserver(0);
}

Observer::Observer() 
          :id( curObserver++ )
          , autoScale( true )
          , angVel( { 0.0, 0.0, 0.0 } )

{

	sgMakeIdentQuat(orientationQuat);

	sgQuatToMatrix(orientationMat, orientationQuat);

	float fov=45.0f; // in degrees
	float aspect= glutGet(GLUT_WINDOW_WIDTH);
	aspect /= glutGet(GLUT_WINDOW_HEIGHT);
	//float aspect=1.3333f;
	float znear=1.0f;
	float zfar=1e12f;
	perspectiveMat = new float [16];
	BuildPerspProjMat(perspectiveMat, fov, aspect, znear, zfar);

}

void Observer::getView() {
	glLoadMatrixf(perspectiveMat);

	glTranslatef(pos[0], pos[1], pos[2]);

	//Orientation below translate gives orbit around point
	//Orientation above translate gives pivot at point
	glMultMatrixf( (const GLfloat*)orientationMat);

	//glMultMatrixf( (const GLFloat*)orientationMat);

}



void Observer::zoomIn() {
	pos[2] *= .95;
}

void Observer::zoomOut() {
	pos[2] *= 1.05;
}


void Observer::adjustAngle(const SGfloat dAngle, const sgVec3 rotAxis) {
	sgQuat tempRotQuat;
	sgAngleAxisToQuat(tempRotQuat, dAngle,  rotAxis);
	//sgRotQuat(orientationQuat, dAngle, rotAxis);

	sgPostMultQuat(orientationQuat, tempRotQuat);
	sgQuatToMatrix(orientationMat, orientationQuat);

}

void Observer::setAngVel(float xAngVel, float yAngVel, float zAngVel) {

	angVel[0] = xAngVel;
	angVel[1] = yAngVel;
	angVel[2] = zAngVel;

}

void Observer::adjustAngVel(float dxAngVel, float dyAngVel, float dzAngVel) {
	angVel[0] = angVel[0] + dxAngVel;
	angVel[1] = angVel[1] + dyAngVel;
	angVel[2] = angVel[2] + dzAngVel;
}

void Observer::getAngVel(sgVec3 retVec) {
	sgCopyVec3(retVec, angVel);
}

void Observer::update(float dt) {
	sgVec3 rotVec;

	rotVec[0] = 1; rotVec[1] = 0; rotVec[2] = 0;
	adjustAngle(angVel[0], rotVec);
	rotVec[0] = 0; rotVec[1] = 1; rotVec[2] = 0;
	adjustAngle(angVel[1], rotVec);
	rotVec[0] = 0; rotVec[1] = 0; rotVec[2] = 1;
	adjustAngle(angVel[2], rotVec);
}

void Observer::setAutoScale(bool shouldScale) {
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

int Observer::getCurObserver() {
	return curObserver;
}
