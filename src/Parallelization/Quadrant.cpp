#include "Quadrant.h"

using namespace std;

Quadrant * Quadrant::getQuadrantFromCell( int x, int y, int z )
{
  return quadOctree->at( x, y, z );
}

void Quadrant::subdivide( int x, int y, int z, int numCells )
{
  // Quadrant
  quadOctree->set( x, y, z, new Quadrant( numCells, this->level + 1 ) );
}

Quadrant::Quadrant(int numCells, int level)
{
  this->level = level;
  int length = 2;
  quadOctree = new Octree<Quadrant * >(numCells);
  intOctree = new Octree<int>(numCells);
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
