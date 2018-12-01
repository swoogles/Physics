#include "MyShape.h"

double MyShape::distanceTo(MyShape &object2) {
    return this->vectorTo(object2).length();
}

MyShape::MyShape(ShapeType shapeType, PhysicalVector momentum)
		:Moveable(momentum),
		shapeType(shapeType) {
};

double MyShape::scale(){}

void MyShape::adjustMomentum(PhysicalVector dMomentum) {
    this->_momentum = _momentum.plus(dMomentum);
}

PhysicalVector MyShape::momentum() {
    PhysicalVector vecStruct(_momentum);
    return vecStruct;
}

double MyShape::momentOfInertia() { return 1;}

void MyShape::setAngularMomentum(PhysicalVector newAngMomentum) {
    this->prevAngVelocity = angVelocity;
	this->prevAngVelocity = angVelocity;
	double I = momentOfInertia();
	this->angMomentum = newAngMomentum;
	this->angVelocity = angMomentum.scaledBy(1.0f/I);
}

PhysicalVector MyShape::angularMomentum() {
    PhysicalVector retVec(angMomentum.vec);
    return retVec;
}

void MyShape::setMass(kilogram_t newMass) {
	_mass = newMass;
}

kilogram_t MyShape::mass() {
	return _mass;
}

kilograms_per_cubic_meter_t MyShape::density() {
	return _density;
}

PhysicalVector MyShape::getColor() const {
    PhysicalVector vecStruct(_color.vec);
    return vecStruct;
}

void MyShape::calcColor() {
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

ShapeType MyShape::getType() {
	return this->shapeType;
}

PhysicalVector MyShape::weightedPosition() {
//    return PhysicalVector();
    return this->position().scaledBy(this->mass().value() );
}

