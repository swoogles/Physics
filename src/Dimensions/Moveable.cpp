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

shared_ptr<VecStruct> Moveable::getPosNew() const {
	shared_ptr<VecStruct> vecStruct = make_shared<VecStruct>();
    sgCopyVec4(vecStruct->vec, pos);
    return vecStruct;
}

vecPtr Moveable::getVectorToObject( Moveable * object2) {
	vecPtr vec(new VecStruct());
	vecPtr pos1(this->getPosNew());
	vecPtr pos2(object2->getPosNew());
	sgSubVec4(vec->vec, pos2->vec, pos1->vec);
	return vec;
}

shared_ptr<MatrixStruct> Moveable::getOrientationMat() const {
	shared_ptr<MatrixStruct> matrixStruct = make_shared<MatrixStruct>();

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

void Moveable::adjustVelocity(const sgVec4 dVel) {
	sgVec4 tempVec;
	sgScaleVec4(tempVec, momentum, 1/mass);
	sgAddVec4(tempVec, dVel);
	sgScaleVec4(momentum, tempVec, mass);
}

vecPtr Moveable::getVelocity() const {
	vecPtr vecStruct(new VecStruct());
	sgCopyVec4(vecStruct->vec, momentum);
	sgScaleVec4(vecStruct->vec, 1/mass);

	return vecStruct;
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
