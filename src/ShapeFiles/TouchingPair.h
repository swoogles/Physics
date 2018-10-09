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
    weak_ptr<MyShape> a;
    weak_ptr<MyShape> b;

public:
    TouchingPair(shared_ptr<MyShape> aIn, shared_ptr<MyShape> bIn);

    bool sameItems(const TouchingPair & touchingPair) const;
    bool contains(shapePointer_t shape) const;
    inline weak_ptr<MyShape> getA() const { return a; };
    inline weak_ptr<MyShape> getB() const { return b; };
};


#endif
