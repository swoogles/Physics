#include "Quadrant.h"

bool withinBoundaries(PhysicalVector & insertPos, PhysicalVector & minBoundaries, PhysicalVector & maxBoundaries) {
  bool withinBoundaries = true;
  withinBoundaries &= (insertPos.x() > minBoundaries.x() && insertPos.x() < maxBoundaries.x());
  withinBoundaries &= (insertPos.y() > minBoundaries.y() && insertPos.y() < maxBoundaries.y());
  withinBoundaries &= (insertPos.z() > minBoundaries.z() && insertPos.z() < maxBoundaries.z());
  return withinBoundaries;
}


Quadrant::Quadrant(
        int level,
        PhysicalVector &pos,
        float width,
        meter_t radius,
        PhysicalVector weightedPosition,
        kilogram_t mass,
        const PhysicalVector particlePosition)
        : Box(
                pos,
                width,
                PhysicalVector(
                        level * .10f,
                        1 - level * .10f,
                        1 - level * .10f),
                    mass)
        , isLeaf(true)
        , containsBody(true)
        , particleRadius(radius)
        , level(level)
        , dimensions(width, width, width)
        , quadOctree(extents[2][2][2])
        , weightedPosition(weightedPosition)
        , particlePosition(particlePosition)
        , particleWeightedPosition(weightedPosition)
        , particleWeight(mass){}

shared_ptr<Quadrant>   Quadrant::getQuadrantFromCell( int x, int y, int z ) const {
  return this->quadOctree[x][y][z];
}


/* Description of insertion algorithm

  1. If node x does not contain a body, put the new body b here.

  2. a. If node x is an internal node, update the center-of-mass and total mass of x. 
     b. Recursively insert the body b in the appropriate quadrant.

  3. a. If node x is an external node, say containing a body named c, then there are two bodies b and c in the same region. 
     b. Subdivide the region further by creating four children. Then, recursively insert both b and c into the appropriate quadrant(s). Since b and c may still end up in the same quadrant, there may be several subdivisions during a single insertion.      c. Finally, update the center-of-mass and total mass of x.

     TODO Regarding particle outside of the quadrant, I should either
     *  Mark the particle for deletion in some way
     *  Expand the Quadrant until it *does* include the particle
     *
*/

void Quadrant::insert(
        meter_t radius,
        PhysicalVector weightedPositionParameter,
        kilogram_t massParameter,
        const PhysicalVector particlePositionParameter) {
    if (!positionIsInQuadrantBoundaries(particlePositionParameter)) {
        std::throw_with_nested(std::runtime_error(__func__));
    }


    if ( isLeaf ) {
        isLeaf = false;
        this->createSubQuadrantThatContains(
                this->particleRadius,
                this->particleWeightedPosition,
                this->particleWeight,
                this->particlePosition);
    }
    this->createSubQuadrantThatContains(
            radius,
            weightedPositionParameter,
            massParameter,
            particlePositionParameter);

    this->adjustMass(massParameter);
    this->weightedPosition = this->weightedPosition.plus(weightedPositionParameter);
}

void Quadrant::createSubQuadrantThatContains(
        meter_t radius,
        PhysicalVector
        weightedPositionParameter,
        kilogram_t mass,
        PhysicalVector particlePositionParameter) {
    auto targetIndices = this->coordinatesForSubQuadrantContaining(particlePositionParameter);
    shared_ptr<Quadrant>  insertionQuadrant = this->subQuadrantAt(targetIndices);

    if ( insertionQuadrant == nullptr ) {
        this->assignSubQuadrantAt(
                targetIndices,
                this->makeSubQuadrant(radius, weightedPositionParameter, mass, particlePositionParameter));
    } else {
        insertionQuadrant->insert(radius, weightedPositionParameter, mass, particlePositionParameter);
    };

}

OctreeCoordinates Quadrant::coordinatesForSubQuadrantContaining(PhysicalVector pointInsideQuadrant) const {
    return OctreeCoordinates(
            pointInsideQuadrant.x() < pos.x(),
            pointInsideQuadrant.y() < pos.y(),
            pointInsideQuadrant.z() < pos.z()
    );
}


// Note/warning: This makes the assumption that newShape *belongs* in the subQuadrant that matches the coordinates.
// Could be dangerous.
shared_ptr<Quadrant>  Quadrant::makeSubQuadrant(
        meter_t radius,
        PhysicalVector weightedPositionParameter,
        kilogram_t mass,
        PhysicalVector particlePositionParameter) const {
    auto targetIndices = this->coordinatesForSubQuadrantContaining(particlePositionParameter);
    PhysicalVector newPos =
            pos.plus(
                    dimensions
                            .scaledBy(.25)
                            .withElementsMultipliedBy(targetIndices.polarities())
                    );

    return std::move(
            make_shared<Quadrant>(
                    this->level + 1,
                    newPos,
                    this->getWidth() / 2.0,
                    radius,
                    weightedPositionParameter,
                    mass,
                    particlePositionParameter ) );
}

bool Quadrant::positionIsInQuadrantBoundaries(PhysicalVector insertPos) const {
  PhysicalVector newDimensions = dimensions.scaledBy(.5);

  PhysicalVector maxBoundariesVec = pos.plus(newDimensions);
  PhysicalVector minBoundariesVec = pos.minus(newDimensions);

  return withinBoundaries( insertPos, minBoundariesVec, maxBoundariesVec );
}

void Quadrant::adjustMass(kilogram_t dMass) {
	_mass += dMass;
}

shared_ptr<Quadrant>  Quadrant::subQuadrantAt(OctreeCoordinates indices) const {
    auto ints = indices.ints();
    return quadOctree[ints[0]][ints[1]][ints[2]];
}

void Quadrant::assignSubQuadrantAt(OctreeCoordinates indices, shared_ptr<Quadrant>  newQuadrant) {
    auto ints = indices.ints();
    this->quadOctree[ints[0]][ints[1]][ints[2]] = std::move(newQuadrant);
}

void Quadrant::applyToAllChildren(function<void (Quadrant &)> functor, function<bool (Quadrant &)> terminalPredicate) {
    if (terminalPredicate(*this)) {
        functor(*this); // Location of this call determines traversal strategy.
    } else {
        for (int x = 0; x < 2; x++) {
            for (int y = 0; y < 2; y++) {
                for (int z = 0; z < 2; z++) {
                    shared_ptr<Quadrant> targetQuadrant = this->getQuadrantFromCell(x, y, z);
                    if (targetQuadrant != nullptr) {
                        targetQuadrant->applyToAllChildren(functor, terminalPredicate);
                    }
                }
            }
        }
    }

}
void Quadrant::applyToAllChildrenConstant(function<void (const Quadrant &)> functor) const {
    functor(*this); // Location of this call determines traversal strategy.

    for ( int x = 0; x < 2; x++ ) {
        for ( int y = 0; y < 2; y++ ) {
            for ( int z = 0; z < 2; z++ ) {
                shared_ptr<Quadrant>  targetQuadrant = this->getQuadrantFromCell( x, y, z );
                if ( targetQuadrant != nullptr ) {
                    targetQuadrant->applyToAllChildrenConstant(functor);
                }
            }
        }
    }

}

const meter_t &Quadrant::getParticleRadius() const {
    return particleRadius;
}

const PhysicalVector &Quadrant::getParticlePosition() const {
    return particlePosition;
}


