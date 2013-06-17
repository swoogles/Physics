#include "octree.h"

class Quadrant
{
  // Octree<Quadrant> quadOctree(); /* Create 4096x4096x4096 octree containing doubles. */

  int level;

  public:
    Octree<Quadrant> * quadOctree; /* Create 4096x4096x4096 octree containing doubles. */
    Octree<int> * intOctree; /* Create 4096x4096x4096 octree containing doubles. */
    Quadrant(int);

};

Quadrant::Quadrant(int numCells)
{
  int length = 2;
  quadOctree = new Octree<Quadrant>(numCells);
  intOctree = new Octree<int>(numCells);
  for ( int i = 0; i < length; i ++ )
  {
    for ( int j = 0; j < length; j ++ )
    {
      for ( int k = 0; k < length; k ++ )
      {

      }
    }
  }
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

  Quadrant mainQuadrant(4);
  mainQuadrant.quadOctree->at(0,0,0);

  return 0;
}

