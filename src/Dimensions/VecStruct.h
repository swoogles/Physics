#ifndef PHYSICS_VECSTRUCT_H
#define PHYSICS_VECSTRUCT_H

#include <plib/sg.h>

class VecStruct {

public:
    sgVec4 vec;

    static VecStruct * vecFromAtoB(VecStruct * a, VecStruct * b);
};

#endif
