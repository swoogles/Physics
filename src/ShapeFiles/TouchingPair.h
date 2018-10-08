#ifndef PHYSICS_TOUCHINGPAIR_H
#define PHYSICS_TOUCHINGPAIR_H

#include "MyShape.h"

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
    shared_ptr<MyShape> a;
    shared_ptr<MyShape> b;

public:
    bool sameItems(const TouchingPair & touchingPair) const;
    TouchingPair(const shared_ptr<MyShape> aIn, const shared_ptr<MyShape> bIn);
    inline shared_ptr<MyShape> getA() { return a; };
    inline shared_ptr<MyShape> getB() { return b; };
};


#endif
