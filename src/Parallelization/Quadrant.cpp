#include "Quadrant.h"

bool withinBoundaries(PhysicalVector & insertPos, PhysicalVector & minBoundaries, PhysicalVector & maxBoundaries) {
  bool withinBoundaries = true;
  withinBoundaries &= (insertPos.x() > minBoundaries.x() && insertPos.x() < maxBoundaries.x());
  withinBoundaries &= (insertPos.y() > minBoundaries.y() && insertPos.y() < maxBoundaries.y());
  withinBoundaries &= (insertPos.z() > minBoundaries.z() && insertPos.z() < maxBoundaries.z());
  return withinBoundaries;
}


Quadrant::Quadrant(int level, PhysicalVector &pos, float width, meter_t radius, PhysicalVector weightedPosition,
                   kilogram_t mass, const PhysicalVector shapePosition)
        : Box(pos, width, PhysicalVector(level * .10f, 1 - level * .10f, 1 - level * .10f), mass)
        ,isLeaf(true)
        ,containsBody(true)
        ,shapeRadius(radius)
        ,level(level)
        ,dimensions(width, width, width)
        ,quadOctree(extents[2][2][2])
        ,weightedPosition(weightedPosition) // Properly using this will help avoid a shape reference.
        ,shapePosition(shapePosition) {}

QuadrantPointer_t  Quadrant::getQuadrantFromCell( int x, int y, int z ) {
  return this->quadOctree[x][y][z];
}


/* Description of insertion algorithm

  1. If node x does not contain a body, put the new body b here.

  2. a. If node x is an internal node, update the center-of-mass and total mass of x. 
     b. Recursively insert the body b in the appropriate quadrant.

  3. a. If node x is an external node, say containing a body named c, then there are two bodies b and c in the same region. 
     b. Subdivide the region further by creating four children. Then, recursively insert both b and c into the appropriate quadrant(s). Since b and c may still end up in the same quadrant, there may be several subdivisions during a single insertion.      c. Finally, update the center-of-mass and total mass of x.

     TODO Regarding shapes outside of the quadrant, I should either
     *  Mark the shape for deletion in some way
     *  Expand the Quadrant until it *does* include the shape
*/

void Quadrant::insert(meter_t radius, PhysicalVector weightedPositionParameter, kilogram_t massParameter,
                      const PhysicalVector shapePositionParameter) {
    if (!positionIsInQuadrantBoundaries(shapePositionParameter)) {
        std::throw_with_nested(std::runtime_error(__func__));
        return;
    }


    this->createSubQuadrantThatContains(radius, weightedPositionParameter,
                                        massParameter, shapePositionParameter);
    if ( isLeaf ) {
        isLeaf = false;
        this->createSubQuadrantThatContains(
                this->shapeRadius,
                this->weightedPosition,

                this->mass(),
                this->shapePosition);
    }

    this->adjustMass(massParameter);
    this->weightedPosition = this->weightedPosition.plus(weightedPositionParameter);
}

void Quadrant::createSubQuadrantThatContains(meter_t radius, PhysicalVector weightedPositionParameter, kilogram_t mass,
                                             const PhysicalVector shapePositionParameter) {

    auto targetIndices = this->coordinatesForSubQuadrantContaining(shapePositionParameter); // TODO This has been difficult to change :/
    QuadrantPointer_t insertionQuadrant = this->subQuadrantAt(targetIndices);

    if ( insertionQuadrant == nullptr ) {
        this->assignSubQuadrantAt(
                targetIndices,
                this->makeSubQuadrant(radius, weightedPositionParameter, mass, shapePositionParameter));
    } else {
        insertionQuadrant->insert(radius, weightedPositionParameter, mass, shapePositionParameter);
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
QuadrantPointer_t Quadrant::makeSubQuadrant(meter_t radius, PhysicalVector weightedPositionParameter, kilogram_t mass,
                                            PhysicalVector shapePositionParameter) const {
    auto targetIndices = this->coordinatesForSubQuadrantContaining(shapePositionParameter);
    PhysicalVector newPos =
            pos.plus(
                    dimensions
                            .scaledBy(.25)
                            .withElementsMultipliedBy(targetIndices.polarities())
                    );

    return std::move(make_shared<Quadrant>(this->level + 1, newPos, this->getWidth() / 2.0, radius, weightedPositionParameter, mass, shapePositionParameter ) );
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

vector<shared_ptr<Quadrant>> Quadrant::children() {
  vector<shared_ptr<Quadrant>>liveChildren ;
  for ( int x = 0; x < 2; x++ ) {
    for ( int y = 0; y < 2; y++ ) {
      for ( int z = 0; z < 2; z++ ) {
        QuadrantPointer_t targetQuadrant = this->getQuadrantFromCell( x, y, z );
        if ( targetQuadrant != nullptr ) {
          liveChildren.push_back(targetQuadrant);
          auto nextChildren = targetQuadrant->children();
          liveChildren.insert(std::end(liveChildren), std::begin(nextChildren), std::end(nextChildren));
        }
      }
    }
  }
  return std::move(liveChildren);
}

QuadrantPointer_t Quadrant::subQuadrantAt(OctreeCoordinates indices) const {
    auto ints = indices.ints();
    return quadOctree[ints[0]][ints[1]][ints[2]];
}

void Quadrant::assignSubQuadrantAt(OctreeCoordinates indices, QuadrantPointer_t newQuadrant) {
    auto ints = indices.ints();
    this->quadOctree[ints[0]][ints[1]][ints[2]] = std::move(newQuadrant);
}

void Quadrant::applyToAllChildren(function<void (Quadrant)> functor) {
    functor(*this); // Location of this call determines traversal strategy.

    for ( int x = 0; x < 2; x++ ) {
        for ( int y = 0; y < 2; y++ ) {
            for ( int z = 0; z < 2; z++ ) {
                QuadrantPointer_t targetQuadrant = this->getQuadrantFromCell( x, y, z );
                if ( targetQuadrant != nullptr ) {
                    targetQuadrant->applyToAllChildren(functor);
                }
            }
        }
    }

}

const meter_t &Quadrant::getShapeRadius() const {
    return shapeRadius;
}

const PhysicalVector &Quadrant::getWeightedPosition() const {
    return weightedPosition;
}

const PhysicalVector &Quadrant::getShapePosition() const {
    return shapePosition;
}


