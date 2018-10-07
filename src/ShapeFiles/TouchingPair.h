#ifndef PHYSICS_TOUCHINGPAIR_H
#define PHYSICS_TOUCHINGPAIR_H

#include "Circle.h"

/*
List(
 (1, 2),
 (3, 4),
 (2, 1),
 (4, 3)
 )
        )
        */

class TouchingPair {
private:
    Circle a;
    Circle b;

public:
    bool sameItems(TouchingPair touchingPair);
};


#endif
