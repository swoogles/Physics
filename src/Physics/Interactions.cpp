#include "Interactions.h"

void elasticCollision( shapePointer_t object1, shapePointer_t object2, float dt) {
	sgVec4 sepVecUnit;

	sgVec4 tempVec, n, vdiff;

	SGfloat multiplier;

	vecPtr sepVec(object1->getVectorToObject(object2.get()));
	sgNormaliseVec4(sepVecUnit, sepVec->vec);

	vecPtr aVel(object1->getVelocity());
	vecPtr bVel(object2->getVelocity());

	sgCopyVec4(n, sepVecUnit);
	sgSubVec4(vdiff, aVel->vec, bVel->vec);

	float c = sgScalarProductVec4(n, vdiff);

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
// TODO This should return the deleted list, so that it can coexist with the Octree version
void calcForcesAll_LessNaive( SimulationPtr_t curSimulation )
{
  float dt = curSimulation->getDT();
  shapePointer_t object1, object2;
  SGfloat distance, minSep;

  sgVec4 gravField = {0, 0, 0, 0};

  ShapeList shapeList = curSimulation->getPhysicalObjects() ;
  vectorT physicalObjects = shapeList.getShapes();
  ShapeList deleteList;

  if (curSimulation->isConstGravField() ) {
    curSimulation->getConstGravFieldVal(gravField);
    sgScaleVec4(gravField, 1/dt);
  }
  
  if ( physicalObjects.size() > 0 ) {
    for (size_t i = 0; i < physicalObjects.size()-1; i++) {
      object1 = physicalObjects.at(i);

      object1->adjustMomentum(gravField);

      for (size_t j = i + 1; j < physicalObjects.size(); j++) {
        object2 = physicalObjects.at(j);


        if (curSimulation->isGravBetweenObjects() ) {
          vecPtr gravVec = calcForceGravNew(object1, object2, dt );

          object1->adjustMomentum(gravVec->vec);
          sgNegateVec4(gravVec->vec);
          object2->adjustMomentum(gravVec->vec);
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
    object1 = physicalObjects.at(physicalObjects.size()-1);
    object1->adjustMomentum(gravField);

    for ( const auto & curShape : deleteList.getShapes() ) {
      curSimulation->removePhysicalObject(curShape);
    }

  }

}

PairCollection calculateForceOnExternalNode(shapePointer_t curObject, QuadrantPointer_t curQuadrant, float dt) {
  //1.
  //a.
  PairCollection deleteList;
  shapePointer_t shapeInQuadrant = curQuadrant->getShapeInQuadrant();

  //b.
  if ( shapeInQuadrant != nullptr && curObject != shapeInQuadrant ) {
    //c.

    if ( curObject->isTouching( shapeInQuadrant ) ) {
        // std::cout << "touching!" << std::endl;
        // TODO handle merging in outter code
//      curObject->mergeWith(shapeInQuadrant);
      TouchingPair pair(curObject, shapeInQuadrant);
      deleteList.insertIfUnique(pair);
    } else {
      vecPtr gravVec = calcForceGravNew( curObject, shapeInQuadrant, dt);
      curObject->adjustMomentum(gravVec->vec);
    }
  }

  return deleteList;
}
//1.
//a. If the current node is an external node
//b. (and it is not body b),
//c. calculate the force exerted by the current node on b
//d. add this amount to b’s net force.
//2.
//a. Otherwise, calculate the ratio s/d. If s/d < θ, treat this internal node as a single body, and calculate the force it exerts on body b
//b. add this amount to b’s net force.
//3. Otherwise, run the procedure recursively on each of the current node’s children.


// TODO Maybe if I add *pairs* of items to deleteList, I can normalize that and not worry about deleting both sides of a collision.
PairCollection calcForceOnObject_Octree(shapePointer_t curObject, QuadrantPointer_t curQuadrant, float dt, int recursionLevel)
{

    if (recursionLevel > 100) {
        exit(1);
    }

  if ( curQuadrant->isExternal() ) {
    return calculateForceOnExternalNode(curObject, curQuadrant, dt);
  }
  else {
      PairCollection deleteList;
      SGfloat distance = curObject->getDistanceToObject( curQuadrant );
      SGfloat theta = 0.5;
      //2.
      //a.
      if ( curQuadrant->getWidth() / distance < theta ) {
          vecPtr gravVec = calcForceGravNew( curObject, curQuadrant, dt);
          //b.
          curObject->adjustMomentum(gravVec->vec);
      } else { //3.
          QuadrantPointer_t targetQuadrant;
          for ( int x = 0; x < 2; x++ ) {
              for ( int y = 0; y < 2; y++ ) {
                  for ( int z = 0; z < 2; z++ ) {
                      targetQuadrant = curQuadrant->getQuadrantFromCell( x, y, z );
                      if ( targetQuadrant != nullptr ) {
                          deleteList.insertUniqueElements(calcForceOnObject_Octree(curObject, targetQuadrant, dt, recursionLevel + 1)) ;
                      }
                  }
              }
          }

      }
      return deleteList;
  }


}

void calcForcesAll( SimulationPtr_t curSimulation )
{
    // std::cout << "doing anything?!" << std::endl;
  switch(curSimulation->getForceCalcMethod()) {
      // TODO are these getting inverted somewhere along the way?
      case ForceCalculationMethod::NAIVE:
          // std::cout << "calculating naively" << std::endl;
        calcForcesAll_LessNaive( curSimulation );
        break;
      case ForceCalculationMethod ::OCTREE:
        PairCollection deleteList;

        for ( const auto & curShape : curSimulation->getPhysicalObjects().getShapes() ) {
          deleteList.insertUniqueElements(calcForceOnObject_Octree(curShape, curSimulation->getQuadrant(), curSimulation->getDT(), 0));
        }

        // TODO whoops, I'm dropping all items that *didn't* merge here. Bit of an oversite.
          curSimulation->update();

          for ( const auto & curShape : deleteList.doomed().getShapes() ) {
              curSimulation->removePhysicalObject(curShape);
          }
          deleteList.mergePairs();
          break;
        // TODO do merging here.

        // TODO curSimulation.quadrant needs to die *before* any of my shapes will actually go away!
  }
}

vecPtr calcForceGravNew( shapePointer_t object1, shapePointer_t object2, float dt )
{
  vecPtr gravVec(new VecStruct());

  sgVec4 unitVec;

  vecPtr sepVec(object1->getVectorToObject(object2.get()));

  SGfloat rSquared = sgLengthSquaredVec4(sepVec->vec);

  if (rSquared < .00001) {
    rSquared = .00001;
  }
  float forceMagnitude = (Simulations::G * object1->getMass() * object2->getMass()) / rSquared;

  sgNormaliseVec4(unitVec, sepVec->vec);

  sgScaleVec4(gravVec->vec, unitVec, forceMagnitude);
  sgScaleVec4(gravVec->vec, dt);

  return  gravVec;
}

//TODO Fix and only apply to a single shape
void calcDrag(float dt, vectorT shapes) {
  float dragConstant = -30;

  for ( const auto & curShape : shapes ) {
    vecPtr dragForce(curShape->getVelocity());
    sgScaleVec4(dragForce->vec, dragConstant);
    sgScaleVec4(dragForce->vec, dt);
    curShape->adjustMomentum(dragForce->vec);
  }
}
