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
             kilogram_t mass, const PhysicalVector particlePosition);

    void insert(meter_t radius, PhysicalVector weightedPositionParameter, kilogram_t massParameter,
                const PhysicalVector particlePositionParameter);

    inline float getWidth() const { return dimensions.vec[0]; }

    inline bool isExternal() const { return isLeaf; }

    void applyToAllChildren(function<void (Quadrant &)> functor, function<bool (Quadrant &)> terminalPredicate);
    void applyToAllChildrenConstant(function<void (const Quadrant &)> functor) const;

    const meter_t &getParticleRadius() const;
    const PhysicalVector &getParticlePosition() const;
    bool positionIsInQuadrantBoundaries(PhysicalVector insertPos) const;


private:
    meter_t particleRadius;
    PhysicalVector particleWeightedPosition;
    bool isLeaf;
    bool containsBody;
    const int level;

    PhysicalVector weightedPosition;
    PhysicalVector particlePosition;
    kilogram_t particleWeight;
    const PhysicalVector dimensions;

    multi_array<shared_ptr<Quadrant>,3>  quadOctree;

    multi_array<unique_ptr<Quadrant>,3>  uniqueQuadrantChildren;

    void createSubQuadrantThatContains(meter_t radius, PhysicalVector weightedPositionParameter, kilogram_t mass,
                                       PhysicalVector particlePositionParameter);

    //! Alters mass of object by dMass
    void adjustMass(kilogram_t dMass);

    shared_ptr<Quadrant>  subQuadrantAt(OctreeCoordinates indices) const;
    void assignSubQuadrantAt(OctreeCoordinates indices, shared_ptr<Quadrant>  newSubQuadrant);
    OctreeCoordinates coordinatesForSubQuadrantContaining(PhysicalVector pointInsideQuadrant) const;

    shared_ptr<Quadrant> getQuadrantFromCell( int x, int y, int z ) const;
    void  applyToQuadrantIfExists(
            int x,
            int y,
            int z,
            function<void (Quadrant &)> functor,
            function<bool (Quadrant &)> terminalPredicate
    );


    shared_ptr<Quadrant>  makeSubQuadrant(
            meter_t radius,
            PhysicalVector weightedPositionParameter,
            kilogram_t mass,
            PhysicalVector particlePositionParameter)
            const;

};
#endif
