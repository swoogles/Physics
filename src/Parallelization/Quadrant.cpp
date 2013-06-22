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
  // Quadrant
  //TODO Calculate new position
  quadOctree->set( x, y, z, new Quadrant( numCells, this->level + 1, pos, newDimensions ) );
}

Quadrant::Quadrant(int numCells, int level, sgVec4 pos, sgVec3 dimensions)
{
  this->level = level;
  int length = 2;
  quadOctree = new Octree<Quadrant * >(numCells);
  intOctree = new Octree<int>(numCells);

  sgCopyVec3( this->pos, pos );
  sgCopyVec3( this->dimensions, dimensions );
  // this->dimensions[0] = dimensions[0];
  // this->dimensions[1] = dimensions[1];
  // this->dimensions[2] = dimensions[2];

  thisShape = new Box();
  thisShape->setPos(pos);
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
