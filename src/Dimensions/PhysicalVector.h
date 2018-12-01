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

class PhysicalVector {

public:
    sgVec4 vec;

    PhysicalVector();
    PhysicalVector(sgVec4 coordinates);
    PhysicalVector(const sgVec4 coordinates);
    PhysicalVector(float x, float y, float z);
    PhysicalVector(float x, float y, float z, bool anchored);
    static unique_ptr <PhysicalVector> vecFromAtoB(PhysicalVector &a, PhysicalVector &b);

    inline float x() const {return vec[0];}
    inline float y() const {return vec[1];}
    inline float z() const {return vec[2];}

    PhysicalVector scaledBy(double scalingFactor);
    PhysicalVector withElementsMultipliedBy(const PhysicalVector &other);
    PhysicalVector plus(const PhysicalVector & other);
    PhysicalVector minus(const PhysicalVector & other);
    PhysicalVector unit();
    float scalarProduct4(const PhysicalVector & other);
    PhysicalVector vectorProduct3(const PhysicalVector & other);
    float length();
    string toString();
    bool hasValues(float xIn, float yIn, float zIn); // TODO Lookout! This is currently only used for testing!
    std::list<PhysicalVector> cancellingVectors(int numberOfVectors);
    bool operator==(const PhysicalVector &other) const;
    friend ostream& operator<<(ostream& os, const PhysicalVector& vec);
};

#endif
