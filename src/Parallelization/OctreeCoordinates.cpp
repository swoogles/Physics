//
// Created by bfrasure on 11/9/18.
//

#include "OctreeCoordinates.h"

OctreeCoordinates::OctreeCoordinates(bool x, bool y, bool z)
:coordinates{x, y, z} {

}

PhysicalVector OctreeCoordinates::polarities() {
    int xPolarity = (coordinates[0]) ? -1 : 1;
    int yPolarity = (coordinates[1]) ? -1 : 1;
    int zPolarity = (coordinates[2]) ? -1 : 1;
    return PhysicalVector(xPolarity, yPolarity, zPolarity);
}

vector<int> OctreeCoordinates::ints() {
    vector<int> intValus{
            (coordinates[0]) ? 0 : 1,
            (coordinates[1]) ? 0 : 1,
            (coordinates[2]) ? 0 : 1
    };
    return intValus;
}
