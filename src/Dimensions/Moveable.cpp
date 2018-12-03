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
          ,shapeType(ShapeType::box)
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

PhysicalVector Moveable::weightedPosition() {
	return this->position().scaledBy(this->_mass.value() );
}

double Moveable::distanceTo(Moveable &object2) {
    return this->vectorTo(object2).length();
}Moveable::Moveable(ShapeType shapeType, PhysicalVector momentum)
		: _momentum(momentum)
		, shapeType(shapeType) {
}

double Moveable::scale(){}

void Moveable::adjustMomentum(PhysicalVector dMomentum) {
    this->_momentum = _momentum.plus(dMomentum);
}

PhysicalVector Moveable::momentum() {
    PhysicalVector vecStruct(_momentum);
    return vecStruct;
}

void Moveable::setAngularMomentum(PhysicalVector newAngMomentum) {
    this->prevAngVelocity = angVelocity;
	this->prevAngVelocity = angVelocity;
	double I = momentOfInertia();
	this->angMomentum = newAngMomentum;
	this->angVelocity = angMomentum.scaledBy(1.0f/I);
}

PhysicalVector Moveable::angularMomentum() {
    PhysicalVector retVec(angMomentum.vec);
    return retVec;
}

void Moveable::setMass(kilogram_t newMass) {
	_mass = newMass;
}

kilogram_t Moveable::mass() {
	return _mass;
}

kilograms_per_cubic_meter_t Moveable::density() {
	return _density;
}

PhysicalVector Moveable::getColor() const {
    PhysicalVector vecStruct(_color.vec);
    return vecStruct;
}

void Moveable::calcColor() {
  double totalMass = _mass.value();

	double redAmount = 0.25f + _mass.value() / (totalMass/3.0f);
	if (redAmount > 1.0)
		redAmount = 1.0;

	double greenAmount = _mass.value() / (0.8f *totalMass);
	if (greenAmount > 1.0) {
		greenAmount = 1.0;
	}

	_color.vec[0] = redAmount;
	_color.vec[1] = greenAmount;
}

ShapeType Moveable::getType() {
	return this->shapeType;
}