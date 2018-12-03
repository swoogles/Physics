#include "Moveable.h"

Moveable::Moveable(PhysicalVector momentum)
           :pos{0.0,0.0,0.0,true}
           ,orientationQuat{0, 0, 0, 1.0} // Identity Quaternion
           ,orientationMat{{1, 0, 0, 0},
                           {0, 1, 0, 0},
                           {0, 0, 1, 0},
                           {0, 0, 0, 1}}
          ,_momentum(momentum)
          ,angMomentum{0, 0, 0, false}
          ,angVelocity{0, 0, 0, false}
          ,_mass(1)
          ,_density(1)
          ,_color{0.5,1.0,0.0}
{}

void Moveable::setPos(PhysicalVector newPos) {
    this->pos = newPos;
}

PhysicalVector Moveable::position() {
	return pos;
}

PhysicalVector Moveable::vectorTo(Moveable &object2) {
    return object2.position().minus(this->position());
}

unique_ptr<MatrixStruct> Moveable::getOrientationMat() const {
	unique_ptr<MatrixStruct> matrixStruct = make_unique<MatrixStruct>();

	sgCopyMat4(matrixStruct->orientationMat, orientationMat);
	return matrixStruct;
}

void Moveable::adjustAngle(SGfloat dAngle, const PhysicalVector rotAxis) {
	sgQuat tempRotQuat;
	sgAngleAxisToQuat(tempRotQuat, dAngle,  rotAxis.vec);
	//sgRotQuat(orientationQuat, dAngle, rotAxis);

	sgPostMultQuat(orientationQuat, tempRotQuat);
	sgQuatToMatrix(orientationMat, orientationQuat);
}

void Moveable::adjustVelocity(PhysicalVector dVel) {
	this->_momentum =
			_momentum
			.scaledBy(1/_mass.value())
			.plus(dVel)
			.scaledBy(_mass.value());
}

PhysicalVector Moveable::velocity() {
	return _momentum.scaledBy(1/_mass.value());
}

// Angular Momentum and Velocity
double Moveable::momentOfInertia() { return 1;}

void Moveable::update(float dt) {
	PhysicalVector prevVelocity(prevMomentum.scaledBy(1/_mass.value()));
	auto dPos =
            this->velocity()
			.plus(prevVelocity)
			.scaledBy(.5) //Get avg velocity
			.scaledBy(dt);
	this->pos = pos.plus(dPos);

	PhysicalVector rotVec;
	rotVec = PhysicalVector(1,  0, 0);
	adjustAngle( (prevAngVelocity.x() + angVelocity.x()) * .5 *dt, rotVec);
	rotVec = PhysicalVector(0,  1, 0);
	adjustAngle( (prevAngVelocity.y() + angVelocity.y()) * .5 *dt, rotVec);
	rotVec = PhysicalVector(0,  0, 1);
	adjustAngle( (prevAngVelocity.z() + angVelocity.z()) * .5 *dt, rotVec);

	this->prevMomentum = _momentum;
	this->prevAngVelocity = angVelocity;

}
