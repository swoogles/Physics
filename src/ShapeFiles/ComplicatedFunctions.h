//
// Created by bfrasure on 8/10/18.
//

#ifndef PHYSICS_COMPLICATEDFUNCTIONS_H
#define PHYSICS_COMPLICATEDFUNCTIONS_H

#include "PairCollection.h"
#include "ShapeList.h"

class ComplicatedFunctions {
public:
    static ShapeList nonCollidingObjects(ShapeList allShapes, PairCollection collidingObjects);

};


#endif //PHYSICS_COMPLICATEDFUNCTIONS_H
