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
    shared_ptr<Circle> a;
    shared_ptr<Circle> b;

public:
    bool sameItems(const TouchingPair & touchingPair) const;
    TouchingPair(const shared_ptr<Circle> aIn, const shared_ptr<Circle> bIn);
    inline shared_ptr<Circle> getA() { return a; };
    inline shared_ptr<Circle> getB() { return b; };
};


#endif
