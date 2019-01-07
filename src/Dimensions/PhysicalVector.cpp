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
    // TODO Make sure Vec3 behavior is appropriate here.
    sgVec4 newVec;
    sgScaleVec3 ( newVec, this->vec, scalingFactor );
    PhysicalVector newVecStruct(newVec);
    return newVecStruct;
}

PhysicalVector PhysicalVector::plus(const PhysicalVector & other) {
    sgVec4 newVec;
    sgAddVec4 ( newVec, this->vec, other.vec );
    PhysicalVector newVecStruct(newVec);
    return newVecStruct;
}

PhysicalVector PhysicalVector::minus(const PhysicalVector &other) {
    sgVec4 newVec;
    sgSubVec4 ( newVec, this->vec, other.vec );
    PhysicalVector newVecStruct(newVec);
    return newVecStruct;
}

PhysicalVector PhysicalVector::withElementsMultipliedBy(const PhysicalVector &other) {
    PhysicalVector retVec(
            this->vec[0] * other.vec[0],
            this->vec[1] * other.vec[1],
            this->vec[2] * other.vec[2]
            );
    return retVec;
}

PhysicalVector PhysicalVector::unit() {
    sgVec4 newVec;
    sgNormaliseVec4(newVec, vec);
    return newVec;
}

float PhysicalVector::scalarProduct4(const PhysicalVector & other) {
    return sgScalarProductVec4(this->vec, other.vec);
}

PhysicalVector::PhysicalVector(const float *coordinates) {
    sgCopyVec4(vec, coordinates);
}

PhysicalVector PhysicalVector::vectorProduct3(const PhysicalVector &other) {
    PhysicalVector retVec;
    sgVectorProductVec3(retVec.vec, this->vec, other.vec);
    return retVec;
}

float PhysicalVector::length() {
    SGfloat distanceSquared = sgLengthSquaredVec4(this->vec);
    return sqrt(distanceSquared);
}

string PhysicalVector::toString() {
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

bool PhysicalVector::hasValues(float xIn, float yIn, float zIn) {
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


