/*
 * Quadrant.h
*  Author: brasure
 */

#ifndef QUADRANT_H_
#define QUADRANT_H_

#include "octree.h"
#include <iostream>
#include <string>

//using namespace std;

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

#endif /* QUADRANT_H_ */

