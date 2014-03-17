/*
 * MyShape.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#include "MyShape.h"

compressed_vector< boost::shared_ptr<MyShape> > MyShape::shapes(0);

void MyShape::getVectorToObject( boost::shared_ptr<MyShape> object2, sgVec4 sepVector) {
  sgVec4 pos1, pos2;
  this->getPos(pos1);
  object2->getPos(pos2 );
  sgSubVec4(sepVector, pos2, pos1);
}

MyShape::MyShape()
{
	numPts = 16;
	pts.resize(numPts, 4);

	ptsHighlighted = true;
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

void MyShape::draw(){
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

std::string MyShape::getMomentumString() 
{
  string posString = "<" + lexical_cast<std::string>( momentum[0] )  + ", " + lexical_cast<std::string>( momentum[0] ) + ", " + lexical_cast<std::string>( momentum[0] ) + ">";
  return posString;
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

void MyShape::setDensity(float newDensity){
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

bool MyShape::isTouching( boost::shared_ptr<MyShape> otherShape )
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
    // cout << "this->radius: " << this->getRadius() << endl;
    // cout << "otherShape->getRadius(): " << otherShape->getRadius() << endl;
    // cout << "MinSep: " << minSep << endl;
    // cout << "distance: " << distance << endl;
    // cout << "No touching!" << endl;
    cout << "this->pos: " << this->getPosString() << endl;
    cout << "otherShape->pos: " << otherShape->getPosString() << endl;
  }

  return touching;
}

void MyShape::mergeWith( boost::shared_ptr<MyShape> otherShape ) 
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

void MyShape::calcMergedAngMomentum( boost::shared_ptr<MyShape> otherShape, sgVec4 totalAngMom )
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
