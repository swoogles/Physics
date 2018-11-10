//
// Created by bfrasure on 11/9/18.
//

#ifndef PHYSICS_OCTREECOORDINATES_H
#define PHYSICS_OCTREECOORDINATES_H


#include <vector>
#include <Dimensions/VecStruct.h>

using std::vector;

class OctreeCoordinates {
    vector<bool> coordinates;
public:
    OctreeCoordinates(bool x, bool y, bool z);
    VecStruct polarities();
    vector<int> ints();

};


#endif //PHYSICS_OCTREECOORDINATES_H
