#ifndef PHYSICS_VECSTRUCT_H
#define PHYSICS_VECSTRUCT_H

#include <plib/sg.h>
#include <memory>

using std::shared_ptr;
using std::make_shared;

class VecStruct {

public:
    sgVec4 vec;

    VecStruct();
    static shared_ptr<VecStruct> vecFromAtoB(VecStruct * a, VecStruct * b);

    inline float x() {return vec[0];}
    inline float y() {return vec[1];}
    inline float z() {return vec[2];}
};

#endif
