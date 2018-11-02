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
    TouchingPair(shared_ptr<MyShape> aIn, shared_ptr<MyShape> bIn);

    bool sameItems(const TouchingPair & touchingPair) const;
    bool contains(MyShape &shape) const;
    bool contains(shared_ptr<MyShape> searchShape) const;
    inline shared_ptr<MyShape> getA() const { return a; };
    inline shared_ptr<MyShape> getB() const { return b; };
    shapePointer_t merge();
};


#endif
