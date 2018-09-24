#include "Interactions.h"

void elasticCollision( shapePointer_t object1, shapePointer_t object2, float dt) {
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
void calcForcesAll_LessNaive( SimulationPtr_t curSimulation )
{
  float dt = curSimulation->getDT();
  sgVec4 gravVec;
  shapePointer_t object1, object2;
  SGfloat distance, minSep;

  sgVec4 gravField = {0, 0, 0, 0};

  ShapeList shapeList = curSimulation->getPhysicalObjects() ;
  compressed_vector<shapePointer_t> physicalObjects = shapeList.getShapes();
  ShapeList deleteList;

  if (curSimulation->isConstGravField() ) {
    curSimulation->getConstGravFieldVal(gravField);
    sgScaleVec4(gravField, 1/dt);
  }
  
  if ( physicalObjects.size() > 0 ) {
    for (unsigned int i = 0; i < physicalObjects.size()-1; i++) {
      object1 = physicalObjects(i);

      object1->adjustMomentum(gravField);

      for (unsigned int j = i + 1; j < physicalObjects.size(); j++) {
        object2 = physicalObjects(j);


        if (curSimulation->isGravBetweenObjects() ) {
          calcForceGrav(gravVec, object1, object2, dt );

          object1->adjustMomentum(gravVec);
          sgNegateVec4(gravVec);
          object2->adjustMomentum(gravVec);
        }

        distance = object1->getDistanceToObject( object2 );
        minSep = object1->getRadius() + object2->getRadius();

        if (distance < minSep) {
          if (curSimulation->isAllElastic() ) {
            elasticCollision( object1, object2, dt );
          }
          else if (curSimulation->isAllInelastic() ){
            object1->mergeWith( object2 );
            deleteList.addShapeToList(object2);
          }
        }

      }
    }

    // Add unary forces to last object
    object1 = physicalObjects(physicalObjects.size()-1);
    object1->adjustMomentum(gravField);

    foreach_ ( shapePointer_t curShape, deleteList.getShapes() ) {
      curSimulation->shapes.removeShapeFromList(curShape);
    }

  }

}

ShapeList calcForceOnObject_Octree(shapePointer_t curObject, QuadrantPointer_t curQuadrant, float dt)
{
  sgVec4 gravVec;
  shapePointer_t object1, shapeInQuadrant;
  SGfloat distance;
  SGfloat theta = 0.5;

  ShapeList deleteList;

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
  if ( curQuadrant->isExternal() ) {
    shapeInQuadrant = curQuadrant->getShapeInQuadrant();

    //b.
    if ( shapeInQuadrant != nullptr && curObject != shapeInQuadrant ) {
      //c.
      calcForceGrav( gravVec, curObject, shapeInQuadrant, dt);

      if ( curObject->isTouching( shapeInQuadrant ) ) {
        deleteList.addShapeToList( shapeInQuadrant );
      }

      curObject->adjustMomentum(gravVec);
    }
  }
  else {
    distance = curObject->getDistanceToObject( curQuadrant );
    //2.
    //a.
    if ( curQuadrant->getWidth() / distance < theta ) {
      calcForceGrav( gravVec, curObject, curQuadrant, dt);

      //b.
      curObject->adjustMomentum(gravVec);
    }
    //3.
    else {
      QuadrantPointer_t targetQuadrant;
      for ( int x = 0; x < 2; x++ ) {
        for ( int y = 0; y < 2; y++ ) {
          for ( int z = 0; z < 2; z++ ) {
            targetQuadrant = curQuadrant->getQuadrantFromCell( x, y, z );
            if ( targetQuadrant != nullptr )
            {
              deleteList.addList( calcForceOnObject_Octree(curObject, targetQuadrant, dt) ) ;
            }
          }
        }
      }

    }
  }

  return deleteList;

}

void calcForcesAll( SimulationPtr_t curSimulation )
{
  if ( curSimulation->getForceCalcMethod() == Simulation::FORCE_CALC_METHOD_NAIVE  )
  {
    calcForcesAll_LessNaive( curSimulation );
  }
  else //Calculations with Octree
  {
      ShapeList deleteList;

      foreach_ ( shapePointer_t curShape, curSimulation->getPhysicalObjects().getShapes() ) {
          // TODO actually *use* the deleteList in some way. That should help avoid drawing merged/dead shapes.
        deleteList.addList( calcForceOnObject_Octree(curShape, curSimulation->getQuadrant(), curSimulation->getDT() ) );
      }

      foreach_ ( shapePointer_t curShape, deleteList.getShapes() ) {
        curSimulation->shapes.removeShapeFromList(curShape);
      }
  }
}

void calcForceGrav( sgVec4 gravVec, shapePointer_t object1, shapePointer_t object2, float dt )
{
  float forceMagnitude;
  SGfloat rSquared;
  sgVec4 sepVec, unitVec;

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
void calcDrag(float dt, compressed_vector<shapePointer_t> shapes) {
  float dragConstant = -30;
  sgVec4 dragForce;

  foreach_ ( shapePointer_t curShape, shapes ) {
    curShape->getVelocity(dragForce);
    sgScaleVec4(dragForce, dragConstant);
    sgScaleVec4(dragForce, dt);
    curShape->adjustMomentum(dragForce);
  }
}
