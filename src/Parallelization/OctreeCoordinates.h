#ifndef PHYSICS_OCTREECOORDINATES_H
#define PHYSICS_OCTREECOORDINATES_H

#include <vector>
#include <Dimensions/PhysicalVector.h>

using std::vector;

class OctreeCoordinates {
public:
    OctreeCoordinates(bool x, bool y, bool z);
    PhysicalVector polarities() const;
    vector<int> ints() const;
private:
    const vector<bool> coordinates;

};


#endif
