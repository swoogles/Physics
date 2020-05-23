#ifndef QUADRANT_H_
#define QUADRANT_H_

#include "../ShapeFiles/Box.h"
#include "ShapeFiles/Particle.h"
#include "OctreeCoordinates.h"

#include <boost/multi_array/multi_array_ref.hpp>
#include <boost/multi_array.hpp>
#include <boost/ref.hpp>
#include <functional>

#include <../lib/units.h>

using boost::extents;
using boost::multi_array;

using std::make_shared;

class Quadrant : public Box
{
public:
    typedef shared_ptr<Quadrant> QuadrantPointer_t;
    Quadrant(shared_ptr<Particle> newShape, int level, PhysicalVector &pos, float width);

    void insert(shared_ptr<Particle> newShape);

    inline float getWidth() const { return dimensions.vec[0]; }

    inline bool isExternal() const { return isLeaf; }

    shared_ptr<Particle> getShapeInQuadrant() const ;

    QuadrantPointer_t getQuadrantFromCell( int x, int y, int z );

    vector<shared_ptr<Quadrant>> children();

    QuadrantPointer_t makeSubQuadrant(shared_ptr<Particle> newShape) const;
    void applyToAllChildren(function<void (Quadrant)> functor);

  private:
    typedef multi_array< QuadrantPointer_t, 3> array_typeNew;

    bool isLeaf;
    bool containsBody;
    const int level;

    shared_ptr<Particle> shapeInQuadrant;

    PhysicalVector weightedPosition;
    const PhysicalVector dimensions;

    array_typeNew  quadOctree;

    void createSubQuadrantThatContains(shared_ptr<Particle> newShape);

    //! Alters mass of object by dMass
    void adjustMass(kilogram_t dMass);

    bool positionIsInQuadrantBoundaries(PhysicalVector insertPos) const;

    QuadrantPointer_t subQuadrantAt(OctreeCoordinates indices) const;
    void assignSubQuadrantAt(OctreeCoordinates indices, QuadrantPointer_t newSubQuadrant);
    OctreeCoordinates coordinatesForSubQuadrantContaining(PhysicalVector pointInsideQuadrant) const;
};
typedef shared_ptr<Quadrant> QuadrantPointer_t;
#endif
