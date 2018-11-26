/*
 * MyShape.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#include "MyShape.h"

double MyShape::distanceTo(MyShape &object2) {
    return this->getVectorToObject(object2).length();
}

MyShape::MyShape() = default;

double MyShape::getScale(){}

void MyShape::adjustMomentum(VecStruct dMomentum) {
    this->momentum = momentum.plus(dMomentum);
}

VecStruct MyShape::getMomentum() {
    VecStruct vecStruct(momentum);
    return vecStruct;
}

// Angular Momentum and Velocity
double MyShape::getMomentOfInertia() { return 1;}

void MyShape::setAngMomentum(VecStruct newAngMomentum) {
    this->prevAngVelocity = angVelocity;
	this->prevAngVelocity = angVelocity;
	double I = getMomentOfInertia();
	this->angMomentum = newAngMomentum;
	this->angVelocity = angMomentum.scaledBy(1.0f/I);
}

VecStruct MyShape::getAngMomentum() {
    VecStruct retVec(angMomentum.vec);
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

VecStruct MyShape::getColor() const {
    VecStruct vecStruct(color.vec);
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

ShapeType MyShape::getType() { }

VecStruct MyShape::getWeightedPosition() {
//    return VecStruct();
    return this->getPos().scaledBy(this->getMass().value() );
}

