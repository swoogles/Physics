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

	object1->getVectorToObject( object2, sepVec);
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
	while ( object1->isTouching(object2) ) {
		object1->update(dt/30);
		object2->update(dt/30);
	}
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

        object1->getVectorToObject( object2, sepVec);

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

// I'm working on this method because I think there are some basic
// structural changes that I can make so that it will be more legible and
// more suitable for later parallelization
void calcForcesAll_LessNaive( boost::shared_ptr<Simulation> curSimulation ) 
{
  compressed_vector<shape_pointer> physicalObjects = curSimulation->getPhysicalObjects().getShapes();
  float dt = curSimulation->getDT();
  sgVec4 gravVec;
  boost::shared_ptr<MyShape> object1, object2;

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

      if (curSimulation->isConstGravField() ) {
        object1->adjustMomentum(gravField);
      }

      for (unsigned int j = i + 1; j < physicalObjects.size(); )
      {

        if (curSimulation->isGravBetweenObjects() ) {
          calcForceGravNew(gravVec, object1, object2, dt );


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
      curObject->getVectorToObject( curQuadrant->getShapeInQuadrant(), sepVec);
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

      object1->getVectorToObject( object2, sepVec);

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
          object1->mergeWith( object2 );
          // mergeObjects(object1, object2);
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

void calcForceGravNew( sgVec4 gravVec, boost::shared_ptr<MyShape> object1, boost::shared_ptr<MyShape> object2, float dt ) 
{
  float forceMagnitude;
  SGfloat rSquared;
  sgVec4 sepVec, unitVec;

  sgVec4 pos;
  object1->getPos(pos);

  object1->getVectorToObject( object2, sepVec);

  rSquared = sgLengthSquaredVec4(sepVec);

  if (rSquared < .00001)
  {
    rSquared = .00001;
  }
  forceMagnitude = (Simulations::G * object1->getMass() * object2->getMass()) / rSquared;

  sgNormaliseVec4(unitVec, sepVec);

  sgScaleVec4(gravVec, unitVec, forceMagnitude);
  sgScaleVec4(gravVec, dt);

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
