/*
 * MyShape.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#include "MyShape.h"

float MyShape::getDistanceToObject(MyShape &object2) {
  vecPtr sepVec(this->getVectorToObject(object2));
  float distance = sgLengthVec4( sepVec->vec );

  return distance;
}

MyShape::MyShape()
{
}

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

MyShape::~MyShape() {
	//cout << "Killing MyShape" << endl;
}

float MyShape::getScale(){}

void MyShape::adjustMomentum(const sgVec4 dMomentum) {
	sgAddVec4(momentum, dMomentum);
}

void MyShape::getMomentum(sgVec4 retVec) {
	sgCopyVec4(retVec, momentum);
}

unique_ptr<VecStruct> MyShape::getMomentum() {
    unique_ptr<VecStruct> vecStruct = make_unique<VecStruct>();
    sgCopyVec4(vecStruct->vec, momentum);
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

void MyShape::adjustAngMomentum(const sgVec4 dAngMomentum) {
	//TODO generalize for other shapes
	float I = getMomentOfInertia();

	sgAddVec4(angMomentum, dAngMomentum);
	sgCopyVec4(angVelocity, angMomentum);
	sgScaleVec4(angVelocity, 1.0/I);
}

void MyShape::getAngMomentum(sgVec4 retVec) {
	sgCopyVec4(retVec, angMomentum);
}

void MyShape::setMass(float newMass) {
	mass = newMass;
}

void MyShape::adjustMass(float dMass) {
	mass += dMass;
}

float MyShape::getMass() {
	return mass;
}

float MyShape::getDensity() {
	return density;
}

void MyShape::getColor(sgVec3 retVec) const{
	sgCopyVec3(retVec, color);
}

unique_ptr<VecStruct> MyShape::getColor() const {
    unique_ptr<VecStruct> vecStruct = make_unique<VecStruct>();
    sgCopyVec4(vecStruct->vec, color);
    return vecStruct;

}

void MyShape::calcColor() {
  float totalMass = mass;

	float redAmount = 0.25 + mass / (totalMass/3.0);
	if (redAmount > 1.0)
		redAmount = 1.0;

	float greenAmount = mass / (0.8 *totalMass);
	if (greenAmount > 1.0) {
		greenAmount = 1.0;
	}


	color[0] = redAmount;
	color[1] = greenAmount;
}

ShapeType MyShape::getType() { }

bool MyShape::isTouching(MyShape &otherShape)
{
  vecPtr sepVec(this->getVectorToObject(otherShape));

  SGfloat distanceSquared = sgLengthSquaredVec4(sepVec->vec);
  SGfloat distance = sqrt(distanceSquared);

  SGfloat minSep = this->getRadius() + otherShape.getRadius();


  return (distance < minSep);
}

// TODO Don't implement this at MyShape. It should have separate implementations for Circles and Boxes (If boxes even need it)
void MyShape::mergeWith(MyShape &otherShape)
{
    cout << "MERGING" << endl;
  float newMass = this->getMass() + otherShape.getMass();
  float density = this->getDensity();

  float newRadius = calcMergedRadius( newMass, density );

  sgVec3 totalAngMom;

  calcMergedAngMomentum( otherShape, totalAngMom );

  // COM start
  vecPtr tempVec(this->getPosNew());
  vecPtr tempVec2(otherShape.getPosNew());
  sgVec4 COM;

  sgScaleVec4(tempVec->vec, this->getMass());
  sgScaleVec4(tempVec2->vec, otherShape.getMass());

  sgAddVec4(COM,tempVec->vec, tempVec2->vec);
  sgScaleVec4(COM, 1/(this->getMass() + otherShape.getMass()) );
  // COM end

  vecPtr otherShapeMomentum(otherShape.getMomentum());

  this->setMass(newMass);
  this->setRadius(newRadius);
  this->adjustMomentum( otherShapeMomentum->vec );

  this->setAngMomentum(totalAngMom);
  this->getAngMomentum(totalAngMom);

  this->calcColor();

  this->setPos(COM);
}

// TODO This should return totalAngMom, instead of mutating parameter.
void MyShape::calcMergedAngMomentum(MyShape &otherShape, sgVec4 totalAngMom)
{
  sgVec4 sepVecUnit;

  vecPtr aPos(this->getPosNew());
  vecPtr bPos(otherShape.getPosNew());
  vecPtr aMomentum(this->getMomentum());
  vecPtr bMomentum(otherShape.getMomentum());
  sgVec4 tempVec;
  sgVec4 hitPt;

  sgVec3 r;
  sgVec3 crossed;

  vecPtr sepVec(VecStruct::vecFromAtoB(aPos.get(), bPos.get()));
  sgNormaliseVec4( sepVecUnit, sepVec->vec );

  sgScaleVec4(tempVec, sepVecUnit, this->getRadius());
  sgAddVec4(hitPt, aPos->vec, tempVec);
  
  sgSubVec3( r, aPos->vec, hitPt );

  for (int i = 0; i < 4; i++) {
    totalAngMom[i] = 0;
  }

  sgVectorProductVec3(crossed, r, aMomentum->vec);

  sgAddVec4(totalAngMom, crossed);

  sgSubVec3( r, bPos->vec, hitPt );

  sgVectorProductVec3(crossed, r, bMomentum->vec);

  sgAddVec3(totalAngMom, crossed);

  this->getAngMomentum(tempVec);
  sgAddVec4(totalAngMom, tempVec);

  otherShape.getAngMomentum(tempVec);
  sgAddVec4(totalAngMom, tempVec);
}

float MyShape::calcMergedRadius(float massBoth, float density) {
  float radius = massBoth/density;
  radius /= (4/3.0);
  radius = pow(radius, 1.0/3);
  return radius;
}

void MyShape::setRadius(float) {}
float MyShape::getRadius() { return 1;}

