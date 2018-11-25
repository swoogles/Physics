#ifndef PHYSICS_TOUCHINGPAIR_H
#define PHYSICS_TOUCHINGPAIR_H

#include "MyShape.h"
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
    TouchingPair(shared_ptr<Circle> aIn, shared_ptr<Circle> bIn);

    bool sameItems(const TouchingPair & touchingPair) const;
    bool contains(Circle &shape) const;
    bool contains(shared_ptr<Circle> searchShape) const;
    inline shared_ptr<Circle> getA() const { return a; };
    inline shared_ptr<Circle> getB() const { return b; };
    shared_ptr<Circle> merge();
};


#endif
