#ifndef QUADRANT_H_
#define QUADRANT_H_

#include <plib/sg.h>

#include "../Dimensions/Moveable.h"
#include "../ShapeFiles/Box.h"
#include "ShapeFiles/Particle.h"
#include "../ShapeFiles/PairCollection.h"
#include "OctreeCoordinates.h"

#include <boost/multi_array/multi_array_ref.hpp>
#include <boost/multi_array.hpp>
#include <boost/ref.hpp>
#include <memory>

#include <functional>

#include "ShapeType.h"

#include <../lib/units.h>

using boost::extents;
using boost::multi_array;

using std::make_shared;
using std::weak_ptr;

typedef shared_ptr<Particle> entity_t;

class Quadrant : public Box
{
public:
    typedef shared_ptr<Quadrant> QuadrantPointer_t;
    Quadrant(entity_t newShape, int level, PhysicalVector &pos, float width);

    void insert(entity_t newShape);

    inline float getWidth() { return dimensions.vec[0]; }

    inline bool isExternal() { return isLeaf; }

    entity_t getShapeInQuadrant();

    QuadrantPointer_t getQuadrantFromCell( int x, int y, int z );

    vector<shared_ptr<Quadrant>> children();
    unique_ptr<vector<shared_ptr<Quadrant>>> children(unique_ptr<vector<shared_ptr<Quadrant>>> inVector);

    QuadrantPointer_t makeSubQuadrant(entity_t newShape);
    void applyToAllChildren(function<void (Quadrant)> functor);

  private:
    typedef multi_array< QuadrantPointer_t, 3> array_typeNew;

    bool isLeaf;
    bool containsBody;
    int level;

    entity_t shapeInQuadrant;

    PhysicalVector weightedPosition;
    PhysicalVector dimensions;

    array_typeNew  quadOctree;

    QuadrantPointer_t subQuadrantThatContains(entity_t newShape);

    //! Alters mass of object by dMass
    void adjustMass(float dMass);

    bool positionIsInQuadrantBoundaries(PhysicalVector insertPos);

    QuadrantPointer_t subQuadrantAt(OctreeCoordinates indices);
    void assignSubQuadrantAt(OctreeCoordinates indices, QuadrantPointer_t newSubQuadrant);
    OctreeCoordinates coordinatesForSubQuadrantContaining(PhysicalVector pointInsideQuadrant);
};
typedef shared_ptr<Quadrant> QuadrantPointer_t;
#endif
