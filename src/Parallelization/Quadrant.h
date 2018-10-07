#ifndef QUADRANT_H_
#define QUADRANT_H_

#include <plib/sg.h>

#include "../ShapeFiles/MyShape.h"
#include "../ShapeFiles/Box.h"
#include "../ShapeFiles/Circle.h"
#include "../ShapeFiles/ShapeList.h"

#include <boost/multi_array/multi_array_ref.hpp>
#include <boost/multi_array.hpp>
#include <boost/ref.hpp>

#define INVALID_OCTREE_INDEX -1

using boost::extents;
using boost::multi_array;

using std::make_shared;

class Quadrant : public Box
{
  private:
    typedef shared_ptr<MyShape> shape_pointer;
    typedef shared_ptr<Quadrant> QuadrantPointer_t;
    typedef multi_array< QuadrantPointer_t, 3> array_typeNew;

    bool isLeaf;
    bool containsBody;
    int level;

    shape_pointer shapeInQuadrant;

    sgVec4 weightedPosition;
    sgVec4 dimensions;

    shared_ptr<Box> borders;
//    shared_ptr<Circle> centerOfMassRepresentation;

    array_typeNew  quadOctree;

    QuadrantPointer_t determineShapeQuadrant( shape_pointer shapeToInsert );
    ShapeList shapes;

  public:
    Quadrant(int level, sgVec4 pos, sgVec4 dimensions, ShapeList shapes );
    ~Quadrant();
    QuadrantPointer_t getQuadrantFromCell( int x, int y, int z );

    void insertShape( shape_pointer newShape );

    inline float getWidth() { return dimensions[0]; }

    inline bool isExternal() { return isLeaf; }

    shared_ptr<MyShape> getShapeInQuadrant();

    void getWeightedPosition(sgVec4 weightedPosition);
    void setWeightedPosition(sgVec4 weightedPosition);

    void getCenterOfMass(sgVec4 centerOfMass);
    void setCenterOfMass( sgVec4 centerOfMass );

};
typedef shared_ptr<Quadrant> QuadrantPointer_t;
#endif
