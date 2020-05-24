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
    Quadrant(int level, PhysicalVector &pos, float width, meter_t radius, PhysicalVector weightedPosition,
             kilogram_t mass, const PhysicalVector shapePosition);

    void insert(meter_t radius, PhysicalVector weightedPositionParameter, kilogram_t massParameter,
                const PhysicalVector shapePositionParameter);

    inline float getWidth() const { return dimensions.vec[0]; }

    inline bool isExternal() const { return isLeaf; }

    QuadrantPointer_t getQuadrantFromCell( int x, int y, int z );

    vector<shared_ptr<Quadrant>> children();

    QuadrantPointer_t makeSubQuadrant(meter_t radius, PhysicalVector weightedPositionParameter, kilogram_t mass,
                                      PhysicalVector shapePositionParameter) const;
    void applyToAllChildren(function<void (Quadrant)> functor);

    bool positionIsInQuadrantBoundaries(PhysicalVector insertPos) const;

  private:
    typedef multi_array< QuadrantPointer_t, 3> array_typeNew;

    meter_t shapeRadius;
public:
    const meter_t &getShapeRadius() const;

private:
    bool isLeaf;
    bool containsBody;
    const int level;

    PhysicalVector weightedPosition;
    PhysicalVector shapePosition;
public:
    const PhysicalVector &getShapePosition() const;

public:
    const PhysicalVector &getWeightedPosition() const;

private:
    const PhysicalVector dimensions;

    array_typeNew  quadOctree;

    void createSubQuadrantThatContains(meter_t radius, PhysicalVector weightedPositionParameter, kilogram_t mass,
                                       const PhysicalVector shapePositionParameter);

    //! Alters mass of object by dMass
    void adjustMass(kilogram_t dMass);

    QuadrantPointer_t subQuadrantAt(OctreeCoordinates indices) const;
    void assignSubQuadrantAt(OctreeCoordinates indices, QuadrantPointer_t newSubQuadrant);
    OctreeCoordinates coordinatesForSubQuadrantContaining(PhysicalVector pointInsideQuadrant) const;
};
typedef shared_ptr<Quadrant> QuadrantPointer_t;
#endif
