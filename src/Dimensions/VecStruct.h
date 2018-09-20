#ifndef PHYSICS_VECSTRUCT_H
#define PHYSICS_VECSTRUCT_H

#include <plib/sg.h>

class VecStruct {

public:
    sgVec4 vec;

    static VecStruct * vecFromAtoB(VecStruct * a, VecStruct * b);

public:
    inline float x() {return vec[0];}
    inline float y() {return vec[1];}
    inline float z() {return vec[2];}
};

#endif
