/*
 * Interactions.cpp
 *
 *  Created on: Jul 21, 2011
 *      Author: brasure
 */

#include "Interactions.h"

using namespace std;

void elasticCollision( boost::shared_ptr<MyShape> object1, boost::shared_ptr<MyShape> object2, float dt) {
	sgVec4 sepVec;
	sgVec4 sepVecUnit;

	sgVec4 aVel, bVel;
	sgVec4 tempVec, n, vdiff;

	SGfloat multiplier;

	float c;

	getVectorToObject2(object1, object2, sepVec);
	sgNormaliseVec4(sepVecUnit, sepVec);

	object1->getVelocity(aVel);
	object2->getVelocity(bVel);

	sgCopyVec4(n, sepVecUnit);
	sgSubVec4(vdiff, aVel, bVel);

	c = sgScalarProductVec4(n, vdiff);

	multiplier = -2 * ( object2->getMass() * c ) / (object2->getMass() + object1->getMass());
	sgScaleVec4(tempVec, n, multiplier);

	object1->adjustVelocity(tempVec);

	multiplier = 2 * ( object1->getMass() * c ) / (object2->getMass() + object1->getMass());
	sgScaleVec4(tempVec, n, multiplier);

	object2->adjustVelocity(tempVec);

  // TODO Figure out how necessary these lines are.
	while (contact(object1, object2)) {
		object1->update(dt/30);
		object2->update(dt/30);
	}


}

bool contact(boost::shared_ptr<MyShape> object1, boost::shared_ptr<MyShape> object2) {
	sgVec4 sepVec;
	float minSep;
	SGfloat distance, distanceSquared;

	getVectorToObject2(object1, object2, sepVec);

	distanceSquared = sgLengthSquaredVec4(sepVec);
	distance = sqrt(distanceSquared);

	minSep = object1->getRadius() + object2->getRadius();

	return (distance < minSep);
}

void calcForcesAll_Naive( boost::shared_ptr<Simulation> curSimulation ) 
{
  compressed_vector<shape_pointer> physicalObjects = curSimulation->getPhysicalObjects().getShapes();
  float dt = curSimulation->getDT();
  sgVec4 sepVec;
  sgVec4 unitVec;
  sgVec4 gravVec;
  boost::shared_ptr<MyShape> object1, object2;
  float fGrav;
  SGfloat distanceSquared;

  sgVec4 gravField;

  int curObjectIdx = 0;
  int actingObjectIdx = 0;

  if (curSimulation->isConstGravField() ) {
    curSimulation->getConstGravFieldVal(gravField);
    sgScaleVec4(gravField, 1/dt);
  }
  
  if ( physicalObjects.size() > 0 )
  {
    for (unsigned int i = 0; i < physicalObjects.size()-1; i++)
    {
      object1 = physicalObjects(i);
      sgVec4 pos;
      object1->getPos(pos);

      if (curSimulation->isConstGravField() ) {
        object1->adjustMomentum(gravField);
      }

      for (unsigned int j = i + 1; j < physicalObjects.size(); )
      {
        object2 = physicalObjects(j);
        object2->getPos(pos);

        getVectorToObject2(object1, object2, sepVec);

        distanceSquared = sgLengthSquaredVec4(sepVec);

        if (curSimulation->isGravBetweenObjects() ) {
          fGrav = calcForceGrav(object1, object2, distanceSquared);

          sgNormaliseVec4(unitVec, sepVec);

          sgScaleVec4(gravVec, unitVec, fGrav);
          sgScaleVec4(gravVec, dt);

          object1->adjustMomentum(gravVec);
          sgNegateVec4(gravVec);
          object2->adjustMomentum(gravVec);
        }

        j++;
        actingObjectIdx++;
      }
      curObjectIdx++;
    }

    // Add unary forces to last object
    object1 = physicalObjects(physicalObjects.size()-1);

    if (curSimulation->isConstGravField() ) {
      object1->adjustMomentum(gravField);
    }
  }

}

void calcForceOnObject_Octree(shape_pointer curObject, boost::shared_ptr<Quadrant> curQuadrant, float dt) 
{
  sgVec4 sepVec, unitVec, gravVec;
  boost::shared_ptr<MyShape> object1, object2, shapeInQuadrant;
  float fGrav;
  SGfloat distance, distanceSquared;
  SGfloat theta = 0.5;
  typedef boost::shared_ptr<Quadrant> quad_pointer;

  //1. 
    //a. If the current node is an external node 
    //b. (and it is not body b), 
    //c. calculate the force exerted by the current node on b
    //d. add this amount to b’s net force.
  //2. 
    //a. Otherwise, calculate the ratio s/d. If s/d < θ, treat this internal node as a single body, and calculate the force it exerts on body b
    //b. add this amount to b’s net force.
  //3. Otherwise, run the procedure recursively on each of the current node’s children.


  //1.
  //a. 
  if ( curQuadrant->isExternal() ) 
  {
    shapeInQuadrant = curQuadrant->getShapeInQuadrant();

    //b.
    if ( shapeInQuadrant != nullptr && curObject != shapeInQuadrant )
    {
      getVectorToObject2( curObject, curQuadrant->getShapeInQuadrant(), sepVec);
      distance = sgLengthVec4( sepVec );
      distanceSquared = sgLengthSquaredVec4( sepVec );
      //c.
      fGrav = calcForceGrav(curObject, shapeInQuadrant, distanceSquared);

      sgNormaliseVec4(unitVec, sepVec);

      sgScaleVec4(gravVec, unitVec, fGrav);
      sgScaleVec4(gravVec, dt);

      curObject->adjustMomentum(gravVec);
    }
  }
  else
  {
    sgVec4 com;
    curQuadrant->getCenterOfMass( com );
    getVectorToQuadrant( curObject, curQuadrant, sepVec);
    distance = sgLengthVec4( sepVec );
    distanceSquared = sgLengthSquaredVec4( sepVec );
    //2.
    //a.
    if ( curQuadrant->getWidth() / distance < theta )
    {
      fGrav = calcForceGrav(curObject, curQuadrant, distanceSquared);

      sgNormaliseVec4(unitVec, sepVec);

      sgScaleVec4(gravVec, unitVec, fGrav);
      sgScaleVec4(gravVec, dt);

      //b.
      curObject->adjustMomentum(gravVec);
    }
    //3.
    else
    {
      quad_pointer targetQuadrant;
      for ( int x = 0; x < 2; x++ )
      {
        for ( int y = 0; y < 2; y++ )
        {
          for ( int z = 0; z < 2; z++ )
          {

            targetQuadrant = curQuadrant->getQuadrantFromCell( x, y, z );
            if ( targetQuadrant != nullptr )
            {
              calcForceOnObject_Octree(curObject, targetQuadrant, dt);
            }
          }
        }
      }

    }
  }

}

void calcForcesAll( boost::shared_ptr<Simulation> curSimulation )
{
  ShapeList physicalObjects = curSimulation->getPhysicalObjects();
  if ( curSimulation->getCurStep() % 100 == 0 )
  {
    cout << "Shapelist.size: " << physicalObjects.getShapes().size() << endl;
  }

  if ( curSimulation->getForceCalcMethod() != Simulation::FORCE_CALC_METHOD_NAIVE  )
  {
    calcForcesAll_Naive( curSimulation );

  }
  else //Calculations with Octree
  {
    if ( physicalObjects.getShapes().size() > 0 )
    {
      ShapeList shapeList;

      foreach_ ( shape_pointer curShape, physicalObjects.getShapes() )
      {
        calcForceOnObject_Octree(curShape, curSimulation->getQuadrant(), curSimulation->getDT() );
      }
    }
  }
}

bool isConflict(int newShape) 
{
  sgVec4 sepVec;
  SGfloat distanceSquared, distance, minSep;
  bool conflict = false;

  boost::shared_ptr<MyShape> object1, object2;

  object2 = MyShape::shapes(newShape);

  for (int i = 0; i < newShape && conflict == false; i++) 
  {
    object1 = MyShape::shapes(i);
    if ( object1->getType() == 2 )
    {
      getVectorToObject2(object1, object2, sepVec);

      distanceSquared = sgLengthSquaredVec4(sepVec);
      distance = sqrt(distanceSquared);

      minSep = object1->getRadius() + object2->getRadius();


      if (distance < minSep) {
        conflict = true;
      }
    }
  }
  return conflict;
}

bool isConflict_ArbitraryList( compressed_vector<shape_pointer> physicalObjects, shape_pointer newShape ) 
{
  sgVec4 sepVec;
  SGfloat distanceSquared, distance, minSep;
  bool conflict = false;

  boost::shared_ptr<MyShape> object1;

  for (int i = 0; i < physicalObjects.size() && conflict == false; i++) 
  {
    object1 = physicalObjects(i);
    conflict = object1->isTouching( newShape  );
  }
  return conflict;
}

void calcCollisionsAll(boost::shared_ptr<Simulation> curSimulation) 
{
  ShapeList shapeList = curSimulation->getPhysicalObjects() ;
  sgVec4 sepVec;
  boost::shared_ptr<MyShape> object1, object2;
  SGfloat distanceSquared, distance, minSep;

  compressed_vector<shape_pointer> physicalObjects = shapeList.getShapes();
  compressed_vector<shape_pointer> deleteList;

  sgVec4 gravField;

  for (unsigned int i = 0; i < physicalObjects.size()-1; i++)
  {
    object1 = physicalObjects(i);

    if (curSimulation->isConstGravField() ) 
    {
      object1->adjustMomentum(gravField);
    }

    for (unsigned int j = i + 1; j < physicalObjects.size(); j++)
    {
      object2 = physicalObjects(j);

      getVectorToObject2(object1, object2, sepVec);

      distanceSquared = sgLengthSquaredVec4(sepVec);
      distance = sqrt(distanceSquared);
      minSep = object1->getRadius() + object2->getRadius();

      if (distance < minSep)
      {
        if (curSimulation->isAllElastic() )
        {
          elasticCollision( object1, object2, curSimulation->getDT() );
        }
        else if (curSimulation->isAllInelastic() ){
          mergeObjects(object1, object2);
          deleteList.resize(deleteList.size()+1);
          deleteList.insert_element(deleteList.size()-1, object2);
        }
      }
    }
  }
  if ( deleteList.size() > 0 )
  {
    foreach_ ( shape_pointer curShape, deleteList )
    {
      shapeList.removeShapeFromList( curShape );
      MyShape::removeShapeFromList( curShape );
    }
  }

  curSimulation->setPhysicalObjects( shapeList ) ;
}

//setting rSquared to a constant value with a grid can make some awesome effects
float calcForceGrav(boost::shared_ptr<MyShape> object1, boost::shared_ptr<MyShape> object2, SGfloat rSquared) {
  if (rSquared < .00001)
  {
    rSquared = .00001;
  }
  return ( Simulations::G * object1->getMass() * object2->getMass()) / rSquared;
}

void getVectorToObject2(boost::shared_ptr<MyShape> object1, boost::shared_ptr<MyShape> object2, sgVec4 sepVector) {
  sgVec4 pos1, pos2;
  object1->getPos(pos1 );
  object2->getPos(pos2 );
  sgSubVec4(sepVector, pos2, pos1);
}

void getVectorToQuadrant(boost::shared_ptr<MyShape> object1, boost::shared_ptr<Quadrant> quadrant, sgVec4 sepVector) {
  sgVec4 pos1, pos2;
  object1->getPos(pos1 );
  quadrant->getCenterOfMass(pos2 );
  if ( ! sgCompareVec4( pos1, pos2, .0001 ) )
  {
    sgSubVec4(sepVector, pos2, pos1);
  }
  else
  {
    sepVector[0]=0;
    sepVector[1]=0;
    sepVector[2]=0;
  }
}

float calcMergedRadius(float massBoth, float density) {
  float radius = massBoth/density;
  radius /= (4/3.0);
  radius = pow(radius, 1.0/3);
  return radius;
}

void calcMergedAngMomentum(boost::shared_ptr<MyShape> object1, boost::shared_ptr<MyShape> object2, sgVec4 totalAngMom)
{
  sgVec4 sepVec, sepVecUnit;

  sgVec4 aPos, bPos;
  sgVec4 aMomentum, bMomentum;
  sgVec4 tempVec;
  sgVec4 hitPt;

  sgVec3 r, aMom3, bMom3;
  sgVec3 crossed = { 0, 0, 0 };

  object1->getPos(aPos);
  object2->getPos(bPos);
  sgSubVec4( sepVec, aPos, bPos );
  sgNormaliseVec4( sepVecUnit, sepVec );

  sgScaleVec4(tempVec, sepVecUnit, object1->getRadius());
  sgAddVec4(hitPt, aPos, tempVec);
  
  sgSubVec3( r, aPos, hitPt );

  for (int i = 0; i < 4; i++) {
    totalAngMom[i] = 0;
  }

  object1->getMomentum(aMomentum);
  object2->getMomentum(bMomentum);

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

  object1->getAngMomentum(tempVec);
  sgAddVec4(totalAngMom, tempVec);

  object2->getAngMomentum(tempVec);
  sgAddVec4(totalAngMom, tempVec);
}

void mergeObjects(boost::shared_ptr<MyShape> object1, boost::shared_ptr<MyShape> object2) 
{
  float newMass = object1->getMass() + object2->getMass();
  float density = object1->getDensity();

  float newRadius = calcMergedRadius(newMass, density);

  sgVec3 totalAngMom;

  calcMergedAngMomentum(object1, object2, totalAngMom);

  // COM start
  sgVec4 aPos, bPos;
  sgVec4 tempVec, tempVec2, COM;

  object1->getPos(aPos);
  object2->getPos(bPos);

  sgCopyVec4(tempVec, aPos);
  sgCopyVec4(tempVec2, bPos);

  sgScaleVec4(tempVec, object1->getMass());
  sgScaleVec4(tempVec2, object2->getMass());

  sgAddVec4(COM,tempVec, tempVec2);
  sgScaleVec4(COM, 1/(object1->getMass() + object2->getMass()) );
  // COM end

  sgVec4 object2momentum;
  object2->getMomentum(object2momentum);

  object1->setMass(newMass);
  object1->setRadius(newRadius);
  object1->adjustMomentum( object2momentum );

  object1->setAngMomentum(totalAngMom);
  sgVec3 angVel;
  object1->getAngMomentum(totalAngMom);
  object1->getAngVelocity(angVel);

  object1->calcColor();

  object1->setPos(COM);
}

float getSplitBodyRadius(float volume, int numPieces ) {
  float pieceVol = volume/numPieces;
  float pieceRadius = pieceVol/(M_PI * 4.0/3.0);
  pieceRadius = pow(pieceRadius, 1.0/3.0);

  return pieceRadius;
}

void randomSplitBodyPlacement(sgVec4 startPos, float pieceRadius, sgVec4 target) {
  int randMult;
  float largestDistance = 0;

  for (int i = 0; i < 3; i++)
  {
    randMult = rand()%50;
    if (randMult % 2 == 0) {
      randMult *= -1;
    }
    startPos[i] = randMult * pieceRadius;
    if ( startPos[i] > largestDistance )
    {
      largestDistance = startPos[i];
    }
  }

  startPos[3] = 1;

  sgAddVec4( startPos, target );
}

void randomSplitBodyPlacementInZone(sgVec4 startPos, sgVec4 volume, sgVec4 target ) {
  int randMult;
  int dimensionInteger;

  for (int i = 0; i < 3; i++)
  {
    dimensionInteger = (int) volume[i]/2;

    randMult = rand()%(dimensionInteger/2);
    if (randMult % 2 == 0) {
      randMult *= -1;
    }
    startPos[i] = randMult;
  }

  sgAddVec4( startPos, target );
}

void randomSplitBodyMomentum(sgVec4 startMom, float pieceMass) {
  static int randMult;

  static bool switchB = false;

  if (switchB)
  {
    for (int i = 0; i < 3; i++) {

      if (switchB)
      {
        // Set the range of momenta, and have them be half positive/half negative
        randMult = rand()%15;
        if (randMult % 2 == 0)
          randMult *= -1;


      }
      else {
        randMult *= -1;
      }
      startMom[i] = randMult * pieceMass * 0.00050; // Good mix
    }
  }


  if (switchB) {
    switchB = false;
  }
  else {
    switchB = true;
  }
  startMom[3] = 0;
}

//TODO Fix and only apply to a single shape
void calcDrag(float dt) {
  float dragConstant = -30;
  sgVec4 dragForce;
  boost::shared_ptr<MyShape> curShape;

  for (unsigned int i = 0; i < MyShape::shapes.size(); i++) {
    curShape = MyShape::shapes(i);
    curShape->getVelocity(dragForce);
    sgScaleVec4(dragForce, dragConstant);
    sgScaleVec4(dragForce, dt);
    curShape->adjustMomentum(dragForce);
  }
}
