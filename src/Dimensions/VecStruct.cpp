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

VecStruct::VecStruct(float x, float y, float z)
    :vec{x, y, z} {

}
