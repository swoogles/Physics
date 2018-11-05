/*
 * MyShape.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#include "MyShape.h"

float MyShape::getDistanceToObject(MyShape &object2) {
  VecStruct sepVec(this->getVectorToObject(object2));
  return sgLengthVec4( sepVec.vec );
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

void MyShape::adjustMomentum(const sgVec4 dMomentum) {
	sgAddVec4(momentum, dMomentum);
}

VecStruct MyShape::getMomentum() {
    VecStruct vecStruct(momentum);
    return vecStruct;
}

// Angular Momentum and Velocity
float MyShape::getMomentOfInertia() { return 1;}

void MyShape::setAngMomentum(sgVec4 newAngMomentum) {
	sgCopyVec4(prevAngVelocity, angVelocity);
	float I = getMomentOfInertia();
	sgCopyVec4(angMomentum, newAngMomentum);
	sgCopyVec4(angVelocity, angMomentum);
	sgScaleVec4(angVelocity, 1.0/I);
}

VecStruct MyShape::getAngMomentum() {
    VecStruct retVec(angMomentum);
    return retVec;
}

void MyShape::setMass(float newMass) {
	mass = newMass;
}

float MyShape::getMass() {
	return mass;
}

float MyShape::getDensity() {
	return density;
}

VecStruct MyShape::getColor() const {
    VecStruct vecStruct;
    sgCopyVec4(vecStruct.vec, color);
    return vecStruct;
}

void MyShape::calcColor() {
  float totalMass = mass;

	float redAmount = 0.25f + mass / (totalMass/3.0f);
	if (redAmount > 1.0)
		redAmount = 1.0;

	float greenAmount = mass / (0.8f *totalMass);
	if (greenAmount > 1.0) {
		greenAmount = 1.0;
	}


	color[0] = redAmount;
	color[1] = greenAmount;
}

ShapeType MyShape::getType() { }

bool MyShape::isTouching(MyShape &otherShape)
{
  VecStruct sepVec(this->getVectorToObject(otherShape));

  SGfloat distanceSquared = sgLengthSquaredVec4(sepVec.vec);
  SGfloat distance = sqrt(distanceSquared);

  SGfloat minSep = this->getRadius() + otherShape.getRadius();

  return (distance < minSep);
}

// TODO Don't implement this at MyShape. It should have separate implementations for Circles and Boxes (If boxes even need it)
void MyShape::mergeWith(MyShape &otherShape)
{
  float combinedMass = this->getMass() + otherShape.getMass();
  float density = this->getDensity();

  float newRadius = calcRadius(combinedMass, density);

  VecStruct totalAngMom = calcMergedAngMomentum(otherShape);

  // COM start
  VecStruct tempVec(this->getPos());
  VecStruct tempVec2(otherShape.getPos());

  sgScaleVec4(tempVec.vec, this->getMass());
  sgScaleVec4(tempVec2.vec, otherShape.getMass());

  VecStruct COM = tempVec.plus(tempVec2).scaledBy(1/(combinedMass));
  // COM end

  this->setMass(combinedMass);
  this->setRadius(newRadius);

  // TODO Verify this stuff
  otherShape.setMass(0);
  otherShape.setRadius(0);
  // TODO /Verify this stuff

  this->adjustMomentum(otherShape.getMomentum().vec);

  this->setAngMomentum(totalAngMom.vec);

  this->calcColor();

  this->setPos(COM.vec);
}

// TODO This should return totalAngMom, instead of mutating parameter.
VecStruct MyShape::calcMergedAngMomentum(MyShape &otherShape)
{
    VecStruct aPos(this->getPos());
    VecStruct bPos(otherShape.getPos());
    VecStruct aMomentum(this->getMomentum());
    VecStruct bMomentum(otherShape.getMomentum());

    sgVec3 crossed;

    vecPtr sepVec(VecStruct::vecFromAtoB(aPos, bPos));
    VecStruct sepVecUnit;
    sgNormaliseVec4( sepVecUnit.vec, sepVec->vec );

    VecStruct hitPointOnA = sepVecUnit.scaledBy(this->getRadius());

    VecStruct hitPt = aPos.plus(hitPointOnA);

    VecStruct rForA = aPos.minus(hitPt);

    sgVectorProductVec3(crossed, rForA.vec, aMomentum.vec);
    VecStruct newAngularMomentumForA(crossed);

    VecStruct rForB = bPos.minus(hitPt);

    sgVectorProductVec3(crossed, rForB.vec, bMomentum.vec);
    VecStruct newAngularMomentumForB(crossed);
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

