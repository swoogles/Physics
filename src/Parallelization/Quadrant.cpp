#include "Quadrant.h"

using namespace std;

Quadrant * Quadrant::getQuadrantFromCell( int x, int y, int z )
{
  return quadOctree->at( x, y, z );
}

void Quadrant::subdivide( int x, int y, int z, int numCells )
{
  sgVec4 newPos;
  sgVec3 newDimensions;

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

void Quadrant::subdivideAll( int levels, int numCells )
{
  sgVec4 newPos;
  sgVec3 newDimensions;

  int xFactor;
  int yFactor;
  int zFactor;

  // Quadrant
  //TODO Calculate new position
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

          newPos[0]=pos[0]+(xFactor*dimensions[0]/4);
          newPos[1]=pos[1]+(yFactor*dimensions[1]/4);
          newPos[2]=pos[2]+(zFactor*dimensions[2]/4);

          newDimensions[0] = dimensions[0]/2;
          newDimensions[1] = dimensions[1]/2;
          newDimensions[2] = dimensions[2]/2;

          //targentQuadrant 
          //quadOctree->subdivide(i,j,k,4);
          quadOctree->set( x, y, z, new Quadrant( numCells, this->level + 1, newPos, newDimensions ) );
          targetQuadrant = quadOctree->at(x,y,z);
          targetQuadrant->subdivideAll(levels, 4);

        }
      }
    }

  }
}

Quadrant::Quadrant(int numCells, int level, sgVec4 pos, sgVec3 dimensions)
{
  this->level = level;
  int length = 2;
  quadOctree = new Octree<Quadrant * >(numCells);
  intOctree = new Octree<int>(numCells);

  sgVec3 offset;

  sgCopyVec3( this->pos, pos );
  sgCopyVec3( this->dimensions, dimensions );
  // this->dimensions[0] = dimensions[0];
  // this->dimensions[1] = dimensions[1];
  // this->dimensions[2] = dimensions[2];

  thisShape = new Box();
  thisShape->setPos(pos);

  int numShapes = MyShape::shapes.size();
  thisShape->setSideLength( dimensions[0] );

  MyShape::shapes.resize(numShapes + 1);
	MyShape::shapes(numShapes) = thisShape;

  // for ( int i = 0; i < length; i ++ )
  // {
  //   for ( int j = 0; j < length; j ++ )
  //   {
  //     for ( int k = 0; k < length; k ++ )
  //     {

  //     }
  //   }
  // }
}
