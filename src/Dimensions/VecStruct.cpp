//
// Created by bfrasure on 15/09/18.
//

#include "VecStruct.h"

VecStruct::VecStruct()
: vec()
{}

unique_ptr <VecStruct> VecStruct::vecFromAtoB(VecStruct *a, VecStruct *b) {
    unique_ptr<VecStruct> sepVec = make_unique<VecStruct>();
    sgSubVec4( sepVec->vec, a->vec, b->vec );
    return sepVec;
}

VecStruct::VecStruct(sgVec4 coordinates) {
    sgCopyVec4(vec, coordinates);
}

VecStruct::VecStruct(float x, float y, float z)
    :vec{x, y, z} {

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

