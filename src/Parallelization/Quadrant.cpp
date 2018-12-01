#include "Quadrant.h"

bool withinBoundaries(VecStruct & insertPos, VecStruct & minBoundaries, VecStruct & maxBoundaries) {
  bool withinBoundaries = true;
  withinBoundaries &= (insertPos.x() > minBoundaries.x() && insertPos.x() < maxBoundaries.x());
  withinBoundaries &= (insertPos.y() > minBoundaries.y() && insertPos.y() < maxBoundaries.y());
  withinBoundaries &= (insertPos.z() > minBoundaries.z() && insertPos.z() < maxBoundaries.z());
  return withinBoundaries;
}

entity_t Quadrant::getShapeInQuadrant() {
  return shapeInQuadrant;
}


Quadrant::Quadrant(entity_t newShape, int level, VecStruct &pos, float width)
        :Box(pos, width, VecStruct(level*.10f, 1-level*.10f, 1-level*.10f))
        ,isLeaf(true)
        ,containsBody(true)
        ,shapeInQuadrant(std::move(newShape))
        ,level(level)
        ,dimensions(width, width, width)
        ,borders(make_shared<Box>(pos, width, (sgVec3){ (float) (level*.10), (float) (1-level*.10), (float) (1-level*.10)} ) )
        ,quadOctree(extents[2][2][2])
        ,weightedPosition(0,0,0)
{
  // TODO Get this in a more idiomatic form
  this->setMass(shapeInQuadrant->getMass());
  // TODO Can this be a more direct move?

  // TODO reinstate CoM calculations here
}

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

void Quadrant::insert(entity_t insertedShape) {
    if (!positionIsInQuadrantBoundaries(insertedShape->getPos())) { return; }

    this->adjustMass(insertedShape->getMass().value());
    this->weightedPosition = this->weightedPosition.plus(insertedShape->getWeightedPosition());

    this->subQuadrantThatContains(std::move(insertedShape));
    if ( isLeaf ) {
        isLeaf = false;
        this->subQuadrantThatContains(std::move(shapeInQuadrant));
    }
    // 3.d centerOfMassRepresentation->setPos( CoMPosition );
}

QuadrantPointer_t Quadrant::subQuadrantThatContains(entity_t newShape) {
    auto targetIndices = this->coordinatesForSubQuadrantContaining(newShape->getPos());
    QuadrantPointer_t insertionQuadrant = this->subQuadrantAt(targetIndices);

    if ( insertionQuadrant == nullptr ) {
        QuadrantPointer_t newSubQuadrant = this->makeSubQuadrant(std::move(newShape));
        this->assignSubQuadrantAt(targetIndices, newSubQuadrant);
        return newSubQuadrant;
    } else {
        insertionQuadrant->insert(std::move(newShape));
        return insertionQuadrant;
   };

}

OctreeCoordinates Quadrant::coordinatesForSubQuadrantContaining(VecStruct pointInsideQuadrant) {
    return OctreeCoordinates(
            pointInsideQuadrant.x() < pos.x(),
            pointInsideQuadrant.y() < pos.y(),
            pointInsideQuadrant.z() < pos.z()
    );
}


// Note/warning: This makes the assumption that newShape *belongs* in the subQuadrant that matches the coordinates.
// Could be dangerous.
QuadrantPointer_t Quadrant::makeSubQuadrant(entity_t newShape) {
    auto targetIndices = this->coordinatesForSubQuadrantContaining(newShape->getPos());
    VecStruct offsets =
            dimensions
                    .scaledBy(.25)
                    .withElementsMultipliedBy(targetIndices.polarities());
    VecStruct newPos = pos.plus( offsets );

    return std::move(std::make_shared<Quadrant>( newShape, this->level + 1, newPos, this->getWidth() / 2.0 ) );
}

bool Quadrant::positionIsInQuadrantBoundaries(VecStruct insertPos) {
  VecStruct newDimensions = dimensions.scaledBy(.5);

  VecStruct maxBoundariesVec = pos.plus(newDimensions);
  VecStruct minBoundariesVec = pos.minus(newDimensions);

  return withinBoundaries( insertPos, minBoundariesVec, maxBoundariesVec );
}

void Quadrant::adjustMass(float dMass) {
	mass += kilogram_t(dMass);
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

QuadrantPointer_t Quadrant::subQuadrantAt(OctreeCoordinates indices) {
    auto ints = indices.ints();
    return quadOctree[ints[0]][ints[1]][ints[2]];
}

void Quadrant::assignSubQuadrantAt(OctreeCoordinates indices, QuadrantPointer_t newQuadrant) {
    auto ints = indices.ints();
    this->quadOctree[ints[0]][ints[1]][ints[2]] = std::move(newQuadrant);
}

void Quadrant::applyToAllChildren(function<void (Quadrant)> functor) {
    functor(*this);

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

shared_ptr<Box> Quadrant::getBorders() {
    return borders;
}


