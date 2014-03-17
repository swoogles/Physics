/*
 * Quadrant.h
 *  Author: brasure
 */

#ifndef QUADRANT_H_
#define QUADRANT_H_

#include <iostream>
#include <string>
#include <plib/sg.h>

#include "../ShapeFiles/MyShape.h"
#include "../ShapeFiles/Box.h"
#include "../ShapeFiles/Circle.h"
#include "../ShapeFiles/ShapeList.h"

#include <boost/multi_array/multi_array_ref.hpp>
#include <boost/numeric/ublas/vector_sparse.hpp>
#include <boost/multi_array.hpp>
#include <boost/current_function.hpp>
#include <boost/ref.hpp>
#include <boost/foreach.hpp>
#include <cassert>

#define INVALID_OCTREE_INDEX -1

using boost::shared_ptr;
using boost::make_shared;
using boost::extents;
using boost::multi_array;

class Quadrant : public Box
{
  private:
    bool isLeaf;
    bool containsBody;
    int level;

    sgVec4 weightedPosition;

    boost::shared_ptr<Box> borders;
    boost::shared_ptr<Circle> centerOfMassRepresentation;

    typedef boost::shared_ptr<MyShape> shape_pointer;
    typedef boost::shared_ptr<Quadrant> QuadrantPointer_t;
    typedef multi_array< QuadrantPointer_t, 3> array_typeNew;
    array_typeNew  quadOctree;

  public:
    // Octree<Quadrant *> * quadOctree; /* Create 4096x4096x4096 octree containing doubles. */
    Quadrant(int numCells, int level, sgVec4 pos, sgVec4 dimensions );
    ~Quadrant();
    void subDivideAll( int levels, int numCells );
    QuadrantPointer_t getQuadrantFromCell( int x, int y, int z );

    shape_pointer shapeInQuadrant;
    boost::shared_ptr<MyShape> shapeInQuadrantNew;

    sgVec4 dimensions;

    void insertShape( shape_pointer newShape );

    inline float getWidth() { return dimensions[0]; }

    inline bool isExternal() { return isLeaf; }

    inline void getPos( sgVec4 retVec ) { getCenterOfMass( retVec ); }

    boost::shared_ptr<MyShape> getShapeInQuadrant();

    void getWeightedPosition(sgVec4 weightedPosition);
    void setWeightedPosition(sgVec4 weightedPosition);

    void getCenterOfMass(sgVec4 centerOfMass);
    void setCenterOfMass( sgVec4 centerOfMass );

    QuadrantPointer_t determineShapeQuadrant( shape_pointer shapeToInsert );

    ShapeList getShapesRecursive();

    QuadrantPointer_t operator() ( const sgVec3 targets );

};
#endif /* QUADRANT_H_ */
