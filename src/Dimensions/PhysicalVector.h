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
    explicit PhysicalVector(sgVec4 coordinates);
    explicit PhysicalVector(const sgVec4 coordinates);

    PhysicalVector(float x, float y, float z);
    PhysicalVector(float x, float y, float z, bool anchored);
    static unique_ptr <PhysicalVector> vecFromAtoB(PhysicalVector &a, PhysicalVector &b);

    inline float x() const {return vec[0];}
    inline float y() const {return vec[1];}
    inline float z() const {return vec[2];}

    PhysicalVector scaledBy(double scalingFactor) const;
    PhysicalVector withElementsMultipliedBy(const PhysicalVector &other) const;
    PhysicalVector plus(const PhysicalVector & other) const;
    PhysicalVector minus(const PhysicalVector & other) const;
    PhysicalVector unit() const;
    float scalarProduct4(const PhysicalVector & other) const;
    PhysicalVector vectorProduct3(const PhysicalVector & other) const;
    float length() const;
    string toString() const;
    bool hasValues(float xIn, float yIn, float zIn) const; // TODO Lookout! This is currently only used for testing!
    std::list<PhysicalVector> cancellingVectors(int numberOfVectors);
    bool operator==(const PhysicalVector &other) const;
    friend ostream& operator<<(ostream& os, const PhysicalVector& vec);
};

#endif
