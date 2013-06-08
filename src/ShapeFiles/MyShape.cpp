/*
 * MyShape.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#include "MyShape.h"
#include "../Physics/WorldSettings.h"

#include <iostream>

using namespace std;
using namespace boost::numeric::ublas;

float MyShape::G = 6.67384e-11;

boost::numeric::ublas::vector<MyShape *> MyShape::shapes(0);



MyShape::MyShape() {
	numPts = 16;
	//pos.resize(4);
	//toolVec.resize(4);

	pts.resize(numPts, 4);
	//color.resize(3);

	//scaleMat.resize(4,4);
	//rotMat.resize(4,4);

	pos[0] = 0; pos[1] = 0; pos[2] = 0; pos[3] = 1;
	color[0] = 0.25; color[1] = 0.0; color[2] = 0.0;


	sgMakeIdentQuat(orientationQuat);
	mass = 1;
	density = 1;

	for (int i = 0; i < 4; i++) {
		momentum[i] = 0;
		angMomentum[i] = 0;
		angVelocity[i] = 0;
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				//scaleMat(i,j) = 1;
				//rotMat(i,j) = 1;
				orientationMat[i][j] = 1;
			}
			else {
				//scaleMat(i,j) = 0;
				//rotMat(i,j) = 0;
			}
		}
	}

	ptsHighlighted = true;
}

MyShape::~MyShape() {
	//cout << "Killing MyShape" << endl;
}

matrix<double> MyShape::getPts() {
	return pts;
}

void MyShape::printPts() {
	cout << pts << endl;
}

void MyShape::draw() {
	glPushMatrix();

	//Translate
	glTranslatef(pos[0], pos[1], pos[2]);

	//Rotate
	glMultMatrixf( (const GLfloat*)orientationMat);


	//Scale
	drawScale();

	/*
	bool changed = false;
	static float model[16], projection[16], final[16];
	glGetFloatv(GL_MODELVIEW, model);

	for (int i = 0; i < 16; i++)
		if (model[i] != projection[i])
			changed = true;

	if (changed)
		for (int i = 0; i < 4; i++) {
			cout << endl;
			for (int j = 0; j < 4; j++) {
				cout << model[j+i*4] << "  ";
			}
		}

	glGetFloatv(GL_MODELVIEW, projection);
	*/

	glColor3fv(color);

	drawUnit();

	glPopMatrix();

	/*
	this->getPts();

	glBegin(GL_POLYGON);
	{
		glColor3f(0,0,1);
		for (unsigned int i = 0; i < pts.size1(); i++){
			//cout << "PersMat: " << endl << perspectiveMat << endl;

			//cout << "Orig Point:" << row(pts, i) << endl;
			//toolVec = prod(worldRotFrame, row(pts, i));
			//cout << "After rot: " << toolVec << endl;
			//toolVec = prod(perspectiveMat, row(pts,i));
			//cout << "After persp" << toolVec << endl << endl;


			//glVertex3f(pts(curPt,0), pts(curPt,1), pts(curPt,2));
			toolVec = row(pts,i);
			glVertex3f(toolVec(0), toolVec(1), toolVec(2));
			curPt++;
		}


	}
	glEnd();


	if (ptsHighlighted) {
		markSize = this->getMarkerSize();
		//cout << "Marksize: " << markSize << endl;
		glBegin(GL_QUADS);
		glColor3f(1,1,1);
		for (unsigned int i = 0; i < pts.size1(); i++) {
			glColor3f(red, green, 0);
			toolVec = row(pts,i);
			//toolVec = prod(worldRotFrame, row(pts, i));
			//toolVec = prod(perspectiveMat, row(pts,i));

			glVertex3f(toolVec(0) - markSize, toolVec(1) + markSize, toolVec(2));
			glVertex3f(toolVec(0) + markSize, toolVec(1) + markSize, toolVec(2));
			glVertex3f(toolVec(0) + markSize, toolVec(1) - markSize, toolVec(2));
			glVertex3f(toolVec(0) - markSize, toolVec(1) - markSize, toolVec(2));

			red -= 1.0/pts.size1();
			green += 1.0/pts.size1();


			//Make coordinate marks
		}
		glEnd();
	}
	*/
}


void MyShape::drawScale() {}
void MyShape::drawUnit() {}

/*
void MyShape::scale(float sizeFactor) {

}


void MyShape::scaleMembers(float) {
};


void MyShape::scaleMembers(float xFac, float yFac) {
};
*/




void MyShape::setPos(float inX, float inY, float inZ) {
	pos[0] = inX;
	pos[1] = inY;
	pos[2] = inZ;

}

void MyShape::setPos(sgVec4 newPos) {
	sgCopyVec4(pos, newPos);
}

void MyShape::adjustPos(float dx, float dy, float dz) {
	pos[0] += dx;
	pos[1] += dy;
	pos[2] += dz;
}

void MyShape::adjustPos(sgVec4 dPos) {
	pos[0] += dPos[0];
	pos[2] += dPos[1];
	pos[1] += dPos[2];
}


sgVec4 *  MyShape::getPos(){
	sgVec4 * retPos = new sgVec4[1];
	sgCopyVec4( (*retPos), pos);
	return retPos;
}

void MyShape::getPos(sgVec4 retVec) {
	sgCopyVec4(retVec, pos);
}

/*
void MyShape::hFlip() {
	scaleMat(0,0) = -1;
	scaleMat(1,1) = 1;

}

void MyShape::vFlip() {
	scaleMat(0,0) = 1;
	scaleMat(1,1) = -1;

}
*/

float MyShape::getMarkerSize() {
	return 0;
}





void MyShape::setAngle(float xAngle, float yAngle, float zAngle) {
	sgMakeIdentQuat(orientationQuat);

	sgQuat tempQuat;
	sgVec3 xAxis, yAxis, zAxis;
	xAxis[0] = 1; xAxis[1] = 0; xAxis[2] = 0;
	yAxis[0] = 0; yAxis[1] = 1; yAxis[2] = 0;
	zAxis[0] = 0; zAxis[1] = 0; zAxis[2] = 1;

	sgAngleAxisToQuat(tempQuat, xAngle, xAxis);
	sgPostMultQuat(orientationQuat, tempQuat);

	sgAngleAxisToQuat(tempQuat, yAngle, yAxis);
	sgPostMultQuat(orientationQuat, tempQuat);

	sgAngleAxisToQuat(tempQuat, zAngle, zAxis);
	sgPostMultQuat(orientationQuat, tempQuat);

	sgQuatToMatrix(orientationMat, orientationQuat);
}

void MyShape::adjustAngle(const SGfloat dAngle, const sgVec3 rotAxis) {
	sgQuat tempRotQuat;
	sgAngleAxisToQuat(tempRotQuat, dAngle,  rotAxis);
	//sgRotQuat(orientationQuat, dAngle, rotAxis);

	sgPostMultQuat(orientationQuat, tempRotQuat);
	sgQuatToMatrix(orientationMat, orientationQuat);

}


void MyShape::setMomentum(float inX, float inY, float inZ) {
	momentum[0] = inX;
	momentum[1] = inY;
	momentum[2] = inZ;
	momentum[3] = 0;
	sgCopyVec4(prevMomentum, momentum);
}

void MyShape::setMomentum(sgVec4 newMomentum) {
	sgCopyVec4(momentum, newMomentum);
	sgCopyVec4(prevMomentum, momentum);
}

void MyShape::adjustMomentum(float dx, float dy, float dz) {
	momentum[0] += dx;
	momentum[1] += dy;
	momentum[2] += dz;
}

void MyShape::adjustMomentum(sgVec4 dMomentum) {
	sgAddVec4(momentum, dMomentum);
}

sgVec4 * MyShape::getMomentum() {
	sgVec4 * retMomentum = new sgVec4[1];
	sgCopyVec4( (*retMomentum), momentum);
	return retMomentum;
}

void MyShape::getMomentum(sgVec4 retVec) {
	sgCopyVec4(retVec, momentum);
}



void MyShape::setVelocity(float inX, float inY, float inZ) {
	momentum[0] = inX * mass;
	momentum[1] = inY * mass;
	momentum[2] = inZ * mass;
	momentum[3] = 0;
	sgCopyVec4(prevMomentum, momentum);
}

void MyShape::setVelocity(sgVec4 newVel) {
	momentum[0] = newVel[0] * mass;
	momentum[1] = newVel[1] * mass;
	momentum[2] = newVel[2] * mass;
	momentum[3] = 0;
	sgCopyVec4(prevMomentum, momentum);

}
void MyShape::adjustVelocity(float dx, float dy, float dz) {
	momentum[0] += dx/mass;
	momentum[1] += dx/mass;
	momentum[2] += dx/mass;
}
void MyShape::adjustVelocity(sgVec4 dVel) {
	sgVec4 tempVec;
	sgScaleVec4(tempVec, momentum, 1/mass);
	sgAddVec4(tempVec, dVel);
	sgScaleVec4(momentum, tempVec, mass);
}

sgVec4 * MyShape::getVelocity() {
	sgVec4 * velocity = new sgVec4[1];
	sgScaleVec4( (*velocity), momentum, 1/mass);
	return velocity;
}

void MyShape::getVelocity(sgVec4 retVec) {
	sgVec4 tempVec;
	sgCopyVec4(tempVec, momentum);
	sgScaleVec4(tempVec, 1/mass);
	sgCopyVec4(retVec, tempVec);
}

// Angular Momentum and Velocity
float MyShape::getMomentOfInertia() { return 1;}

void MyShape::setAngMomentum(sgVec4 newAngMomentum) {
	float I = getMomentOfInertia();
	sgCopyVec4(angMomentum, newAngMomentum);
	sgCopyVec4(angVelocity, angMomentum);
	sgScaleVec4(angVelocity, 1.0/I);
	sgCopyVec4(prevAngVelocity, angVelocity);

}

void MyShape::adjustAngMomentum(sgVec4 dAngMomentum) {
	//TODO generalize for other shapes
	float I = getMomentOfInertia();

	sgAddVec4(angMomentum, dAngMomentum);
	sgCopyVec4(angVelocity, angMomentum);
	sgScaleVec4(angVelocity, 1.0/I);
}

void MyShape::getAngMomentum(sgVec4 retVec) {
	sgCopyVec4(retVec, angMomentum);
}

void MyShape::setAngVelocity(sgVec4 newAngVelocity) {
	float I = getMomentOfInertia();
	sgCopyVec4(angVelocity, newAngVelocity);
	sgCopyVec4(angMomentum, angVelocity);
	sgScaleVec4(angMomentum, I);
	sgCopyVec4(prevAngVelocity, angVelocity);
}

void MyShape::adjustAngVelocity(sgVec4 dAngVelocity) {
	float I = getMomentOfInertia();
	sgAddVec4(angVelocity, dAngVelocity);
	sgCopyVec4(angMomentum, angVelocity);
	sgScaleVec4(angMomentum, I);
}

void MyShape::getAngVelocity(sgVec4 retAngVelocity) {
	sgCopyVec4(retAngVelocity, angVelocity);
}



void MyShape::setMass(float newMass) {
	mass = newMass;
}

void MyShape::adjustMass(float dMass) {
	mass += dMass;
}

float MyShape::getMass() {
	return mass;
}


void MyShape::setDensity(float newDensity) {
	density = newDensity;
}

float MyShape::getDensity() {
	return density;
}


void MyShape::setColor(sgVec3 newColor) {
	sgCopyVec3(color, newColor);

}
void MyShape::getColor(sgVec3 retVec) {
	sgCopyVec3(retVec, color);
}

void MyShape::calcColor() {
  float totalMass=WorldSettings::getTotalMass();
  // cout << "totalMass:" << totalMass << endl;

	float redAmount = 0.25 + mass / (totalMass/3.0);
	if (redAmount > 1.0)
		redAmount = 1.0;

	float greenAmount = mass / (0.8 *totalMass);
	if (greenAmount > 1.0) {
		greenAmount = 1.0;
	}


	color[0] = redAmount;
	color[1] = greenAmount;
}

void MyShape::update(float dt) {

	sgVec4 velocity;
	sgVec4 prevVelocity;
	sgScaleVec4(velocity, momentum, 1/mass);
	sgScaleVec4(prevVelocity, prevMomentum, 1/mass);
	sgAddVec4(velocity, prevVelocity); //Sum current and previous velocities
	sgScaleVec4(velocity, .5); //Get avg velocity
	sgScaleVec4(velocity, dt);
	sgAddVec4(pos, velocity);

	sgVec3 rotVec;
	rotVec[0] = 1; rotVec[1] = 0; rotVec[2] = 0;
	adjustAngle( (prevAngVelocity[0] + angVelocity[0]) * .5 *dt, rotVec);
	rotVec[0] = 0; rotVec[1] = 1; rotVec[2] = 0;
	adjustAngle( (prevAngVelocity[1] + angVelocity[1]) * .5 *dt, rotVec);
	rotVec[0] = 0; rotVec[1] = 0; rotVec[2] = 1;
	adjustAngle( (prevAngVelocity[2] + angVelocity[2]) * .5 *dt, rotVec);

	sgCopyVec4(prevMomentum, momentum);
	sgCopyVec4(prevAngVelocity, angVelocity);

}

/*
void MyShape::getUnitVecTo(MyShape * destination, sgVec4 unitv) {
	sgSubVec4(unitv, pos, destination->pos);
	sgNormaliseVec4(unitv);
}
*/

void MyShape::clearShapes() {
	for (int i = MyShape::shapes.size() - 1; i > -1; i--) {
      cout << "Shape # " << i << endl;
		MyShape::shapes(i)->~MyShape();
		MyShape::shapes.erase_element(i);
	}
	MyShape::shapes.resize(0);
}


void MyShape::setRadius(float) {}
float MyShape::getRadius() { return 1;}

int MyShape::getType() {}
