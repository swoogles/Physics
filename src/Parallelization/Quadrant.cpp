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
        const PhysicalVector particlePosition,
        shared_ptr<Particle> particlePtrParam)
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
        , childQuadrants(extents[2][2][2])
        , weightedPosition(weightedPosition)
        , particlePosition(particlePosition)
        , particleWeightedPosition(weightedPosition)
        , particleWeight(mass)
        , particlePtr(particlePtrParam){}


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
        meter_t radiusParameter,
        PhysicalVector weightedPositionParameter,
        kilogram_t massParameter,
        const PhysicalVector particlePositionParameter,
        shared_ptr<Particle> particlePtrParam) {
    if (!positionIsInQuadrantBoundaries(particlePositionParameter)) {
//        std::throw_with_nested(std::runtime_error(__func__));
// Just ignore for now, instead of throwing.
return;
    }

//    cout << "weightedPositionParameter: " << weightedPositionParameter << endl;
//    cout << "particlePositionParameter: " << particlePositionParameter << endl;
    if ( isLeaf ) {
        isLeaf = false;
        this->createSubQuadrantThatContains(
                this->particleRadius,
                this->particleWeightedPosition,
                this->particleWeight,
                this->particlePosition,
                this->particlePtr);
        // Clear this node's particle pointer since it's now internal
        this->particlePtr = nullptr;
    }
    this->createSubQuadrantThatContains(
            radiusParameter,
            weightedPositionParameter,
            massParameter,
            particlePositionParameter,
            particlePtrParam);

    this->adjustMass(massParameter);
    this->weightedPosition = this->weightedPosition.plus(weightedPositionParameter);
}

void Quadrant::createSubQuadrantThatContains(
        meter_t radius,
        PhysicalVector
        weightedPositionParameter,
        kilogram_t mass,
        PhysicalVector particlePositionParameter,
        shared_ptr<Particle> particlePtrParam) {

    function<void (Quadrant &)> functor =
            [this, radius, weightedPositionParameter, mass, particlePositionParameter, particlePtrParam](Quadrant & insertionQuadrant) {
                insertionQuadrant.insert(radius, weightedPositionParameter, mass, particlePositionParameter, particlePtrParam);
            };
    function<unique_ptr<Quadrant> ()> quadrantCreator =
            [this, radius, weightedPositionParameter, mass, particlePositionParameter, particlePtrParam]() {
                return std::move(this->makeSubQuadrant(radius, weightedPositionParameter, mass, particlePositionParameter, particlePtrParam));
            };
    // TODO We're *always* executing quadrantCreator, rather than stopping at a certain point.
    auto targetIndices = this->coordinatesForSubQuadrantContaining(particlePositionParameter);
    this->applyToQuadrantIfExistsOrElse(
            targetIndices.ints()[0],
            targetIndices.ints()[1],
            targetIndices.ints()[2],
            functor,
            quadrantCreator);

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
unique_ptr<Quadrant>  Quadrant::makeSubQuadrant(
        meter_t radius,
        PhysicalVector weightedPositionParameter,
        kilogram_t mass,
        PhysicalVector particlePositionParameter,
        shared_ptr<Particle> particlePtrParam) const {
    auto targetIndices = this->coordinatesForSubQuadrantContaining(particlePositionParameter);
    PhysicalVector newPos =
            pos.plus(
                    dimensions
                            .scaledBy(.25)
                            .withElementsMultipliedBy(targetIndices.polarities())
                    );

    return std::move(
            make_unique<Quadrant>(
                    this->level + 1,
                    newPos,
                    this->getWidth() / 2.0,
                    radius,
                    weightedPositionParameter,
                    mass,
                    particlePositionParameter,
                    particlePtrParam ) );
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

void Quadrant::resetForRebuild(meter_t radius, PhysicalVector weightedPositionParam,
                                kilogram_t mass, const PhysicalVector particlePositionParam,
                                shared_ptr<Particle> particlePtrParam) {
    // Reset state to initial leaf state
    isLeaf = true;
    containsBody = true;
    particleRadius = radius;
    particleWeight = mass;
    _mass = mass;
    weightedPosition = weightedPositionParam;
    particleWeightedPosition = weightedPositionParam;
    particlePosition = particlePositionParam;
    particlePtr = particlePtrParam;

    // Clear all children - they will be recreated as needed
    for (int x = 0; x < 2; x++) {
        for (int y = 0; y < 2; y++) {
            for (int z = 0; z < 2; z++) {
                childQuadrants[x][y][z].reset();
            }
        }
    }
}

void Quadrant::reinitialize(int levelParam, PhysicalVector& posParam, float width,
                             meter_t radius, PhysicalVector weightedPositionParam,
                             kilogram_t mass, const PhysicalVector particlePositionParam,
                             shared_ptr<Particle> particlePtrParam) {
    // Reinitialize all fields as if newly constructed
    pos = posParam;
    level = levelParam;
    dimensions = PhysicalVector(width, width, width);

    // Reset color based on level (same as constructor)
    _color = PhysicalVector(level * .10f, 1 - level * .10f, 1 - level * .10f);

    isLeaf = true;
    containsBody = true;
    particleRadius = radius;
    particleWeight = mass;
    _mass = mass;
    weightedPosition = weightedPositionParam;
    particleWeightedPosition = weightedPositionParam;
    particlePosition = particlePositionParam;
    particlePtr = particlePtrParam;

    // Clear children references (but don't deallocate - pool manages that)
    for (int x = 0; x < 2; x++) {
        for (int y = 0; y < 2; y++) {
            for (int z = 0; z < 2; z++) {
                childQuadrants[x][y][z] = nullptr;
            }
        }
    }
}

void Quadrant::applyToAllChildren(function<void (Quadrant &)> functor, function<bool (Quadrant &)> terminalPredicate) {
    if (terminalPredicate(*this)) {
        functor(*this); // Location of this call determines traversal strategy.
    } else {
        for (int x = 0; x < 2; x++) {
            for (int y = 0; y < 2; y++) {
                for (int z = 0; z < 2; z++) {
                    this->applyToQuadrantIfExists(x, y, z, functor, terminalPredicate);
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
                applyToSingleQuadrantIfExists( x, y, z,
                [this, functor](Quadrant & insertionQuadrant) {
                    insertionQuadrant.applyToAllChildrenConstant(functor);
                });
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

shared_ptr<Particle> Quadrant::getParticlePtr() const {
    return particlePtr;
}

void Quadrant::applyToQuadrantIfExists(
        int x,
        int y,
        int z,
        function<void (Quadrant &)> functor,
        function<bool (Quadrant &)> terminalPredicate
) {
    if (this->childQuadrants[x][y][z] != nullptr) {
        this->childQuadrants[x][y][z]->applyToAllChildren(functor, terminalPredicate);
    }
}

void Quadrant::applyToQuadrantIfExistsOrElse(
        int x,
        int y,
        int z,
        function<void (Quadrant &)> functor,
        function<unique_ptr<Quadrant> ()> quadrantCreator
) {
    if (this->childQuadrants[x][y][z] == nullptr) {
        this->childQuadrants[x][y][z] = std::move(quadrantCreator());
    } else {
        functor(*this->childQuadrants[x][y][z]);
    }
}

void Quadrant::applyToSingleQuadrantIfExists(int x, int y, int z, function<void(Quadrant &)> functor) const {
    if (this->childQuadrants[x][y][z] != nullptr) {
        functor(*this->childQuadrants[x][y][z]);
    }
}

