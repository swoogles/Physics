//
// Created by bfrasure on 15/09/18.
//

#include "VecStruct.h"

VecStruct::VecStruct()
: vec()
{}

shared_ptr<VecStruct> VecStruct::vecFromAtoB(VecStruct * a, VecStruct * b) {
    shared_ptr<VecStruct> sepVec = make_shared<VecStruct>();
    sgSubVec4( sepVec->vec, a->vec, b->vec );
    return sepVec;
}
