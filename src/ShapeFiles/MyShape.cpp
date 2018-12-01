#include "MyShape.h"

double MyShape::distanceTo(MyShape &object2) {
    return this->getVectorToObject(object2).length();
}

MyShape::MyShape(ShapeType shapeType, PhysicalVector momentum)
		:Moveable(momentum),
		shapeType(shapeType) {
};

double MyShape::getScale(){}

void MyShape::adjustMomentum(PhysicalVector dMomentum) {
    this->momentum = momentum.plus(dMomentum);
}

PhysicalVector MyShape::getMomentum() {
    PhysicalVector vecStruct(momentum);
    return vecStruct;
}

double MyShape::getMomentOfInertia() { return 1;}

void MyShape::setAngMomentum(PhysicalVector newAngMomentum) {
    this->prevAngVelocity = angVelocity;
	this->prevAngVelocity = angVelocity;
	double I = getMomentOfInertia();
	this->angMomentum = newAngMomentum;
	this->angVelocity = angMomentum.scaledBy(1.0f/I);
}

PhysicalVector MyShape::getAngMomentum() {
    PhysicalVector retVec(angMomentum.vec);
    return retVec;
}

void MyShape::setMass(kilogram_t newMass) {
	mass = newMass;
}

kilogram_t MyShape::getMass() {
	return mass;
}

kilograms_per_cubic_meter_t MyShape::getDensity() {
	return density;
}

PhysicalVector MyShape::getColor() const {
    PhysicalVector vecStruct(color.vec);
    return vecStruct;
}

void MyShape::calcColor() {
  double totalMass = mass.value();

	double redAmount = 0.25f + mass.value() / (totalMass/3.0f);
	if (redAmount > 1.0)
		redAmount = 1.0;

	double greenAmount = mass.value() / (0.8f *totalMass);
	if (greenAmount > 1.0) {
		greenAmount = 1.0;
	}

	color.vec[0] = redAmount;
	color.vec[1] = greenAmount;
}

ShapeType MyShape::getType() {
	return this->shapeType;
}

PhysicalVector MyShape::getWeightedPosition() {
//    return PhysicalVector();
    return this->getPos().scaledBy(this->getMass().value() );
}

