#include "Moveable.h"

Moveable::Moveable(VecStruct momentum)
           :pos{0.0,0.0,0.0,true}
           ,orientationQuat{0, 0, 0, 1.0} // Identity Quaternion
           ,orientationMat{{1, 0, 0, 0},
                           {0, 1, 0, 0},
                           {0, 0, 1, 0},
                           {0, 0, 0, 1}}
          ,momentum(momentum)
          ,angMomentum{0, 0, 0, false}
          ,angVelocity{0, 0, 0, false}
          ,mass(1)
          ,density(1)
          ,color{0.5,1.0,0.0}
{}

void Moveable::setPos(float inX, float inY, float inZ) {
	pos.vec[0] = inX;
	pos.vec[1] = inY;
	pos.vec[2] = inZ;
}

void Moveable::setPos(VecStruct newPos) {
    this->pos = newPos;
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

void Moveable::adjustAngle(SGfloat dAngle, const VecStruct rotAxis) {
	sgQuat tempRotQuat;
	sgAngleAxisToQuat(tempRotQuat, dAngle,  rotAxis.vec);
	//sgRotQuat(orientationQuat, dAngle, rotAxis);

	sgPostMultQuat(orientationQuat, tempRotQuat);
	sgQuatToMatrix(orientationMat, orientationQuat);
}

void Moveable::adjustVelocity(VecStruct dVel) {
	this->momentum = momentum.scaledBy(1/mass.value()).plus(dVel).scaledBy(mass.value());
}

VecStruct Moveable::getVelocity() {
	return momentum.scaledBy(1/mass.value());
}

// Angular Momentum and Velocity
double Moveable::getMomentOfInertia() { return 1;}

void Moveable::setAngVelocity(VecStruct newAngVelocity) {
	this->prevAngVelocity = angVelocity;
	double I = getMomentOfInertia();
	this->angVelocity = newAngVelocity;
	this->angMomentum = angVelocity.scaledBy(I);
}

void Moveable::adjustAngVelocity(VecStruct dAngVelocity) {
	double I = getMomentOfInertia();
	VecStruct dAngV(dAngVelocity);
	this->angVelocity = angVelocity.plus(dAngV);
	this->angMomentum = angVelocity.scaledBy(I);
}

void Moveable::update(float dt) {
	VecStruct prevVelocity(prevMomentum.scaledBy(1/mass.value()));
	auto dPos =
			this->getVelocity()
			.plus(prevVelocity)
			.scaledBy(.5) //Get avg velocity
			.scaledBy(dt);
	this->pos = pos.plus(dPos);

	VecStruct rotVec;
	rotVec = VecStruct(1,  0, 0);
	adjustAngle( (prevAngVelocity.x() + angVelocity.x()) * .5 *dt, rotVec);
	rotVec = VecStruct(0,  1, 0);
	adjustAngle( (prevAngVelocity.y() + angVelocity.y()) * .5 *dt, rotVec);
	rotVec = VecStruct(0,  0, 1);
	adjustAngle( (prevAngVelocity.z() + angVelocity.z()) * .5 *dt, rotVec);

	this->prevMomentum = momentum;
	this->prevAngVelocity = angVelocity;

}
