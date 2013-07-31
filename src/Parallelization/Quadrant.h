/*
 * Quadrant.h
*  Author: brasure
 */

#ifndef QUADRANT_H_
#define QUADRANT_H_

#include "octree.h"
#include <iostream>
#include <string>
#include <plib/sg.h>

#include "../ShapeFiles/MyShape.h"
#include "../ShapeFiles/Box.h"

//using namespace std;

#define INVALID_OCTREE_INDEX -1

class Quadrant : public Box
{
    Box * borders;
    sgVec4 preCenterOfMass;

  public:
    int level;
    Octree<Quadrant *> * quadOctree; 
    Octree<int> * intOctree; 
    Quadrant(int numCells, int level, sgVec4 pos, sgVec4 dimensions );
    void subDivide( int x, int y, int z, int numCells );
    void subDivideAll( int levels, int numCells );
    Quadrant * getQuadrantFromCell( int x, int y, int z );

    void printCorners();

    MyShape * shapeInQuadrant;
    
    sgVec4 dimensions;

    //sgVec4 pos;

    void insertShape( MyShape * newShape );

    void getCenterOfMass(sgVec4 centerOfMass);
    void setCenterOfMass( sgVec4 centerOfMass );

    Quadrant * determineShapeQuadrant( MyShape * shapeToInsert );
};

#endif /* QUADRANT_H_ */

