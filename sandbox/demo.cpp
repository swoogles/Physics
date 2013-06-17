#include "octree.h"
#include <iostream>
#include <string>

using namespace std;

class Quadrant
{
  // Octree<Quadrant> quadOctree(); /* Create 4096x4096x4096 octree containing doubles. */

  // string label;

  // Quadrant * nextQuadrant;

  public:
    int level;
    Octree<Quadrant *> * quadOctree; /* Create 4096x4096x4096 octree containing doubles. */
    Octree<int> * intOctree; /* Create 4096x4096x4096 octree containing doubles. */
    Quadrant(int numCells, int level);
    void subdivide( int x, int y, int z, int numCells );
    Quadrant * getQuadrantFromCell( int x, int y, int z );

};

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

#ifdef WIN32
#include <tchar.h>
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
  Octree<double> o(4096); /* Create 4096x4096x4096 octree containing doubles. */
  o(1,2,3) = 3.1416;      /* Put pi in (1,2,3). */
  o.erase(1,2,3);         /* Erase that node. */

  Quadrant mainQuadrant(4, 1);
  mainQuadrant.subdivide(1,1,1, 8);
  Quadrant * targetQuadrant = mainQuadrant.quadOctree->at(1,1,1);
  cout << "Level: " << targetQuadrant->level << endl;
  targetQuadrant->subdivide(4,4,4, 2);
  targetQuadrant = targetQuadrant->quadOctree->at(4,4,4);
  cout << "Level: " << targetQuadrant->level << endl;

  return 0;
}

