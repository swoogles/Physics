/*
 * MyShape.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#include "MyShape.h"

float MyShape::distanceTo(MyShape &object2) {
    return this->getVectorToObject(object2).length();
}

MyShape::MyShape() = default;

// MyShape::MyShape( const MyShape& copyShape )
// {
//   sgVec4 newPos;
//   copyShape.getPos( newPos );
//   this->setPos( newPos );
// 
//   sgVec4 newMom;
//   copyShape.getMomentum( newMom );
//   this->setMomentum( newMom );
// 
//   this->setMass( copyShape.getMass() );
//   this->setDensity( copyShape.getDensity() );
//   // this->setRadius( copyShape.getRadius() );
// 
//   sgVec4 newColor;;
//   copyShape.getColor( newColor );
//   this->setColor( newColor );
// }

float MyShape::getScale(){}

void MyShape::adjustMomentum(VecStruct dMomentum) {
    this->momentum = momentum.plus(dMomentum);
}

VecStruct MyShape::getMomentum() {
    VecStruct vecStruct(momentum);
    return vecStruct;
}

// Angular Momentum and Velocity
float MyShape::getMomentOfInertia() { return 1;}

void MyShape::setAngMomentum(VecStruct newAngMomentum) {
    this->prevAngVelocity = angVelocity;
	this->prevAngVelocity = angVelocity;
	float I = getMomentOfInertia();
	this->angMomentum = newAngMomentum;
	this->angVelocity = angMomentum.scaledBy(1.0f/I);
}

VecStruct MyShape::getAngMomentum() {
    VecStruct retVec(angMomentum.vec);
    return retVec;
}

void MyShape::setMass(float newMass) {
	mass = kilogram_t(newMass);
}

float MyShape::getMass() {
	return mass.value();
}

float MyShape::getDensity() {
	return density;
}

VecStruct MyShape::getColor() const {
    VecStruct vecStruct(color.vec);
    return vecStruct;
}

void MyShape::calcColor() {
  float totalMass = mass.value();

	float redAmount = 0.25f + mass.value() / (totalMass/3.0f);
	if (redAmount > 1.0)
		redAmount = 1.0;

	float greenAmount = mass.value() / (0.8f *totalMass);
	if (greenAmount > 1.0) {
		greenAmount = 1.0;
	}

	color.vec[0] = redAmount;
	color.vec[1] = greenAmount;
}

ShapeType MyShape::getType() { }

bool MyShape::isTouching(MyShape &otherShape)
{
  VecStruct sepVec(this->getVectorToObject(otherShape));
  float minSep = this->getRadius() + otherShape.getRadius();

  return (sepVec.length() < minSep);
}

// TODO Don't implement this at MyShape. It should have separate implementations for Circles and Boxes (If boxes even need it)
void MyShape::mergeWith(MyShape &otherShape)
{
  float combinedMass = this->getMass() + otherShape.getMass();
  float density = this->getDensity();

  float newRadius = calcRadius(combinedMass, density);

  VecStruct totalAngMom = calcMergedAngMomentum(otherShape);

  VecStruct COM =
          this->getWeightedPosition()
          .plus(otherShape.getWeightedPosition())
          .scaledBy(1/(combinedMass));

  this->setMass(combinedMass);
  this->setRadius(newRadius);

  // TODO Verify this stuff
  otherShape.setMass(0);
  otherShape.setRadius(0);
  // TODO /Verify this stuff

  this->adjustMomentum(otherShape.getMomentum());

  this->setAngMomentum(totalAngMom);

  this->calcColor();

  this->setPos(COM);
}

// TODO This should return totalAngMom, instead of mutating parameter.
VecStruct MyShape::calcMergedAngMomentum(MyShape &otherShape)
{
    VecStruct aPos(this->getPos());
    VecStruct bPos(otherShape.getPos());
    VecStruct aMomentum(this->getMomentum());
    VecStruct bMomentum(otherShape.getMomentum());

    vecPtr sepVec(VecStruct::vecFromAtoB(aPos, bPos));

    VecStruct hitPointOnA = sepVec->unit().scaledBy(this->getRadius());

    VecStruct hitPt = aPos.plus(hitPointOnA);

    VecStruct rForA = aPos.minus(hitPt);

    VecStruct newAngularMomentumForA = rForA.vectorProduct3(aMomentum);

    VecStruct rForB = bPos.minus(hitPt);

    VecStruct newAngularMomentumForB = rForB.vectorProduct3(bMomentum);
    VecStruct totalAngMom = newAngularMomentumForA.plus(newAngularMomentumForB);

    return totalAngMom
            .plus(this->getAngMomentum())
            .plus(otherShape.getAngMomentum());
}

// This was t-totally fucked before
float MyShape::calcRadius(float massBoth, float density) {
    return sqrt(((3*massBoth) / 4 * M_PI ) / density);
}

void MyShape::setRadius(float) {}
float MyShape::getRadius() { return 1;}

VecStruct MyShape::getWeightedPosition() {
//    return VecStruct();
    return this->getPos().scaledBy(this->getMass() );
}

