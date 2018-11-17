#ifndef QUADRANT_H_
#define QUADRANT_H_

#include <plib/sg.h>

#include "../ShapeFiles/MyShape.h"
#include "../ShapeFiles/Box.h"
#include "../ShapeFiles/Circle.h"
#include "../ShapeFiles/ShapeList.h"
#include "../ShapeFiles/PairCollection.h"
#include "OctreeCoordinates.h"

#include <boost/multi_array/multi_array_ref.hpp>
#include <boost/multi_array.hpp>
#include <boost/ref.hpp>
#include <memory>

#include <functional>

using boost::extents;
using boost::multi_array;

using std::make_shared;
using std::weak_ptr;

class Quadrant : public Box
{
  private:
    typedef shared_ptr<Quadrant> QuadrantPointer_t;
    typedef multi_array< QuadrantPointer_t, 3> array_typeNew;

    bool isLeaf;
    bool containsBody;
    int level;

    shapePointer_t shapeInQuadrant;

    VecStruct weightedPosition;
    VecStruct dimensions;

    shared_ptr<Box> borders;
//    shared_ptr<Circle> centerOfMassRepresentation;

    array_typeNew  quadOctree;

    QuadrantPointer_t subQuadrantThatContains(shapePointer_t newShape);

    //! Alters mass of object by dMass
    void adjustMass(float dMass);

    bool positionIsInQuadrantBoundaries(VecStruct insertPos);

    QuadrantPointer_t subQuadrantAt(OctreeCoordinates indices);
    void assignSubQuadrantAt(OctreeCoordinates indices, QuadrantPointer_t newSubQuadrant);
    OctreeCoordinates coordinatesForSubQuadrantContaining(VecStruct pointInsideQuadrant);
  public:
    Quadrant(shapePointer_t newShape, int level, VecStruct &pos, float width);

    void insertShape(shapePointer_t newShape);

    inline float getWidth() { return dimensions.vec[0]; }

    inline bool isExternal() { return isLeaf; }

    shapePointer_t getShapeInQuadrant();

    QuadrantPointer_t getQuadrantFromCell( int x, int y, int z );

    vector<shared_ptr<Quadrant>> children();
    unique_ptr<vector<shared_ptr<Quadrant>>> children(unique_ptr<vector<shared_ptr<Quadrant>>> inVector);

    QuadrantPointer_t makeSubQuadrant(shapePointer_t newShape);
    void applyToAllChildren(function<void (Quadrant)> functor);

    shared_ptr<Box> getBorders();
};
typedef shared_ptr<Quadrant> QuadrantPointer_t;
#endif
