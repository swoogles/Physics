/*
 * MyShape.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#include "MyShape.h"

typedef boost::shared_ptr<MyShape> shapePointer_t;
compressed_vector< shapePointer_t > MyShape::shapes(0);

void MyShape::getVectorToObject( shapePointer_t object2, sgVec4 sepVector) {
  sgVec4 pos1, pos2;
  this->getPos(pos1);
  object2->getCenterOfMass( pos2 );
  sgSubVec4(sepVector, pos2, pos1);
}

float MyShape::getDistanceToObject( shapePointer_t object2 ) {
  sgVec4 sepVec;
  float distance;

  this->getVectorToObject( object2, sepVec);
  distance = sgLengthVec4( sepVec );

  return distance;
}

MyShape::MyShape()
{
	numPts = 16;
	pts.resize(numPts, 4);
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

matrix<double> MyShape::getPts() {
	return pts;
}

// TODO Figure out how to get the OpenGL crud out of this class. That should be handled elsewhere.
void MyShape::draw(){
	glPushMatrix();

	//Translate
	glTranslatef(pos[0], pos[1], pos[2]);

	//Rotate
	glMultMatrixf( (const GLfloat*)orientationMat);

	//Scale
	drawScale();

	glColor3fv(color);

	drawUnit();
	glPopMatrix();
}

void MyShape::drawAllShapes() {
    foreach_ ( shapePointer_t curShape, MyShape::shapes )
    {
        curShape->draw();
    }

}

void MyShape::drawScale(){}
void MyShape::drawUnit(){}

float MyShape::getMarkerSize() {
	return 0;
}

void MyShape::setMomentum(float inX, float inY, float inZ) {
	sgCopyVec4(prevMomentum, momentum);
	momentum[0] = inX;
	momentum[1] = inY;
	momentum[2] = inZ;
	momentum[3] = 0;
}

void MyShape::setMomentum(const sgVec4 newMomentum) {
	sgCopyVec4(prevMomentum, momentum);
	sgCopyVec4(momentum, newMomentum);
}

void MyShape::adjustMomentum(float dx, float dy, float dz) {
	momentum[0] += dx;
	momentum[1] += dy;
	momentum[2] += dz;
}

void MyShape::adjustMomentum(const sgVec4 dMomentum) {
	sgAddVec4(momentum, dMomentum);
}

void MyShape::getMomentum(sgVec4 retVec) {
	sgCopyVec4(retVec, momentum);
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

void MyShape::setDensity(float newDensity){
	density = newDensity;
}

float MyShape::getDensity() {
	return density;
}

void MyShape::getColor(sgVec3 retVec) const{
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

int MyShape::addShapeToList( shapePointer_t insertShape )
{
  int curSize = shapes.size();
  shapes.resize(curSize + 1);
  shapes(curSize) = insertShape;
  return curSize;
}

void MyShape::removeShapeFromList( shapePointer_t shapeToRemove )
{
  compressed_vector<shapePointer_t> newShapeVector;
  int newSize =  shapes.size();
  newShapeVector.resize(newSize);
  bool removedShape = false;

  int curIndex = 0;
  foreach_( shapePointer_t curShape, shapes)
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
    // compressed_vector<shapePointer_t> localShapeList =  newShapeVector ;
    shapes = newShapeVector;
  }
}

bool MyShape::isTouching( shapePointer_t otherShape )
{
  bool touching = false;
  sgVec4 sepVec;
  SGfloat distanceSquared, distance, minSep;

  this->getVectorToObject( otherShape, sepVec);

  distanceSquared = sgLengthSquaredVec4(sepVec);
  distance = sqrt(distanceSquared);

  minSep = this->getRadius() + otherShape->getRadius();


  if (distance < minSep) 
  {
    touching = true;
  }

  return touching;
}

// TODO Don't implement this at MyShape. It should have separate implementations for Circles and Boxes (If boxes even need it)
void MyShape::mergeWith( shapePointer_t otherShape ) 
{
  float newMass = this->getMass() + otherShape->getMass();
  float density = this->getDensity();

  float newRadius = calcMergedRadius( newMass, density );

  sgVec3 totalAngMom;

  calcMergedAngMomentum( otherShape, totalAngMom );

  // COM start
  sgVec4 aPos, bPos;
  sgVec4 tempVec, tempVec2, COM;

  this->getPos(aPos);
  otherShape->getPos(bPos);

  sgCopyVec4(tempVec, aPos);
  sgCopyVec4(tempVec2, bPos);

  sgScaleVec4(tempVec, this->getMass());
  sgScaleVec4(tempVec2, otherShape->getMass());

  sgAddVec4(COM,tempVec, tempVec2);
  sgScaleVec4(COM, 1/(this->getMass() + otherShape->getMass()) );
  // COM end

  sgVec4 otherShapeMomentum;
  otherShape->getMomentum(otherShapeMomentum);

  this->setMass(newMass);
  this->setRadius(newRadius);
  this->adjustMomentum( otherShapeMomentum );

  this->setAngMomentum(totalAngMom);
  sgVec3 angVel;
  this->getAngMomentum(totalAngMom);
  this->getAngVelocity(angVel);

  this->calcColor();

  this->setPos(COM);
}

void MyShape::calcMergedAngMomentum( shapePointer_t otherShape, sgVec4 totalAngMom )
{
  sgVec4 sepVec, sepVecUnit;

  sgVec4 aPos, bPos;
  sgVec4 aMomentum, bMomentum;
  sgVec4 tempVec;
  sgVec4 hitPt;

  sgVec3 r, aMom3, bMom3;
  sgVec3 crossed = { 0, 0, 0 };

  this->getPos(aPos);
  otherShape->getPos(bPos);
  sgSubVec4( sepVec, aPos, bPos );
  sgNormaliseVec4( sepVecUnit, sepVec );

  sgScaleVec4(tempVec, sepVecUnit, this->getRadius());
  sgAddVec4(hitPt, aPos, tempVec);
  
  sgSubVec3( r, aPos, hitPt );

  for (int i = 0; i < 4; i++) {
    totalAngMom[i] = 0;
  }

  this->getMomentum(aMomentum);
  otherShape->getMomentum(bMomentum);

  aMom3[0] = aMomentum[0];
  aMom3[1] = aMomentum[1];
  aMom3[2] = aMomentum[2];
  sgVectorProductVec3(crossed, r, aMom3);

  sgAddVec4(totalAngMom, crossed);

  sgSubVec3( r, bPos, hitPt );

  bMom3[0] = bMomentum[0];
  bMom3[1] = bMomentum[1];
  bMom3[2] = bMomentum[2];
  sgVectorProductVec3(crossed, r, bMom3);

  sgAddVec3(totalAngMom, crossed);

  this->getAngMomentum(tempVec);
  sgAddVec4(totalAngMom, tempVec);

  otherShape->getAngMomentum(tempVec);
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

void MyShape::getCenterOfMass(sgVec4 retVec) {
	sgCopyVec4(retVec, pos);
}
