#ifndef PHYSICS_VECSTRUCT_H
#define PHYSICS_VECSTRUCT_H

#include <plib/sg.h>
#include <memory>
#include <iostream>
#include <string>
#include <list>

using std::unique_ptr;
using std::make_unique;
using std::string;
using std::ostream;

class VecStruct {

public:
    sgVec4 vec;

    VecStruct();
    VecStruct(sgVec4 coordinates);
    VecStruct(const sgVec4 coordinates);
    VecStruct(float x, float y, float z);
    VecStruct(float x, float y, float z, bool anchored);
    static unique_ptr <VecStruct> vecFromAtoB(VecStruct &a, VecStruct &b);

    inline float x() const {return vec[0];}
    inline float y() const {return vec[1];}
    inline float z() const {return vec[2];}

    VecStruct scaledBy(double scalingFactor);
    VecStruct withElementsMultipliedBy(const VecStruct &other);
    VecStruct plus(const VecStruct & other);
    VecStruct minus(const VecStruct & other);
    VecStruct unit();
    float scalarProduct4(const VecStruct & other);
    VecStruct vectorProduct3(const VecStruct & other);
    float length();
    string toString();
    bool hasValues(float xIn, float yIn, float zIn); // TODO Lookout! This is currently only used for testing!
    std::list<VecStruct> cancellingVectors(int numberOfVectors);
    bool operator==(const VecStruct &other) const;
    friend ostream& operator<<(ostream& os, const VecStruct& vec);
};

#endif
