//
// Created by bfrasure on 15/09/18.
//

#include "VecStruct.h"
VecStruct * VecStruct::vecFromAtoB(VecStruct * a, VecStruct * b) {
    VecStruct * sepVec = new VecStruct();
    sgSubVec4( sepVec->vec, a->vec, b->vec );
    return sepVec;

}
