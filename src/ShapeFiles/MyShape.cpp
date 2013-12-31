/*
 * MyShape.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#include "MyShape.h"

compressed_vector< shared_ptr<MyShape> > MyShape::shapes(0);

MyShape::MyShape()
{
	numPts = 16;
	pts.resize(numPts, 4);

	ptsHighlighted = true;
}

MyShape::~MyShape() {
	//cout << "Killing MyShape" << endl;
}

matrix<double> MyShape::getPts() {
	return pts;
}

void MyShape::draw() const{
	glPushMatrix();

	//Translate
	glTranslatef(pos[0], pos[1], pos[2]);

	//Rotate
  // TODO TURN THIS BACK ON. YOU ARE NOT ROTATING ANYTHING RIGHT NOW
	glMultMatrixf( (const GLfloat*)orientationMat);

	//Scale
	drawScale();

	glColor3fv(color);

	drawUnit();
	glPopMatrix();
}

void MyShape::drawScale() const {}
void MyShape::drawUnit() const {}

float MyShape::getMarkerSize() {
	return 0;
}

void MyShape::setMomentum(float inX, float inY, float inZ) {
	momentum[0] = inX;
	momentum[1] = inY;
	momentum[2] = inZ;
	momentum[3] = 0;
	sgCopyVec4(prevMomentum, momentum);
}

void MyShape::setMomentum(const sgVec4 newMomentum) {
	sgCopyVec4(momentum, newMomentum);
	sgCopyVec4(prevMomentum, momentum);
}

void MyShape::adjustMomentum(float dx, float dy, float dz) {
	momentum[0] += dx;
	momentum[1] += dy;
	momentum[2] += dz;
}

void MyShape::adjustMomentum(const sgVec4 dMomentum) {
	sgAddVec4(momentum, dMomentum);
}

void MyShape::getMomentum(sgVec4 retVec) const{
	sgCopyVec4(retVec, momentum);
}

std::string MyShape::getMomentumString() const
{
  string posString = "<" + lexical_cast<std::string>( momentum[0] )  + ", " + lexical_cast<std::string>( momentum[0] ) + ", " + lexical_cast<std::string>( momentum[0] ) + ">";
  return posString;
}

// Angular Momentum and Velocity
float MyShape::getMomentOfInertia() const { return 1;}

void MyShape::setAngMomentum(const sgVec4 newAngMomentum) {
	float I = getMomentOfInertia();
	sgCopyVec4(angMomentum, newAngMomentum);
	sgCopyVec4(angVelocity, angMomentum);
	sgScaleVec4(angVelocity, 1.0/I);
	sgCopyVec4(prevAngVelocity, angVelocity);
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

std::string MyShape::getAngMomentumString() const
{
  string angMomString = "<" + lexical_cast<std::string>( angMomentum[0] )  + ", " + lexical_cast<std::string>( angMomentum[0] ) + ", " + lexical_cast<std::string>( angMomentum[0] ) + ">";
  return angMomString;
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

void MyShape::setDensity(float newDensity) {
	density = newDensity;
}

float MyShape::getDensity() {
	return density;
}

bool MyShape::setKineticEnergy(float newKineticEnergy)
{
  bool moving;
  sgVec4 curVelocity;
  getVelocity( curVelocity );
  if ( sgLengthVec4( curVelocity ) == 0 )
  {
    moving = false;
  }
  else
  {
    sgVec4 newVelocity;
    sgNormaliseVec4(newVelocity, curVelocity );

    float totalVelocity = sqrt( newKineticEnergy ) / mass * 2;
    sgScaleVec4( newVelocity, totalVelocity );
    setVelocity( newVelocity );
    moving = true;
  }

  return moving;
}

//! Returns kineticEnergy of object
float MyShape::getKineticEnergy()
{
  return kineticEnergy;
}

void MyShape::setColor(sgVec3 newColor) {
	sgCopyVec3(color, newColor);
}

void MyShape::getColor(sgVec3 retVec) {
	sgCopyVec3(retVec, color);
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

void MyShape::clearShapes() {
  shapes.clear();
	shapes.resize(0);
}

int MyShape::getType() { return 1;}

int MyShape::addShapeToList( shape_pointer insertShape )
{
  int curSize = shapes.size();
  shapes.resize(curSize + 1);
  shapes(curSize) = insertShape;
  return curSize;
}

void MyShape::removeShapeFromList( shape_pointer shapeToRemove )
{
  compressed_vector<shape_pointer> newShapeVector;
  int newSize =  shapes.size();
  newShapeVector.resize(newSize);
  bool removedShape = false;

  int curIndex = 0;
  foreach_( shape_pointer curShape, shapes)
  {
    if ( curShape.get() != shapeToRemove.get() )
    {
      newShapeVector.insert_element(curIndex, curShape);
      curIndex++;
    }
    else
    {
      removedShape = true;
      newShapeVector.resize(newSize-1);
    }
  }
  if ( removedShape )
  {
    //TODO PHYS-17 Do a more appropriate vector copy
    // compressed_vector<shape_pointer> localShapeList =  newShapeVector ;
    shapes = newShapeVector;
  }
}

void MyShape::setRadius(float) {}
float MyShape::getRadius() { return 1;}
