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

// I'm working on this method because I think there are some basic
// structural changes that I can make so that it will be more legible and
// more suitable for later parallelization
void calcForcesAll_LessNaive( boost::shared_ptr<Simulation> curSimulation ) 
{
  float dt = curSimulation->getDT();
  sgVec4 gravVec;
  boost::shared_ptr<MyShape> object1, object2;
  SGfloat distance, minSep;

  sgVec4 gravField = {0, 0, 0, 0};

  ShapeList shapeList = curSimulation->getPhysicalObjects() ;
  compressed_vector<shape_pointer> physicalObjects = shapeList.getShapes();
  compressed_vector<shape_pointer> deleteList;

  if (curSimulation->isConstGravField() ) {
    curSimulation->getConstGravFieldVal(gravField);
    sgScaleVec4(gravField, 1/dt);
  }
  
  if ( physicalObjects.size() > 0 )
  {
    for (unsigned int i = 0; i < physicalObjects.size()-1; i++)
    {
      object1 = physicalObjects(i);

      object1->adjustMomentum(gravField);

      for (unsigned int j = i + 1; j < physicalObjects.size(); j++)
      {
        object2 = physicalObjects(j);


        if (curSimulation->isGravBetweenObjects() ) {
          calcForceGrav(gravVec, object1, object2, dt );

          object1->adjustMomentum(gravVec);
          sgNegateVec4(gravVec);
          object2->adjustMomentum(gravVec);
        }

        distance = object1->getDistanceToObject( object2 );
        minSep = object1->getRadius() + object2->getRadius();

        if (distance < minSep)
        {
          if (curSimulation->isAllElastic() )
          {
            elasticCollision( object1, object2, curSimulation->getDT() );
          }
          else if (curSimulation->isAllInelastic() ){
            object1->mergeWith( object2 );
            deleteList.resize(deleteList.size()+1);
            deleteList.insert_element(deleteList.size()-1, object2);
          }
        }

      }
    }

    // Add unary forces to last object
    object1 = physicalObjects(physicalObjects.size()-1);

    object1->adjustMomentum(gravField);

    if ( deleteList.size() > 0 )
    {
      foreach_ ( shape_pointer curShape, deleteList )
      {
        shapeList.removeShapeFromList( curShape );
        MyShape::removeShapeFromList( curShape );
      }
      cout << endl << endl;
    }

    curSimulation->setPhysicalObjects( shapeList ) ;
  }

}

void calcForceOnObject_Octree(shape_pointer curObject, boost::shared_ptr<Quadrant> curQuadrant, float dt) 
{
  sgVec4 gravVec;
  boost::shared_ptr<MyShape> object1, shapeInQuadrant;
  SGfloat distance;
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
      //c.
      calcForceGrav( gravVec, curObject, shapeInQuadrant, dt);

      curObject->adjustMomentum(gravVec);
    }
  }
  else
  {
    distance = curObject->getDistanceToObject( curQuadrant );
    //2.
    //a.
    if ( curQuadrant->getWidth() / distance < theta )
    {
      calcForceGrav( gravVec, curObject, curQuadrant, dt);

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
    calcForcesAll_LessNaive( curSimulation );
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
  boost::shared_ptr<MyShape> object1, object2;
  SGfloat distance, minSep;

  compressed_vector<shape_pointer> physicalObjects = shapeList.getShapes();
  compressed_vector<shape_pointer> deleteList;

  for (unsigned int i = 0; i < physicalObjects.size()-1; i++)
  {
    object1 = physicalObjects(i);

    for (unsigned int j = i + 1; j < physicalObjects.size(); j++)
    {
      object2 = physicalObjects(j);

      distance = object1->getDistanceToObject( object2 );

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

void calcForceGrav( sgVec4 gravVec, boost::shared_ptr<MyShape> object1, boost::shared_ptr<MyShape> object2, float dt ) 
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
