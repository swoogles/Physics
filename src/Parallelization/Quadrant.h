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
    Quadrant(int level, PhysicalVector &pos, float width, meter_t radius, PhysicalVector weightedPosition,
             kilogram_t mass, const PhysicalVector shapePosition);

    void insert(meter_t radius, PhysicalVector weightedPositionParameter, kilogram_t massParameter,
                const PhysicalVector shapePositionParameter);

    inline float getWidth() const { return dimensions.vec[0]; }

    inline bool isExternal() const { return isLeaf; }

    shared_ptr<Quadrant>  getQuadrantFromCell( int x, int y, int z );

    vector<shared_ptr<Quadrant>> children();

    shared_ptr<Quadrant>  makeSubQuadrant(meter_t radius, PhysicalVector weightedPositionParameter, kilogram_t mass,
                                      PhysicalVector shapePositionParameter) const;
    void applyToAllChildren(function<void (Quadrant &)> functor);

    bool positionIsInQuadrantBoundaries(PhysicalVector insertPos) const;

  private:
    meter_t shapeRadius;
    PhysicalVector shapeWeightPosition;
public:
    const meter_t &getShapeRadius() const;

private:
    bool isLeaf;
    bool containsBody;
    const int level;

    PhysicalVector weightedPosition;
    PhysicalVector shapePosition;
    kilogram_t shapeWeight;
public:
    const PhysicalVector &getShapePosition() const;

private:
    const PhysicalVector dimensions;

    multi_array<shared_ptr<Quadrant>,3>  quadOctree;

    void createSubQuadrantThatContains(meter_t radius, PhysicalVector weightedPositionParameter, kilogram_t mass,
                                       PhysicalVector shapePositionParameter);

    //! Alters mass of object by dMass
    void adjustMass(kilogram_t dMass);

    shared_ptr<Quadrant>  subQuadrantAt(OctreeCoordinates indices) const;
    void assignSubQuadrantAt(OctreeCoordinates indices, shared_ptr<Quadrant>  newSubQuadrant);
    OctreeCoordinates coordinatesForSubQuadrantContaining(PhysicalVector pointInsideQuadrant) const;
};
#endif
