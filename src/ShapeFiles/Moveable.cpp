/*
 * Moveable.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#include "Moveable.h"
#include "../Physics/WorldSettings.h"

#include <iostream>

using namespace std;
using namespace boost::numeric::ublas;

//TODO Make sure I'm supposed to actually be referencing the 0 here...
boost::numeric::ublas::compressed_vector<Moveable *> Moveable::moveables(0);


Moveable::Moveable()
          :pos({0.0,0.0,0.0,1.0})
          ,color({0.5,1.0,0.0})
          ,mass(1)
          ,density(1)
          ,ptsHighlighted(true)
{
	sgMakeIdentQuat(orientationQuat);

	for (int i = 0; i < 4; i++) {
		momentum[i] = 0;
		angMomentum[i] = 0;
		angVelocity[i] = 0;
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				orientationMat[i][j] = 1;
			}
			else {
			}
		}
	}

}

Moveable::~Moveable() {
	//cout << "Killing Moveable" << endl;
}


void Moveable::draw() {
	glPushMatrix();

	//Translate
	glTranslatef(pos[0], pos[1], pos[2]);

	//Rotate
	glMultMatrixf( (const GLfloat*)orientationMat);


	//Scale
	drawScale();

	glColor3fv(color);

	drawUnit();

	glPopMatrix();

}


void Moveable::drawScale() {}
void Moveable::drawUnit() {}

void Moveable::setPos(float inX, float inY, float inZ) {
	pos[0] = inX;
	pos[1] = inY;
	pos[2] = inZ;

}

void Moveable::setPos(sgVec4 newPos) {
	sgCopyVec4(this->pos, newPos);
}

void Moveable::adjustPos(float dx, float dy, float dz) {
	pos[0] += dx;
	pos[1] += dy;
	pos[2] += dz;
}

void Moveable::adjustPos(sgVec4 dPos) {
	pos[0] += dPos[0];
	pos[2] += dPos[1];
	pos[1] += dPos[2];
}


sgVec4 *  Moveable::getPos(){
	sgVec4 * retPos = new sgVec4[1];
	sgCopyVec4( (*retPos), pos);
	return retPos;
}

void Moveable::getPos(sgVec4 retVec) {
	sgCopyVec4(retVec, pos);
}

/*
void Moveable::hFlip() {
	scaleMat(0,0) = -1;
	scaleMat(1,1) = 1;

}

void Moveable::vFlip() {
	scaleMat(0,0) = 1;
	scaleMat(1,1) = -1;

}
*/


void Moveable::setAngle(float xAngle, float yAngle, float zAngle) {
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

void Moveable::adjustAngle(const SGfloat dAngle, const sgVec3 rotAxis) {
	sgQuat tempRotQuat;
	sgAngleAxisToQuat(tempRotQuat, dAngle,  rotAxis);
	//sgRotQuat(orientationQuat, dAngle, rotAxis);

	sgPostMultQuat(orientationQuat, tempRotQuat);
	sgQuatToMatrix(orientationMat, orientationQuat);

}


void Moveable::setMomentum(float inX, float inY, float inZ) {
	momentum[0] = inX;
	momentum[1] = inY;
	momentum[2] = inZ;
	momentum[3] = 0;
	sgCopyVec4(prevMomentum, momentum);
}

void Moveable::setMomentum(sgVec4 newMomentum) {
	sgCopyVec4(momentum, newMomentum);
	sgCopyVec4(prevMomentum, momentum);
}

void Moveable::adjustMomentum(float dx, float dy, float dz) {
	momentum[0] += dx;
	momentum[1] += dy;
	momentum[2] += dz;
}

void Moveable::adjustMomentum(sgVec4 dMomentum) {
	sgAddVec4(momentum, dMomentum);
}

sgVec4 * Moveable::getMomentum() {
	sgVec4 * retMomentum = new sgVec4[1];
	sgCopyVec4( (*retMomentum), momentum);
	return retMomentum;
}

void Moveable::getMomentum(sgVec4 retVec) {
	sgCopyVec4(retVec, momentum);
}



void Moveable::setVelocity(float inX, float inY, float inZ) {
	momentum[0] = inX * mass;
	momentum[1] = inY * mass;
	momentum[2] = inZ * mass;
	momentum[3] = 0;
	sgCopyVec4(prevMomentum, momentum);
}

void Moveable::setVelocity(sgVec4 newVel) {
	momentum[0] = newVel[0] * mass;
	momentum[1] = newVel[1] * mass;
	momentum[2] = newVel[2] * mass;
	momentum[3] = 0;
	sgCopyVec4(prevMomentum, momentum);

}
void Moveable::adjustVelocity(float dx, float dy, float dz) {
	momentum[0] += dx/mass;
	momentum[1] += dx/mass;
	momentum[2] += dx/mass;
}
void Moveable::adjustVelocity(sgVec4 dVel) {
	sgVec4 tempVec;
	sgScaleVec4(tempVec, momentum, 1/mass);
	sgAddVec4(tempVec, dVel);
	sgScaleVec4(momentum, tempVec, mass);
}

sgVec4 * Moveable::getVelocity() {
	sgVec4 * velocity = new sgVec4[1];
	sgScaleVec4( (*velocity), momentum, 1/mass);
	return velocity;
}

void Moveable::getVelocity(sgVec4 retVec) {
	sgVec4 tempVec;
	sgCopyVec4(tempVec, momentum);
	sgScaleVec4(tempVec, 1/mass);
	sgCopyVec4(retVec, tempVec);
}

// Angular Momentum and Velocity
float Moveable::getMomentOfInertia() { return 1;}

void Moveable::setAngMomentum(sgVec4 newAngMomentum) {
	float I = getMomentOfInertia();
	sgCopyVec4(angMomentum, newAngMomentum);
	sgCopyVec4(angVelocity, angMomentum);
	sgScaleVec4(angVelocity, 1.0/I);
	sgCopyVec4(prevAngVelocity, angVelocity);

}

void Moveable::adjustAngMomentum(sgVec4 dAngMomentum) {
	//TODO generalize for other shapes
	float I = getMomentOfInertia();

	sgAddVec4(angMomentum, dAngMomentum);
	sgCopyVec4(angVelocity, angMomentum);
	sgScaleVec4(angVelocity, 1.0/I);
}

void Moveable::getAngMomentum(sgVec4 retVec) {
	sgCopyVec4(retVec, angMomentum);
}

void Moveable::setAngVelocity(sgVec4 newAngVelocity) {
	float I = getMomentOfInertia();
	sgCopyVec4(angVelocity, newAngVelocity);
	sgCopyVec4(angMomentum, angVelocity);
	sgScaleVec4(angMomentum, I);
	sgCopyVec4(prevAngVelocity, angVelocity);
}

void Moveable::adjustAngVelocity(sgVec4 dAngVelocity) {
	float I = getMomentOfInertia();
	sgAddVec4(angVelocity, dAngVelocity);
	sgCopyVec4(angMomentum, angVelocity);
	sgScaleVec4(angMomentum, I);
}

void Moveable::getAngVelocity(sgVec4 retAngVelocity) {
	sgCopyVec4(retAngVelocity, angVelocity);
}



void Moveable::setMass(float newMass) {
	mass = newMass;
}

void Moveable::adjustMass(float dMass) {
	mass += dMass;
}

float Moveable::getMass() {
	return mass;
}


void Moveable::setDensity(float newDensity) {
	density = newDensity;
}

float Moveable::getDensity() {
	return density;
}

void Moveable::setColor(sgVec3 newColor) {
	sgCopyVec3(color, newColor);

}
void Moveable::getColor(sgVec3 retVec) {
	sgCopyVec3(retVec, color);
}

void Moveable::update(float dt) {

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
void Moveable::getUnitVecTo(Moveable * destination, sgVec4 unitv) {
	sgSubVec4(unitv, pos, destination->pos);
	sgNormaliseVec4(unitv);
}
*/

Moveable * Moveable::getMoveableFromList( unsigned int moveableIndex )
{
  Moveable * returnMoveable = NULL;
  if ( moveables.size() > moveableIndex )
  {
    returnMoveable = moveables(moveableIndex);
  }

  return returnMoveable;
}

int Moveable::addMoveableToList( Moveable * insertMoveable )
{
  int curSize = moveables.size();
  moveables.resize(curSize + 1);
  moveables(curSize) = insertMoveable;
  return curSize;
}

void Moveable::removeMoveableFromList( int moveableIndex )
{
  //
  Moveable * moveableToremove = Moveable::moveables(moveableIndex);
  moveableToremove->~Moveable();
  Moveable::moveables.erase_element(moveableIndex);
  // TODO Decide how to best erase/resize
  // boost::numeric::ublas::compressed_vector<Moveable *> newListA;
  // newListA = moveables.subVector(0,i);
  // boost::numeric::ublas::compressed_vector<Moveable *> newListB;
  // newListB = moveables.subVector(i, moveables.size() );
  // moveables = newListA + newListB;
}


void Moveable::clearMoveables() {
	for (int i = Moveable::moveables.size() - 1; i > -1; i--) {
      cout << "Moveable # " << i << endl;
    Moveable * moveableToremove = Moveable::moveables(i);
    moveableToremove->~Moveable();
    Moveable::moveables.erase_element(i);
	}
	Moveable::moveables.resize(0);
}

