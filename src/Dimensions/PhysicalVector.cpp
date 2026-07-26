//
// Created by bfrasure on 15/09/18.
//

#include "PhysicalVector.h"

PhysicalVector::PhysicalVector()
: vec()
{}

unique_ptr <PhysicalVector> PhysicalVector::vecFromAtoB(PhysicalVector &a, PhysicalVector &b) {
    unique_ptr<PhysicalVector> sepVec = make_unique<PhysicalVector>();
    sgSubVec4( sepVec->vec, a.vec, b.vec );
    return sepVec;
}

PhysicalVector::PhysicalVector(sgVec4 coordinates) {
    sgCopyVec4(vec, coordinates);
}

PhysicalVector::PhysicalVector(float x, float y, float z)
    :vec{x, y, z} {

}

PhysicalVector::PhysicalVector(float x, float y, float z, bool anchored)
    :vec{x, y, z, anchored ? 1.0f : 0.0f} {
}


PhysicalVector PhysicalVector::scaledBy(double scalingFactor) const {
    // sgScaleVec3 only writes the first three components, so the anchor weight
    // has to be carried across explicitly. Leaving it uninitialized used to
    // leak stack garbage into every scaled vector.
    sgVec4 newVec;
    sgScaleVec3 ( newVec, this->vec, scalingFactor );
    newVec[3] = this->vec[3] * scalingFactor;
    PhysicalVector newVecStruct(newVec);
    return newVecStruct;
}

PhysicalVector PhysicalVector::plus(const PhysicalVector & other) const {
    sgVec4 newVec;
    sgAddVec4 ( newVec, this->vec, other.vec );
    PhysicalVector newVecStruct(newVec);
    return newVecStruct;
}

PhysicalVector PhysicalVector::minus(const PhysicalVector &other) const {
    sgVec4 newVec;
    sgSubVec4 ( newVec, this->vec, other.vec );
    PhysicalVector newVecStruct(newVec);
    return newVecStruct;
}

PhysicalVector PhysicalVector::withElementsMultipliedBy(const PhysicalVector &other) const {
    PhysicalVector retVec(
            this->vec[0] * other.vec[0],
            this->vec[1] * other.vec[1],
            this->vec[2] * other.vec[2]
            );
    return retVec;
}

/*! Direction only: the anchor weight is dropped so normalising a position
 *  (w = 1) gives the direction to it rather than a shortened, tilted vector.
 */
PhysicalVector PhysicalVector::unit() const {
    sgVec4 newVec = {0, 0, 0, 0};
    sgNormaliseVec3(newVec, vec);
    return PhysicalVector(newVec);
}

float PhysicalVector::scalarProduct4(const PhysicalVector & other) const {
    return sgScalarProductVec4(this->vec, other.vec);
}

PhysicalVector::PhysicalVector(const float *coordinates) {
    sgCopyVec4(vec, coordinates);
}

PhysicalVector PhysicalVector::vectorProduct3(const PhysicalVector &other) const {
    PhysicalVector retVec;
    sgVectorProductVec3(retVec.vec, this->vec, other.vec);
    return retVec;
}

/*! Spatial length. The anchor weight is deliberately excluded: an anchored
 *  vector (w = 1) is a point in space, and its length is how far from the
 *  origin it sits, not sqrt(x^2 + y^2 + z^2 + 1).
 */
float PhysicalVector::length() const {
    SGfloat distanceSquared = sgLengthSquaredVec3(this->vec);
    return sqrt(distanceSquared);
}

string PhysicalVector::toString() const {
    string rep;
    // TODO Get float->string conversion. Bleh.
    return rep
            .append("{")
            .append(std::to_string(x()))
            .append(", ")
            .append(std::to_string(y()))
            .append(", ")
            .append(std::to_string(z()))
            .append("}")
            ;
}

bool PhysicalVector::hasValues(float xIn, float yIn, float zIn) const {
    return  x() == xIn && y() == yIn && z() == zIn;
}

std::list<PhysicalVector> PhysicalVector::cancellingVectors(int numberOfVectors) {
    return std::list<PhysicalVector>();
}

bool PhysicalVector::operator==(const PhysicalVector &other) const {
    float epsilon = 0.0001;
    return fabs(this->x() -other.x()) < epsilon
           && fabs(this->y() -other.y()) < epsilon
           && fabs(this->z() -other.z()) < epsilon;
}

ostream &operator<<(ostream &os, const PhysicalVector &vec) {
    os << "{" << vec.x() << ", " << vec.y() << ", " << vec.z() << "}";
    return os;
}


