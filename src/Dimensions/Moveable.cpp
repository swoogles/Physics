#include "Moveable.h"

Moveable::Moveable()
           :pos{0.0,0.0,0.0,1.0}
           ,orientationQuat{0, 0, 0, 1.0} // Identity Quaternion
           ,orientationMat{{1, 0, 0, 0},
                           {0, 1, 0, 0},
                           {0, 0, 1, 0},
                           {0, 0, 0, 1}}
          ,momentum{0, 0, 0, 0}
          ,angMomentum{0, 0, 0, 0}
          ,angVelocity{0, 0, 0, 0}
          ,mass(1)
          ,density(1)
          ,color{0.5,1.0,0.0}
{}

void Moveable::setPos(float inX, float inY, float inZ) {
	pos[0] = inX;
	pos[1] = inY;
	pos[2] = inZ;
}

void Moveable::setPos(const sgVec4 newPos) {
	sgCopyVec4(this->pos, newPos);
}

VecStruct Moveable::getPos() {
    VecStruct retVec(pos);
	return retVec;
}


VecStruct Moveable::getVectorToObject(Moveable &object2) {
    return object2.getPos().minus(this->getPos());
}

unique_ptr<MatrixStruct> Moveable::getOrientationMat() const {
	unique_ptr<MatrixStruct> matrixStruct = make_unique<MatrixStruct>();

	sgCopyMat4(matrixStruct->orientationMat, orientationMat);
	return matrixStruct;
}

void Moveable::adjustAngle(const SGfloat dAngle, const sgVec3 rotAxis) {
	sgQuat tempRotQuat;
	sgAngleAxisToQuat(tempRotQuat, dAngle,  rotAxis);
	//sgRotQuat(orientationQuat, dAngle, rotAxis);

	sgPostMultQuat(orientationQuat, tempRotQuat);
	sgQuatToMatrix(orientationMat, orientationQuat);
}

void Moveable::adjustVelocity(VecStruct dVel) {
	VecStruct momentumVec(momentum);
	sgCopyVec4(
			momentum,
			momentumVec.scaledBy(1/mass).plus(dVel).scaledBy(mass).vec
	) ;
}

VecStruct Moveable::getVelocity() {
	VecStruct retVec(momentum);
	return retVec.scaledBy(1/mass);
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
