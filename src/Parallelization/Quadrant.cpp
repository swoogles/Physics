#include "Quadrant.h"

using namespace std;

ostream& operator<<(ostream& os, sgVec4 outputVec) {
	cout << "<" << outputVec[0] << ", " << outputVec[1] << ", " << outputVec[2] << ">";
	return os;
}

Quadrant::Quadrant(int numCells, int level, sgVec4 pos, sgVec4 dimensions)
{
  this->level = level;
  int length = 2;
  //quadOctree = new Octree<Quadrant * >(numCells);

  setPos( pos );
  //sgCopyVec4( this->pos, pos );
  sgCopyVec4( this->dimensions, dimensions );

  borders = new Box();
  borders->setPos(pos);

  sgVec3 newColor;
  float redAmount = level*.2; 
  float greenAmount = (1-level*.2);
  newColor[0]=redAmount;
  newColor[1]=greenAmount;
  cout << "Quadrant color: " << newColor << endl;
  borders->setColor( newColor );

  int numShapes = MyShape::shapes.size();
  borders->setSideLength( dimensions[0] );

  MyShape::shapes.resize(numShapes + 1);
	MyShape::shapes(numShapes) = borders;

}

void Quadrant::printCorners()
{
  sgVec4 curCenter;
  sgVec4 curCorner;
  sgVec4 newDimensions;
  int xFactor;
  int yFactor;
  int zFactor;

    for ( int x = 0; x < 2; x++ )
    {
      for ( int y = 0; y < 2; y++ )
      {
        for ( int z = 0; z < 2; z++ )
        {
          if ( x ) {
            xFactor=1;
          }
          else {
            xFactor =-1;
          }
          if ( y ) {
            yFactor=1;
          }
          else {
            yFactor =-1;
          }
          if ( z ) {
            zFactor=1;
          }
          else {
            zFactor =-1;
          }

          curCenter[0]=pos[0]+(xFactor*dimensions[0]/2.0);
          curCenter[1]=pos[1]+(yFactor*dimensions[1]/2.0);
          curCenter[2]=pos[2]+(zFactor*dimensions[2]/2.0);
          curCenter[3]=1;

          cout << "curCenter: <" << curCenter[0] << ", " << curCenter[1]
            << ", " << curCenter[2] << "> " << endl;

          newDimensions[0] = dimensions[0]/2;
          newDimensions[1] = dimensions[1]/2;
          newDimensions[2] = dimensions[2]/2;

          cout << "newDimensions: <" << newDimensions[0] << ", " << newDimensions[1]
            << ", " << newDimensions[2] << "> " << endl;
        }
      }
    }
}


Quadrant * Quadrant::getQuadrantFromCell( int x, int y, int z )
{
  return quadOctree->at( x, y, z );
}

// x,y, & z must have values of 0|1
void Quadrant::subDivide( int x, int y, int z, int numCells )
{
  sgVec4 newPos;
  sgVec4 newDimensions;

  int xFactor;
  int yFactor;
  int zFactor;

  if ( quadOctree == NULL )
  {
    quadOctree = new Octree<Quadrant * >(numCells);
  }

  if ( x ) {
    xFactor=1;
  }
  else {
    xFactor =-1;
  }
  if ( y ) {
    yFactor=1;
  }
  else {
    yFactor =-1;
  }
  if ( z ) {
    zFactor=1;
  }
  else {
    zFactor =-1;
  }

  // cout << "xFactor: " << xFactor << endl;
  // cout << "yFactor: " << yFactor << endl;
  // cout << "zFactor: " << zFactor << endl;

  newDimensions[0] = dimensions[0]/2;
  newDimensions[1] = dimensions[1]/2;
  newDimensions[2] = dimensions[2]/2;


  // cout << "Ein? " << endl;
  cout << "original Pos: " << pos << endl;
  //sgVec4 off
  newPos[0]=pos[0]+(xFactor*dimensions[0]/4.0);
  newPos[1]=pos[1]+(yFactor*dimensions[1]/4.0);
  newPos[2]=pos[2]+(zFactor*dimensions[2]/4.0);
  //newPos[3]=0;


  // Quadrant
  //TODO Calculate new position
  // cout << "Ein? " << endl;
  quadOctree->set( x, y, z, new Quadrant( numCells, this->level + 1, newPos, newDimensions ) );
  cout << "New Pos: " << newPos << endl;
  cout << "New Dimensions: " << newDimensions << endl;
  // cout << "Ein? " << endl;
}

void Quadrant::subDivideAll( int levels, int numCells )
{
  sgVec4 newPos;
  sgVec4 newDimensions;

  int xFactor;
  int yFactor;
  int zFactor;

  // Quadrant
  Quadrant * targetQuadrant;

  if ( quadOctree == NULL )
  {
    quadOctree = new Octree<Quadrant * >(numCells);
  }

  if ( this->level < levels )
  {

    for ( int x = 0; x < 2; x++ )
    {
      for ( int y = 0; y < 2; y++ )
      {
        for ( int z = 0; z < 2; z++ )
        {
          if ( x ) {
            xFactor=1;
          }
          else {
            xFactor =-1;
          }
          if ( y ) {
            yFactor=1;
          }
          else {
            yFactor =-1;
          }
          if ( z ) {
            zFactor=1;
          }
          else {
            zFactor =-1;
          }

          //sgVec4 offset;
          //sgScaleVec4( pos
          newPos[0]=pos[0]+(xFactor*dimensions[0]/4.0);
          newPos[1]=pos[1]+(yFactor*dimensions[1]/4.0);
          newPos[2]=pos[2]+(zFactor*dimensions[2]/4.0);
          newPos[3]=1;

          newDimensions[0] = dimensions[0]/2;
          newDimensions[1] = dimensions[1]/2;
          newDimensions[2] = dimensions[2]/2;

          quadOctree->set( x, y, z, new Quadrant( numCells, this->level + 1, newPos, newDimensions ) );
          targetQuadrant = quadOctree->at(x,y,z);
          targetQuadrant->subDivideAll(levels, 4);

        }
      }
    }

  }
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
  this->adjustMass( insertedShape->getMass() );
  //cout << "Quadrant: " << this << endl;
  //cout << "Mass: " << this->getMass() << endl;

  if ( shapeInQuadrant == NULL && quadOctree == NULL )
  {
    //cout << "executing step 1" << endl;
    shapeInQuadrant = insertedShape;
    //cout << "executed step 1" << endl;
  }
  else if ( shapeInQuadrant == NULL && quadOctree != NULL )
  {
    //cout << "executing step 2" << endl;
    // TODO Update centerOfMass
    int levels = 2;
    Quadrant * targetQuadrant = this->determineShapeQuadrant( insertedShape );
    //cout << "targetQuadrantAddress: " << targetQuadrant << endl;
    //cout << "intermission" << endl;
    targetQuadrant->insertShape( insertedShape );
    //if ( targetQuadrant->shapeInQuadrant
    //this->subDivide( levels, 4 );
    //cout << "executed step 2" << endl;
  }
  else
  {
    //cout << "executing step 3" << endl;
    Quadrant * targetQuadrant = this->determineShapeQuadrant( insertedShape );
    targetQuadrant->insertShape( insertedShape );

    targetQuadrant = this->determineShapeQuadrant( shapeInQuadrant );
    targetQuadrant->insertShape( shapeInQuadrant );

    shapeInQuadrant = NULL;

    //cout << "executed step 3" << endl;
  }

}


// Guaranteed to hand back an instantiated Quadrant
Quadrant * Quadrant::determineShapeQuadrant( MyShape * shapeToInsert )
{
  //cout << "Determining shape Quadrant" << endl;
  sgVec4 insertPos;
  shapeToInsert->getPos( insertPos ); 
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

  Quadrant * insertionQuadrant;

  //cout << "Ok" << endl;

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
    }

    //cout << "QuadOctree: " << quadOctree  << endl;
    insertionQuadrant = getQuadrantFromCell( targetX, targetY, targetZ );
    //cout << "Should insert shape in quadrant[" 
      //<< targetX << ","
      //<< targetY << ","
      //<< targetZ << "]" << endl;

    if ( insertionQuadrant == NULL )
    {
      //cout << "Creating a new quadrant for insertion" << endl;
      //cout << endl;
      //subDivide( targetX, targetY, targetZ, numCells );

      insertionQuadrant = new Quadrant( numCells, this->level + 1, newPos, newDimensions );
      quadOctree->set( targetX, targetY, targetZ, insertionQuadrant );
    }
    else
    {
      //cout << "Position requires further dividing of an existant Quadrant" << endl;
      //cout << endl;
      //subDivide( targetX, targetY, targetZ, numCells );
    }
  }

  if ( insertionQuadrant == NULL )
  {
    //cout << "Oh fu..!" << endl;
  }
  return insertionQuadrant;
  
}
