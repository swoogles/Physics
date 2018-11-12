//
// Created by bfrasure on 15/09/18.
//

#include "VecStruct.h"

VecStruct::VecStruct()
: vec()
{}

unique_ptr <VecStruct> VecStruct::vecFromAtoB(VecStruct &a, VecStruct &b) {
    unique_ptr<VecStruct> sepVec = make_unique<VecStruct>();
    sgSubVec4( sepVec->vec, a.vec, b.vec );
    return sepVec;
}

VecStruct::VecStruct(sgVec4 coordinates) {
    sgCopyVec4(vec, coordinates);
}

VecStruct::VecStruct(float x, float y, float z)
    :vec{x, y, z} {

}

VecStruct::VecStruct(float x, float y, float z, bool anchored)
    :vec{x, y, z, anchored ? 1.0f : 0.0f} {
}


VecStruct VecStruct::scaledBy(float scalingFactor) {
    // TODO Make sure Vec3 behavior is appropriate here.
    sgVec4 newVec;
    sgScaleVec3 ( newVec, this->vec, scalingFactor );
    VecStruct newVecStruct(newVec);
    return newVecStruct;
}

VecStruct VecStruct::plus(const VecStruct & other) {
    sgVec4 newVec;
    sgAddVec4 ( newVec, this->vec, other.vec );
    VecStruct newVecStruct(newVec);
    return newVecStruct;
}

VecStruct VecStruct::minus(const VecStruct &other) {
    sgVec4 newVec;
    sgSubVec4 ( newVec, this->vec, other.vec );
    VecStruct newVecStruct(newVec);
    return newVecStruct;
}

VecStruct VecStruct::withElementsMultipliedBy(const VecStruct &other) {
    VecStruct retVec(
            this->vec[0] * other.vec[0],
            this->vec[1] * other.vec[1],
            this->vec[2] * other.vec[2]
            );
    return retVec;
}

VecStruct VecStruct::unit() {
    sgVec4 newVec;
    sgNormaliseVec4(newVec, vec);
    return newVec;
}

float VecStruct::scalarProduct4(const VecStruct & other) {
    return sgScalarProductVec4(this->vec, other.vec);
}

VecStruct::VecStruct(const float *coordinates) {
    sgCopyVec4(vec, coordinates);
}

VecStruct VecStruct::vectorProduct3(const VecStruct &other) {
    VecStruct retVec;
    sgVectorProductVec3(retVec.vec, this->vec, other.vec);
    return retVec;
}

float VecStruct::length() {
    SGfloat distanceSquared = sgLengthSquaredVec4(this->vec);
    return sqrt(distanceSquared);
}

string VecStruct::toString() {
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

bool VecStruct::hasValues(float xIn, float yIn, float zIn) {
    return  x() == xIn && y() == yIn && z() == zIn;
}

