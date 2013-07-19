#include "Quadrant.h"

using namespace std;

ostream& operator<<(ostream& os, sgVec4 outputVec) {
	cout << "<" << outputVec[0] << ", " << outputVec[1] << ", " << outputVec[2] << ">";
	return os;
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

void Quadrant::subDivide( int x, int y, int z, int numCells )
{
  sgVec4 newPos;
  sgVec4 newDimensions;

  newDimensions[0] = dimensions[0]/2;
  newDimensions[1] = dimensions[1]/2;
  newDimensions[2] = dimensions[2]/2;

  int xFactor;
  int yFactor;
  int zFactor;
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

  newPos[0]=pos[0]+(xFactor*newDimensions[0]);
  newPos[1]=pos[1]+(yFactor*newDimensions[1]);
  newPos[2]=pos[2]+(zFactor*newDimensions[2]);

  // Quadrant
  //TODO Calculate new position
  quadOctree->set( x, y, z, new Quadrant( numCells, this->level + 1, newPos, newDimensions ) );
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

Quadrant::Quadrant(int numCells, int level, sgVec4 pos, sgVec4 dimensions)
{
  this->level = level;
  int length = 2;
  quadOctree = new Octree<Quadrant * >(numCells);
  intOctree = new Octree<int>(numCells);

  sgCopyVec4( this->pos, pos );
  sgCopyVec4( this->dimensions, dimensions );

  borders = new Box();
  borders->setPos(pos);

  int numShapes = MyShape::shapes.size();
  borders->setSideLength( dimensions[0] );

  MyShape::shapes.resize(numShapes + 1);
	MyShape::shapes(numShapes) = borders;

}

void Quadrant::insertShape( MyShape * insertedShape )
{
  if ( shapeInQuadrant == NULL )
  {
    cout << "Empty quadrant, assigning shape" << endl;
    shapeInQuadrant = insertedShape;
  }
  else
  {
    cout << "Need to divide before inserting shape" << endl;
    int levels = 2;
    this->subDivideAll( levels, 4 );
  }

}
