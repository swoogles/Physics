//
// Created by bfrasure on 8/10/18.
//

#ifndef PHYSICS_COMPLICATEDFUNCTIONS_H
#define PHYSICS_COMPLICATEDFUNCTIONS_H

#include "PairCollection.h"
#include "ShapeList.h"
#include "ParticleList.h"

class ComplicatedFunctions {
public:
    static ParticleList nonCollidingObjects(ParticleList allShapes, PairCollection collidingObjects);

};


#endif //PHYSICS_COMPLICATEDFUNCTIONS_H
