#include "Quadrant.h"

using namespace std;

ostream& operator<<(ostream& os, sgVec4 outputVec) {
	cout << "<" << outputVec[0] << ", " << outputVec[1] << ", " << outputVec[2] << ">";
	return os;
}

//void Quadrant::calcForcesForAll()
//{
//}


Quadrant::Quadrant(int numCells, int level, sgVec4 pos, sgVec4 dimensions)
{
  this->level = level;

  setPos( pos );
  sgCopyVec4( this->dimensions, dimensions );

  borders = new Box();
  borders->setPos(pos);
  borders->setSideLength( dimensions[0] );
  MyShape::addShapeToList( borders );
}

Quadrant * Quadrant::getQuadrantFromCell( int x, int y, int z )
{
  return quadOctree->at( x, y, z );
}



void Quadrant::getCenterOfMass(sgVec4 centerOfMass)
{
	sgCopyVec4(centerOfMass, this->preCenterOfMass);
  sgScaleVec4( centerOfMass, 1.0/mass );
}

void Quadrant::setCenterOfMass( sgVec4 centerOfMass )
{
  sgCopyVec4(this->preCenterOfMass, centerOfMass);
  sgScaleVec4( this->preCenterOfMass, mass);
}

void Quadrant::insertShape( MyShape * insertedShape )
{
  if ( shapeInQuadrant == NULL && quadOctree == NULL )
  {
    cout << "executing step 1" << endl;
    shapeInQuadrant = insertedShape;
    cout << "executed step 1" << endl;
  }
  else if ( shapeInQuadrant == NULL && quadOctree != NULL )
  {
    cout << "executing step 2" << endl;
    // TODO Update centerOfMass
    Quadrant * targetQuadrant = this->determineShapeQuadrant( insertedShape );
    targetQuadrant->insertShape( insertedShape );
    cout << "executed step 2" << endl;
  }
  else
  {
    cout << "executing step 3" << endl;
    sgVec4 curPos;

    MyShape * prevShapeInQuadrant = shapeInQuadrant;
    shapeInQuadrant = NULL;

    Quadrant * targetQuadrant;

    targetQuadrant = this->determineShapeQuadrant( prevShapeInQuadrant );
    targetQuadrant->insertShape( prevShapeInQuadrant );
    if ( this == targetQuadrant )
      exit(1);

    targetQuadrant = this->determineShapeQuadrant( insertedShape );
    targetQuadrant->insertShape( insertedShape );

    if ( this == targetQuadrant )
      exit(1);


    cout << "executed step 3" << endl;
  }

}


// Guaranteed to hand back an instantiated Quadrant
Quadrant * Quadrant::determineShapeQuadrant( MyShape * shapeToInsert )
{
  cout << "Determining shape Quadrant" << endl;
  sgVec4 insertPos;
  shapeToInsert->getPos( insertPos ); 
  cout << "for pos: " << insertPos << endl;
  float insertX = insertPos[0];
  float insertY = insertPos[1];
  float insertZ = insertPos[2];

  sgVec4 newPos;
  sgVec4 newDimensions;
  sgCopyVec4( newDimensions, dimensions );
  sgScaleVec4 ( newDimensions, .5 );

  float xOffset = dimensions[0]/4;
  float minXBoundary = pos[0] - newDimensions[0];
  float centralXBoundary = pos[0];
  float maxXBoundary = pos[0] + newDimensions[0];

  float yOffset = dimensions[1]/4;
  float minYBoundary = pos[1] - newDimensions[1];
  float centralYBoundary = pos[1];
  float maxYBoundary = pos[1] + newDimensions[1];

  float zOffset = dimensions[2]/4;
  float minZBoundary = pos[2] - newDimensions[2];
  float centralZBoundary = pos[2];
  float maxZBoundary = pos[2] + newDimensions[2];

  int targetX = INVALID_OCTREE_INDEX;
  int targetY = INVALID_OCTREE_INDEX;
  int targetZ = INVALID_OCTREE_INDEX;

  cout << "minZ" << minZBoundary << endl;
  cout << "midZ" << centralZBoundary << endl;
  cout << "maxZ" << maxZBoundary << endl;


  // X coordinate checking
  if ( insertX > minXBoundary ) //Within min X boundary
  {
    //cout << "X Checking" << endl;
    if ( insertX < centralXBoundary ) //Less than central X boundary
    {
      targetX = 0; 
      newPos[0] = centralXBoundary - xOffset;
    }
    else if ( insertX < maxXBoundary ) //Less than max X boundary
    {
      targetX = 1;
      newPos[0] = centralXBoundary + xOffset;
    }

    // Y coordinate checking
    if ( insertY > minYBoundary ) //Within min Y boundary
    {
      //cout << "Y Checking" << endl;
      if ( insertY < centralYBoundary ) //Less than central Y boundary
      {
        targetY = 0; 
        newPos[1] = centralYBoundary - yOffset;
      }
      else if ( insertY < maxYBoundary ) //Less than max Y boundary
      {
        targetY = 1;
        newPos[1] = centralYBoundary + yOffset;
      }

      //cout << "insertZ: " << insertZ << endl;
      //cout << "minZBoundary: " << minZBoundary << endl;

      // Z coordinate checking
      if ( insertZ > minZBoundary ) //Within min Z boundary
      {
        //cout << "Z Checking" << endl;
        //cout << "centralZBoundary" << centralZBoundary << endl;
        //cout << "maxZBoundary" << maxZBoundary << endl;
        if ( insertZ < centralZBoundary ) //Less than central Z boundary
        {
          newPos[2] = centralZBoundary - zOffset;
          targetZ = 0; 
        }
        else if ( insertZ < maxZBoundary ) //Less than max Z boundary
        {
          targetZ = 1;
          newPos[2] = centralZBoundary + zOffset;
        }
      }
    }
  }
  newPos[3] = 1;
    cout << "Should insert shape in quadrant[" 
      << targetX << ","
      << targetY << ","
      << targetZ << "]" << endl;

  Quadrant * insertionQuadrant = NULL;

  // Only proceed if we have determined that the provided shape does actually
  // belong in this Quadrant
  if ( targetX != INVALID_OCTREE_INDEX &&
      targetY != INVALID_OCTREE_INDEX &&
      targetZ != INVALID_OCTREE_INDEX )
  {
    int numCells = 8;
    if ( quadOctree == NULL )
    {
      quadOctree = new Octree<Quadrant * >(numCells);
      // for ( int a = 0; a++; a<2 )
      // {
      //   for ( int b = 0; b++; b<2 )
      //   {
      //     for ( int c = 0; c++; c<2 )
      //     {
      //       insertionQuadrant = new Quadrant( numCells, this->level + 1, newPos, newDimensions );
      //       quadOctree->set( a, b, c, insertionQuadrant );

      //     }
      //   }
      // }
    }

    //cout << "QuadOctree: " << quadOctree  << endl;
    insertionQuadrant = getQuadrantFromCell( targetX, targetY, targetZ );

    if ( insertionQuadrant == NULL )
    {
      cout << "Creating a new quadrant for insertion" << endl;
      cout << "With dimensions: " << newDimensions << endl;
      cout << "@ pos: " << newPos << endl;

      insertionQuadrant = new Quadrant( numCells, this->level + 1, newPos, newDimensions );
      quadOctree->set( targetX, targetY, targetZ, insertionQuadrant );
    }
  }
  else
  {
    sgVec4 dumbPos;
    shapeToInsert->getPos(dumbPos) ;
    cout  << "BadPos: " << dumbPos << endl;
  cout << "Target indeces" 
  << targetX << ","
  << targetY << ","
  << targetZ << endl;
    cout << "Fuck!" << endl;

  }

  return insertionQuadrant;
}
