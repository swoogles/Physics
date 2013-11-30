/*
 * Interactions.cpp
 *
 *  Created on: Jul 21, 2011
 *      Author: brasure
 */

#include "Interactions.h"
#include "Simulations.h"
//#include "../ShapeFiles/MyShape.h"

using namespace std;

/*
ostream& operator<<(ostream& os, sgVec3 outputVec) {
	cout << "<" << outputVec[0] << ", " << outputVec[1] << ", " << outputVec[2] << ">";
}
*/

void elasticCollision( shared_ptr<MyShape> object1, shared_ptr<MyShape> object2) {
	sgVec4 sepVec;
	sgVec4 sepVecUnit;

	//SGfloat distance, distanceSquared;

	//sgVec4 aPos, bPos;
	//sgVec4 aMomentum, bMomentum;
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

	while (contact(object1, object2)) {
		object1->update(WorldSettings::getDT()/30);
		object2->update(WorldSettings::getDT()/30);
	}


}

bool contact(shared_ptr<MyShape> object1, shared_ptr<MyShape> object2) {
	sgVec4 sepVec;
	float minSep;

	SGfloat distance, distanceSquared;

	getVectorToObject2(object1, object2, sepVec);

	distanceSquared = sgLengthSquaredVec4(sepVec);
	distance = sqrt(distanceSquared);

	minSep = object1->getRadius() + object2->getRadius();

	if (distance < minSep)
		return true;
	else
		return false;
}

void calcForcesAll_ArbitraryList(compressed_vector<shape_pointer> physicalObjects, float dt) {
		sgVec4 sepVec;
		sgVec4 unitVec;
		sgVec4 gravVec;
    shared_ptr<MyShape> object1;
    shared_ptr<MyShape> object2;
		float fGrav;
		SGfloat distanceSquared;

		sgVec4 gravField;

		bool killed = false;

		//bool constGravField = WorldSettings::isConstGravField();

		if (WorldSettings::isConstGravField() ) {
			WorldSettings::getConstGravFieldVal(gravField);
			sgScaleVec4(gravField, 1/dt);
		}
    // cout << "Calc forces" << endl;

		//sgVec4 * ob1mom;
    if ( physicalObjects.size() > 0 )
    {
      for (unsigned int i = 0; i < physicalObjects.size()-1; i++)
      {
        // cout << "\tCalc forces for object: " << i << endl;
        if (killed) {
          // cout << "curI: " << i << endl;
        }
        object1 = physicalObjects(i);
        sgVec4 pos;
        object1->getPos(pos);
        // cout << "shape.pos: " << pos[0] << "," << pos[1] << "," << pos[2] << endl;

        if (WorldSettings::isConstGravField() ) {
          object1->adjustMomentum(gravField);
        }

        for (unsigned int j = i + 1; j < physicalObjects.size(); )
        {
          // cout << "\t\tCalc forces with object: " << j << endl;
          object2 = physicalObjects(j);
          object2->getPos(pos);
          // cout << "shape.pos: " << pos[0] << "," << pos[1] << "," << pos[2] << endl;


          getVectorToObject2(object1, object2, sepVec);
          // cout << "sepVec: " << sepVec[0] << "," << sepVec[1] << "," << sepVec[2] << endl;

          distanceSquared = sgLengthSquaredVec4(sepVec);
          // cout <<"Distance Squared: " <<  distanceSquared << endl;;

          if (WorldSettings::isGravBetweenObjects() ) {
            fGrav = calcForceGrav(object1, object2, distanceSquared);
            // cout << "fGrav from Shape: " << fGrav << endl;

            sgNormaliseVec4(unitVec, sepVec);

            sgScaleVec4(gravVec, unitVec, fGrav);
            sgScaleVec4(gravVec, dt);

            object1->adjustMomentum(gravVec);
            sgNegateVec4(gravVec);
            object2->adjustMomentum(gravVec);
          }

          j++;
        }

      }

      // Add unary forces to last object
      object1 = physicalObjects(physicalObjects.size()-1);

      if (WorldSettings::isConstGravField() ) {
        object1->adjustMomentum(gravField);
      }
    }

}

void calcForceOnObject_Octree(shape_pointer curObject, shared_ptr<Quadrant> curQuadrant, float dt) {
  sgVec4 sepVec;
  sgVec4 unitVec;
  sgVec4 gravVec;
  shared_ptr<MyShape> object1;
  shared_ptr<MyShape> object2;
  shared_ptr<MyShape> shapeInQuadrant;
  float fGrav;
  SGfloat distance;
  SGfloat distanceSquared;
  SGfloat theta = 0.5;
  typedef shared_ptr<Quadrant> quad_pointer;

  sgVec4 pos;
  curObject->getPos(pos);
  // cout << "curObject.pos: " << pos[0] << "," << pos[1] << "," << pos[2] << endl;
  curQuadrant->getPos(pos);
  // cout << "Quad.pos: " << pos[0] << "," << pos[1] << "," << pos[2] << endl;
  // shapeInQuadrant = curQuadrant->getShapeInQuadrant();
  // if ( shapeInQuadrant )
  // {
  // }


  if ( curQuadrant->getShapeInQuadrant() )
  {
    getVectorToObject2( curObject, curQuadrant->getShapeInQuadrant(), sepVec);
  }
  else if ( curQuadrant )
  {
    getVectorToObject2( curObject, curQuadrant, sepVec);
  }
  distance = sgLengthVec4( sepVec );
  distanceSquared = sgLengthSquaredVec4( sepVec );


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
    // cout << "1" << endl;
    shapeInQuadrant = curQuadrant->getShapeInQuadrant();

    // sgVec4 pos;
    // curQuadrant->getPos(pos);
    // cout << "Quad.pos: " << pos[0] << "," << pos[1] << "," << pos[2] << endl;
    // shapeInQuadrant->getPos(pos);
    // cout << "shape.pos: " << pos[0] << "," << pos[1] << "," << pos[2] << endl;

    //b.
    if ( shapeInQuadrant != NULL && curObject != shapeInQuadrant )
    {
      // cout << "1.a" << endl;
      //c.
      // cout <<"Distance Squared: " <<  distanceSquared << endl;;
      fGrav = calcForceGrav(curObject, shapeInQuadrant, distanceSquared);
      // cout << "fGrav from Shape: " << fGrav << endl;
      // cout << "1.b" << endl;

      sgNormaliseVec4(unitVec, sepVec);

      sgScaleVec4(gravVec, unitVec, fGrav);
      sgScaleVec4(gravVec, dt);

      // cout << "1.c" << endl;
      // sgVec3 curMomentum;
      // curObject->getMomentum( curMomentum );
      // cout << "iMomentum: " << curMomentum[0] << "," << curMomentum[1] << "," << curMomentum[2] << endl;
      curObject->adjustMomentum(gravVec);
      // cout << "fMomentum: " << curMomentum[0] << "," << curMomentum[1] << "," << curMomentum[2] << endl;
      // cout << "1.d" << endl;
      // Only used during the naive n^2 approach
      // sgNegateVec4(gravVec);
      // object2->adjustMomentum(gravVec);
      // cout << "1.end" << endl;

    }
  }
  else
  {
    //2.
    //a.
    // cout << "2" << endl;
    if ( curQuadrant->getWidth() / distance < theta )
    {
      fGrav = calcForceGrav(curObject, curQuadrant, distanceSquared);

      sgNormaliseVec4(unitVec, sepVec);

      sgScaleVec4(gravVec, unitVec, fGrav);
      sgScaleVec4(gravVec, dt);

      //b.
      // cout << "fGrav from Quad: " << fGrav << endl;
      curObject->adjustMomentum(gravVec);
      
    }
    //3.
    else
    {
      // cout << "3" << endl;
      quad_pointer targetQuadrant;
      for ( int x = 0; x < 2; x++ )
      {
        for ( int y = 0; y < 2; y++ )
        {
          for ( int z = 0; z < 2; z++ )
          {

            targetQuadrant = curQuadrant->getQuadrantFromCell( x, y, z );
            if ( targetQuadrant != NULL )
            {
              calcForceOnObject_Octree(curObject, targetQuadrant, dt);

              // targetShapeList = targetQuadrant->getShapesRecursive( curLevel  );
              // foreach_ ( shape_pointer curShape, targetShapeList.getShapes() )
              // {
              //   totalShapeList.addShapeToList( curShape );
              // }

            }

          }
        }
      }
    }
  }

}

void calcForcesAll_ArbitraryListWithOctree(compressed_vector<shape_pointer> physicalObjects, shared_ptr<Quadrant> octree, float dt) {
		sgVec4 sepVec;
		sgVec4 unitVec;
		sgVec4 gravVec;
    shared_ptr<MyShape> object1;
    shared_ptr<MyShape> object2;
		float fGrav;
		SGfloat distanceSquared;

		sgVec4 gravField;

		bool killed = false;

		if (WorldSettings::isConstGravField() ) {
			WorldSettings::getConstGravFieldVal(gravField);
			sgScaleVec4(gravField, 1/dt);
		}

    foreach_ ( shape_pointer curShape, physicalObjects )
    {
      getVectorToObject2(object1, object2, sepVec);
    }

    if ( physicalObjects.size() > 0 )
    {
      for (unsigned int i = 0; i < physicalObjects.size()-1; i++)
      {
        if (killed) {
          // cout << "curI: " << i << endl;
        }
        object1 = physicalObjects(i);

        if (WorldSettings::isConstGravField() ) {
          object1->adjustMomentum(gravField);
        }

        for (unsigned int j = i + 1; j < physicalObjects.size(); )
        {
          object2 = physicalObjects(j);


          getVectorToObject2(object1, object2, sepVec);

          distanceSquared = sgLengthSquaredVec4(sepVec);

          if (WorldSettings::isGravBetweenObjects() ) {
            fGrav = calcForceGrav(object1, object2, distanceSquared);

            sgNormaliseVec4(unitVec, sepVec);

            sgScaleVec4(gravVec, unitVec, fGrav);
            sgScaleVec4(gravVec, dt);

            object1->adjustMomentum(gravVec);
            sgNegateVec4(gravVec);
            object2->adjustMomentum(gravVec);
          }

          j++;
        }

      }

      // Add unary forces to last object
      object1 = physicalObjects(physicalObjects.size()-1);

      if (WorldSettings::isConstGravField() ) {
        object1->adjustMomentum(gravField);
      }
    }

}

void calcForcesAll( shared_ptr<Simulation> curSimulation )
{
  ShapeList physicalObjects = curSimulation->getPhysicalObjects();

  if ( ! curSimulation->getForceCalcMethod() == Simulation::FORCE_CALC_METHOD_NAIVE  )
  {
    calcForcesAll_ArbitraryList(physicalObjects.getShapes(), curSimulation->getDT());

    foreach_ ( shape_pointer curShape, physicalObjects.getShapes() )
    {
      curShape->update( curSimulation->getDT() );

      // TODO this should be taking advantage of a passed in Observer, rather than the static worldsettings junk
      // sgVec4 curPos;
      // if (WorldSettings::isAutoScaling())
      // {
      //   curShape->getPos(curPos);
      //   WorldSettings::updateXYMinsAndMaxes(curPos);
      // }

    }
  }
  else //Calculations with Octree
  {
    if ( physicalObjects.getShapes().size() > 0 )
    {
      ShapeList shapeList;

      if ( curSimulation->getCurStep() % 100 == 0 )
      {
        cout << "Shapelist.size: " << physicalObjects.getShapes().size() << endl;
      }
      // int z=0;
      foreach_ ( shape_pointer curShape, physicalObjects.getShapes() )
      {
        calcForceOnObject_Octree(curShape, curSimulation->getQuadrant(), curSimulation->getDT() );
        curShape->update( curSimulation->getDT() );
        // TODO this should be taking advantage of a passed in Observer, rather than the static worldsettings junk
        // if (WorldSettings::isAutoScaling())
        // {
        //   curShape->getPos(curPos);
        // }
      }

    }
  }

}

bool isConflict(int newShape) {
  sgVec4 sepVec;
  SGfloat distanceSquared, distance, minSep;
  bool conflict = false;

  shared_ptr<MyShape> object1;
  shared_ptr<MyShape> object2;

  object2 = MyShape::shapes(newShape);

  for (int i = 0; i < newShape && conflict == false; i++) {
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

bool isConflict_ArbitraryList( compressed_vector<shape_pointer> physicalObjects, int newShape) {
  sgVec4 sepVec;
  SGfloat distanceSquared, distance, minSep;
  bool conflict = false;

  shared_ptr<MyShape> object1;
  shared_ptr<MyShape> object2;

  object2 = physicalObjects(newShape);

  for (int i = 0; i < newShape && conflict == false; i++) {
    object1 = physicalObjects(i);
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

void calcCollisionsAll(shared_ptr<Simulation> curSimulation) {

  ShapeList physicalObjects = curSimulation->getPhysicalObjects() ;
  calcCollisionsAll_ShapeList( physicalObjects );
  curSimulation->setPhysicalObjects( physicalObjects) ;

}

void calcCollisionsAll_ShapeList( ShapeList & shapeList ) {
  sgVec4 sepVec;
  shared_ptr<MyShape> object1;
  shared_ptr<MyShape> object2;
  SGfloat distanceSquared, distance, minSep;

  compressed_vector<shape_pointer> physicalObjects = shapeList.getShapes();
  compressed_vector<shape_pointer> deleteList;

  bool killed = false;

  sgVec4 gravField;

  for (unsigned int i = 0; i < physicalObjects.size()-1; i++)
  {
    // cout << "Outter loop." << endl;
    if (killed) {
      // cout << "curI: " << i << endl;
    }
    object1 = physicalObjects(i);

    if (WorldSettings::isConstGravField() ) {
      object1->adjustMomentum(gravField);
    }

    for (unsigned int j = i + 1; j < physicalObjects.size(); j++)
    {
      // cout << "Stuck in inner loop " << endl;
      // cout << "PhysicalObjects.size: " << physicalObjects.size() << endl;;
      object2 = physicalObjects(j);

      getVectorToObject2(object1, object2, sepVec);

      distanceSquared = sgLengthSquaredVec4(sepVec);
      distance = sqrt(distanceSquared);

      minSep = object1->getRadius() + object2->getRadius();


      if (distance < minSep) {

        if (WorldSettings::isAllElastic() ) {
          elasticCollision(object1,object2);
          j++;
        }

        else if (WorldSettings::isAllInelastic() ){
          mergeObjects(object1, object2);
          deleteList.resize(deleteList.size()+1);
          deleteList.insert_element(deleteList.size()-1, object2);
          j++;

        }

      }
      else {
        j++;
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

}

//setting rSquared to a constant value with a grid can make some awesome effects
float calcForceGrav(shared_ptr<MyShape> object1, shared_ptr<MyShape> object2, SGfloat rSquared) {
  if (rSquared < .00001)
  {
    rSquared = .00001;
  }
  // cout << "Obj1.mass" << object1->getMass() << "\t\tObj2.mass" << object2->getMass() << endl;
  return ( MyShape::G * object1->getMass() * object2->getMass()) / rSquared;
}

void getVectorToObject2(shared_ptr<MyShape> object1, shared_ptr<MyShape> object2, sgVec4 sepVector) {
  sgVec4 pos1, pos2;
  object1->getPos(pos1 );
  object2->getPos(pos2 );
  // cout << "--Subtracting " << pos1[0] << "," << pos1[1] << "," << pos1[2] << endl;
  // cout << "--from        " << pos2[0] << "," << pos2[1] << "," << pos2[2] << endl;
  sgSubVec4(sepVector, pos2, pos1);
}

float calcMergedRadius(float massBoth, float density) {
  float radius = massBoth/density;
  radius /= (4/3.0);
  radius = pow(radius, 1.0/3);
  return radius;
}

void calcMergedAngMomentum(shared_ptr<MyShape> object1, shared_ptr<MyShape> object2, sgVec4 retAngMomentum)
{
  sgVec4 sepVecUnit;

  sgVec4 aPos, bPos;
  sgVec4 aMomentum, bMomentum;
  sgVec4 tempVec, tempVec2;
  sgVec4 hitPt;
  sgVec3 totalAngMom3;

  sgVec3 r, aMom3, bMom3, COM;
  sgVec3 crossed;

  object1->getPos(aPos);
  object2->getPos(bPos);
  sgScaleVec4(tempVec, sepVecUnit, object1->getRadius());
  sgAddVec4(hitPt, aPos, tempVec);

  for (int i = 0; i < 3; i++) {
    totalAngMom3[i] = 0;
  }


  // COM Calc Start
  sgCopyVec4(tempVec, aPos);
  sgCopyVec4(tempVec2, bPos);

  sgScaleVec4(tempVec, object1->getMass());
  sgScaleVec4(tempVec2, object2->getMass());

  sgAddVec4(COM,tempVec, tempVec2);
  sgScaleVec4(COM, 1/(object1->getMass() + object2->getMass()) );
  // COM Calc End

  object1->getMomentum(aMomentum);
  object2->getMomentum(bMomentum);

  sgSubVec3( r, aPos, hitPt );

  aMom3[0] = aMomentum[0];
  aMom3[1] = aMomentum[1];
  aMom3[2] = aMomentum[2];
  sgVectorProductVec3(crossed, r, aMom3);

  sgAddVec3(totalAngMom3, crossed);

  sgSubVec3( r, bPos, hitPt );

  bMom3[0] = bMomentum[0];
  bMom3[1] = bMomentum[1];
  bMom3[2] = bMomentum[2];
  sgVectorProductVec3(crossed, r, bMom3);

  sgAddVec3(totalAngMom3, crossed);

  retAngMomentum[0] = totalAngMom3[0];
  retAngMomentum[1] = totalAngMom3[1];
  retAngMomentum[2] = totalAngMom3[2];



  object1->getAngMomentum(tempVec);

  sgAddVec4(retAngMomentum, tempVec);

  object2->getAngMomentum(tempVec);

  sgAddVec4(retAngMomentum, tempVec);

}

void mergeObjects(shared_ptr<MyShape> object1, shared_ptr<MyShape> object2) {
  sgVec3 color;

  object1->getColor( color );
  object2->getColor( color );

  cout << "Merging!" << endl;

  float newMass = object1->getMass() + object2->getMass();
  float density = object1->getDensity();

  float newRadius = calcMergedRadius(newMass, density);

  // object1->getRadius();
  // cout << "Merged Object radius:" << object1->getRadius() << endl;

  sgVec4 totalAngMom;

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

  sgVec4 object2AngMom;

  object2->getAngMomentum(object2AngMom);

  object1->setMass(newMass);
  object1->setRadius(newRadius);
  object1->adjustMomentum( object2momentum );

  //object1->adjustAngMomentum(object2AngMom);
  object1->setAngMomentum(totalAngMom);

  object1->calcColor();

  object1->setPos(COM);

  object1->getColor( color );
  //TODO Do AngMomentum calculations

  //cout << MASS_SUN << endl;
  // delete object2momentum;

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
    //randMult = rand()%30; //Earth spacing
    randMult = rand()%30;
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
    //randMult = rand()%30; //Earth spacing
    dimensionInteger = (int) volume[i]/2;

    randMult = rand()%(dimensionInteger/2);
    if (randMult % 2 == 0) {
      randMult *= -1;
    }
    startPos[i] = randMult;
  }

  startPos[3] = 1;

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
        randMult = rand()%5;
        if (randMult % 2 == 0)
          randMult *= -1;


      }
      else {
        randMult *= -1;
      }

      //randMult*=.3;

      //startMom[i] = randMult * pieceMass * 0.00060; //Mainly escape
      //startMom[i] = randMult * pieceMass * 0.00080; // Good mix
      startMom[i] = randMult * pieceMass * 0.00050; // Good mix
      //startMom[i] = randMult * pieceMass * 0.00008; //Quick Collapse

      //startMom[i] = randMult * pieceMass * 0.000023; //Earth 500 pieces
    }
  }


  if (switchB)
    switchB = false;
  else
    switchB = true;
  startMom[3] = 0;
}

//TODO Fix and only apply to a single shape
void calcDrag(float dt) {
  float dragConstant = -30;
  sgVec4 dragForce;
  shared_ptr<MyShape> curShape;

  for (unsigned int i = 0; i < MyShape::shapes.size(); i++) {
    curShape = MyShape::shapes(i);
    curShape->getVelocity(dragForce);
    sgScaleVec4(dragForce, dragConstant);
    sgScaleVec4(dragForce, dt);
    curShape->adjustMomentum(dragForce);
  }
}
