/*
 * Moveable.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#include "Moveable.h"

using namespace std;
using namespace boost::numeric::ublas;

ostream& operator<<(ostream& os, sgVec4 outputVec) {
	cout << "<" << outputVec[0] << ", " << outputVec[1] << ", " << outputVec[2] << ">";
	return os;
}

Moveable::Moveable()
          // :pos({0.0,0.0,0.0,1.0})
          :mass(1)
          ,density(1)
          // ,color({0.5,1.0,0.0})
          ,ptsHighlighted(true)
{
  pos[0]=0.0;
  pos[1]=0.0;
  pos[2]=0.0;
  pos[3]=1.0;
  color[0]=0.5;
  color[1]=1.0;
  color[2]=0.0;
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
        orientationMat[i][j] = 0;
			}
		}
	}

}

Moveable::~Moveable() {
	//cout << "Killing Moveable" << endl;
}

void Moveable::setPos(float inX, float inY, float inZ) {
	pos[0] = inX;
	pos[1] = inY;
	pos[2] = inZ;
}

void Moveable::setPos(const sgVec4 newPos) {
	sgCopyVec4(this->pos, newPos);
}

void Moveable::adjustPos(float dx, float dy, float dz) {
	pos[0] += dx;
	pos[1] += dy;
	pos[2] += dz;
}

void Moveable::adjustPos(const sgVec4 dPos) {
	pos[0] += dPos[0];
	pos[2] += dPos[1];
	pos[1] += dPos[2];
}


void Moveable::getPos(sgVec4 retVec) const {
	sgCopyVec4(retVec, pos);
}

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

void Moveable::setVelocity(float inX, float inY, float inZ) {
	momentum[0] = inX * mass;
	momentum[1] = inY * mass;
	momentum[2] = inZ * mass;
	momentum[3] = 0;
	sgCopyVec4(prevMomentum, momentum);
}

void Moveable::setVelocity(const sgVec4 newVel) {
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
void Moveable::adjustVelocity(const sgVec4 dVel) {
	sgVec4 tempVec;
	sgScaleVec4(tempVec, momentum, 1/mass);
	sgAddVec4(tempVec, dVel);
	sgScaleVec4(momentum, tempVec, mass);
}

void Moveable::getVelocity(sgVec4 retVec) const {
	sgVec4 tempVec;
	sgCopyVec4(tempVec, momentum);
	sgScaleVec4(tempVec, 1/mass);
	sgCopyVec4(retVec, tempVec);
}

// Angular Momentum and Velocity
float Moveable::getMomentOfInertia() const { return 1;}

void Moveable::setAngVelocity(const sgVec4 newAngVelocity) {
	sgCopyVec4(prevAngVelocity, angVelocity);
	float I = getMomentOfInertia();
	sgCopyVec4(angVelocity, newAngVelocity);
	sgCopyVec4(angMomentum, angVelocity);
	sgScaleVec4(angMomentum, I);
}

void Moveable::adjustAngVelocity(const sgVec4 dAngVelocity) {
	float I = getMomentOfInertia();
	sgAddVec4(angVelocity, dAngVelocity);
	sgCopyVec4(angMomentum, angVelocity);
	sgScaleVec4(angMomentum, I);
}

void Moveable::getAngVelocity(sgVec4 retAngVelocity) const {
	sgCopyVec4(retAngVelocity, angVelocity);
}

float Moveable::getMass() const {
	return mass;
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

std::string Moveable::getPosString() const
{
  string posString = "<" + lexical_cast<std::string>( pos[0] )  + ", " + lexical_cast<std::string>( pos[1] ) + ", " + lexical_cast<std::string>( pos[2] ) + ">";
  return posString;
}


/*
void Moveable::getUnitVecTo(Moveable * destination, sgVec4 unitv) {
	sgSubVec4(unitv, pos, destination->pos);
	sgNormaliseVec4(unitv);
}
*/
