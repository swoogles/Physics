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

ostream& operator<<(ostream& os, sgVec4 outputVec) {
	cout << "<" << outputVec[0] << ", " << outputVec[1] << ", " << outputVec[2] << ">";
	return os;
}


void elasticCollision(MyShape * object1, MyShape * object2) {
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

bool contact(MyShape * object1, MyShape * object2) {
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

void calcForcesAll(float dt) {
		//cout << "Calcing" << endl;
		sgVec4 sepVec;
		sgVec4 unitVec;
		sgVec4 gravVec;
		MyShape * object1, * object2;
		float fGrav;
    float minSep;
		SGfloat distanceSquared;
		SGfloat distance;

		sgVec4 gravField;

		bool killed = false;

		//bool constGravField = WorldSettings::isConstGravField();

		if (WorldSettings::isConstGravField() ) {
			WorldSettings::getConstGravFieldVal(gravField);
			sgScaleVec4(gravField, 1/dt);
		}

		//sgVec4 * ob1mom;
    unsigned int j = 0; 
    if ( MyShape::shapes.size() > 0 )
    {
      for (unsigned int i = 0; i < MyShape::shapes.size()-1; i++)
      {
        if (killed) {
          // cout << "curI: " << i << endl;
        }
        object1 = MyShape::shapes(i);

        if (WorldSettings::isConstGravField() ) {
          object1->adjustMomentum(gravField);
        }

        for (unsigned int j = i + 1; j < MyShape::shapes.size(); NULL)
        {
          object2 = MyShape::shapes(j);


          getVectorToObject2(object1, object2, sepVec);

          distanceSquared = sgLengthSquaredVec4(sepVec);
          distance = sqrt(distanceSquared);

          minSep = object1->getRadius() + object2->getRadius();


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
      object1 = MyShape::shapes(MyShape::shapes.size()-1);

      if (WorldSettings::isConstGravField() ) {
        object1->adjustMomentum(gravField);
      }
    }

}

//void calcHitAndMerge

bool isConflict(int newShape) {
  sgVec4 sepVec;
  SGfloat distanceSquared, distance, minSep;
  bool conflict = false;

  MyShape * object1, * object2;

  object2 = MyShape::shapes(newShape);

  for (unsigned int i = 0; i < newShape && conflict == false; i++) {
    object1 = MyShape::shapes(i);
    getVectorToObject2(object1, object2, sepVec);

    distanceSquared = sgLengthSquaredVec4(sepVec);
    distance = sqrt(distanceSquared);

    minSep = object1->getRadius() + object2->getRadius();


    if (distance < minSep) {
      conflict = true;
    }
  }
  return conflict;
}

void calcCollisionsAll() {
  sgVec4 sepVec;
  MyShape * object1, * object2;
  SGfloat distanceSquared, distance, minSep;

  bool killed = false;

  sgVec4 gravField;

  for (unsigned int i = 0; i < MyShape::shapes.size()-1; i++)
  {
    if (killed) {
      // cout << "curI: " << i << endl;
    }
    object1 = MyShape::shapes(i);

    if (WorldSettings::isConstGravField() ) {
      object1->adjustMomentum(gravField);
    }

    for (unsigned int j = i + 1; j < MyShape::shapes.size(); NULL)
    {
      //cout << "Stuck in inner loop " << endl;
      object2 = MyShape::shapes(j);

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
          object2->~MyShape();

          MyShape::shapes.erase_element(j);
          for (unsigned int curPos = j; curPos < MyShape::shapes.size()-1; curPos++) {
            MyShape::shapes(curPos) = MyShape::shapes(curPos+1);
          }
          MyShape::shapes.resize(MyShape::shapes.size()-1, true);
        }

      }
      else {
        j++;
      }
    }
  }

}

//setting rSquared to a constant value with a grid can make some awesome effects
float calcForceGrav(MyShape * object1, MyShape * object2, SGfloat rSquared) {
  sgVec4 *ob1pos, *ob2pos;
  ob1pos = object1->getPos();
  ob2pos = object2->getPos();
  //float rSquared = sgDistanceSquaredVec4( (*ob1pos), (*ob2pos) );
  //float rSquared = 2;
  //float G = object1->G;
  if (rSquared < .00001)
  {
    rSquared = .00001;
  }

  delete ob1pos;
  delete ob2pos;

  return ( MyShape::G * object1->getMass() * object2->getMass()) / rSquared;
}

void getVectorToObject2(MyShape * object1, MyShape * object2, sgVec4 sepVector) {
  sgVec4 * pos1, *pos2;
  pos1 = object1->getPos();
  pos2 = object2->getPos();

  //cout << "Pos1: " << *pos1 << endl;
  //cout << "Pos2: " << *pos2 << endl;

  sgSubVec4(sepVector, *pos2, *pos1);

  delete pos1;
  delete pos2;

}

float calcMergedRadius(float massBoth, float density) {
  float radius = massBoth/density;
  radius /= (4/3.0);
  radius = pow(radius, 1.0/3);
  return radius;
}

void calcMergedAngMomentum(MyShape * object1, MyShape * object2, sgVec4 retAngMomentum)
{
  sgVec4 sepVecUnit;

  sgVec4 aPos, bPos;
  sgVec4 aMomentum, bMomentum;
  sgVec4 tempVec, tempVec2;
  sgVec4 hitPt;
  sgVec3 totalAngMom;
  sgVec3 totalAngMom3;

  sgVec3 r, aMom3, bMom3, COM;
  sgVec3 crossed;

  object1->getPos(aPos);
  object2->getPos(bPos);
  sgScaleVec4(tempVec, sepVecUnit, object1->getRadius());
  sgAddVec4(hitPt, aPos, tempVec);

  for (int i = 0; i < 3; i++) {
    totalAngMom[i] = 0;
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

  r[0] = aPos[0] - hitPt[0];
  r[1] = aPos[1] - hitPt[1];
  r[2] = aPos[2] - hitPt[2];

  aMom3[0] = aMomentum[0];
  aMom3[1] = aMomentum[1];
  aMom3[2] = aMomentum[2];
  sgVectorProductVec3(crossed, r, aMom3);

  sgAddVec3(totalAngMom3, crossed);

  //cout << "Momentum A: " << aMom3 << endl;
  //cout << "Outter Angular Momentum A: " << crossed << endl;

  r[0] = bPos[0] - hitPt[0];
  r[1] = bPos[1] - hitPt[1];
  r[2] = bPos[2] - hitPt[2];


  bMom3[0] = bMomentum[0];
  bMom3[1] = bMomentum[1];
  bMom3[2] = bMomentum[2];
  sgVectorProductVec3(crossed, r, bMom3);

  sgAddVec3(totalAngMom3, crossed);

  //cout << "Momentum B: " << bMom3 << endl;
  //cout << "Outter Angular Momentum B: " << crossed << endl;

  //cout << "Total Outter AngMom: " << totalAngMom3 << endl;

  //Merging code

  retAngMomentum[0] = totalAngMom3[0];
  retAngMomentum[1] = totalAngMom3[1];
  retAngMomentum[2] = totalAngMom3[2];



  object1->getAngMomentum(tempVec);
  //cout << "Inner Angular Momentum A: " << tempVec << endl;

  sgAddVec4(retAngMomentum, tempVec);

  object2->getAngMomentum(tempVec);
  //cout << "Inner Angular Momentum B: " << tempVec << endl;

  sgAddVec4(retAngMomentum, tempVec);

}

void mergeObjects(MyShape * object1, MyShape * object2) {
  sgVec3 color;

  object1->getColor( color );
  // cout << "\n\nCOLLISION:\n";
  // cout << "Object 1 Color r:" << color[0] <<
  // "\tg:"    << color[1] <<
  // "\tb:"    << color[2] << endl;
  object2->getColor( color );
  // cout << "Object 2 Color r:" << color <<
  // cout << "Object 2 Color r:" << color[0] <<
  // "\tg:"    << color[1] <<
  // "\tb:"    << color[2] << endl;

  // cout << "Object 1 radius:" << object1->getRadius() << endl;
  // cout << "Object 2 radius:" << object1->getRadius() << endl;

  float newMass = object1->getMass() + object2->getMass();
  float density = object1->getDensity();

  float newRadius = calcMergedRadius(newMass, density);

  // object1->getRadius();
  cout << "Merged Object radius:" << object1->getRadius() << endl;

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

  sgVec4 * object2momentum = object2->getMomentum();

  sgVec4 object2AngMom;

  sgVec3 newColor;

  object2->getAngMomentum(object2AngMom);

  object1->setMass(newMass);
  object1->setRadius(newRadius);
  object1->adjustMomentum( (*object2momentum) );

  //object1->adjustAngMomentum(object2AngMom);
  object1->setAngMomentum(totalAngMom);

  object1->calcColor();
  // newColor[0]=1.0;
  // newColor[1]=1.0;
  // newColor[2]=1.0;
  // object1->setColor( newColor );

  object1->setPos(COM);

  object1->getColor( color );
  // cout << "Post Collision:";
  // cout << "Object 1 Color r:" << color[0] <<
  // "\tg:"    << color[1] <<
  // "\tb:"    << color[2];
  // cout << "Object 1 Color r:" << color[0] <<
  // "\tg:"    << color[1] <<
  // "\tb:"    << color[2];

  //TODO Do AngMomentum calculations

  //cout << MASS_SUN << endl;
  delete object2momentum;

}

// TODO JUST PASS IN THE RADIUS YOU IDIOT
// Why in the world would you go through all this trouble?
float getSplitBodyRadius(float volume, int numPieces ) {
  float pieceVol = volume/numPieces;
  float pieceRadius = pieceVol/(M_PI * 4.0/3.0);
  pieceRadius = pow(pieceRadius, 1.0/3.0);

  return pieceRadius;
}

//TODO make it defined within a certain area, not based on piece size
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

  sgVec4 translatedPos;
  // cout << "Initial startPos: " << startPos << endl;
  sgAddVec4( translatedPos, startPos, target );
  startPos[0] = translatedPos[0];
  startPos[1] = translatedPos[1];
  startPos[2] = translatedPos[2];
  // cout << "Translated startPos: " << startPos << endl;
  //startPos = startPos + target;

  // cout << "Largest Distance: " << largestDistance << endl;

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
  MyShape * curShape;

  for (unsigned int i = 0; i < MyShape::shapes.size(); i++) {
    curShape = MyShape::shapes(i);
    curShape->getVelocity(dragForce);
    sgScaleVec4(dragForce, dragConstant);
    sgScaleVec4(dragForce, dt);
    curShape->adjustMomentum(dragForce);
  }
}

/*


////////////////

*/
