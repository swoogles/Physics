#ifndef PHYSICS_VECSTRUCT_H
#define PHYSICS_VECSTRUCT_H

#include <plib/sg.h>
#include <memory>
#include <iostream>

using std::unique_ptr;
using std::make_unique;

class VecStruct {

public:
    sgVec4 vec;

    VecStruct();
    VecStruct(sgVec4 coordinates);
    VecStruct(float x, float y, float z);
    VecStruct(float x, float y, float z, bool anchored);
    static unique_ptr <VecStruct> vecFromAtoB(VecStruct &a, VecStruct &b);

    inline float x() {return vec[0];}
    inline float y() {return vec[1];}
    inline float z() {return vec[2];}

    VecStruct scaledBy(float scalingFactor);
    VecStruct withElementsMultipliedBy(const VecStruct &other);
    VecStruct plus(const VecStruct & other);
    VecStruct minus(const VecStruct & other);
};

#endif
