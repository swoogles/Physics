#include "Moveable.h"

Moveable::Moveable()
           :pos{0.0,0.0,0.0,1.0}
          ,momentum{0, 0, 0, 0}
          ,angMomentum{0, 0, 0, 0}
          ,angVelocity{0, 0, 0, 0}
          ,mass(1)
          ,density(1)
          ,color{0.5,1.0,0.0}
{
	sgMakeIdentQuat(orientationQuat);

	for (int i = 0; i < 4; i++) {
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

VecStruct * Moveable::getPosNew() const {
	VecStruct * vecStruct = new VecStruct();
    sgCopyVec4(vecStruct->vec, pos);
    return vecStruct;
}

void Moveable::adjustAngle(const SGfloat dAngle, const sgVec3 rotAxis) {
	sgQuat tempRotQuat;
	sgAngleAxisToQuat(tempRotQuat, dAngle,  rotAxis);
	//sgRotQuat(orientationQuat, dAngle, rotAxis);

	sgPostMultQuat(orientationQuat, tempRotQuat);
	sgQuatToMatrix(orientationMat, orientationQuat);
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
float Moveable::getMomentOfInertia() { return 1;}

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
