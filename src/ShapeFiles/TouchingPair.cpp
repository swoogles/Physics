//
// Created by bfrasure on 6/10/18.
//

#include "TouchingPair.h"

bool TouchingPair::contains(Circle &shape) const {
    Circle & aRef = *a;
    Circle & bRef = *b;
    return (&aRef == &shape || &bRef == &shape);
}

bool TouchingPair::sameItems(const TouchingPair & other)  const {
    if (
            (other.a == this->a && other.b == this->b)
            || (other.b == this->a && other.a == this->b)
            ) {
        return true;
    } else return false;
}

TouchingPair::TouchingPair(shared_ptr<Circle> aIn, shared_ptr<Circle> bIn)
        :a(aIn)
        ,b(bIn)
{ }

shared_ptr<Circle> TouchingPair::merge() {
    // Simpler when merge returns A's pointer directly.
    a->mergeWith(*b);
    return a;
}

bool TouchingPair::contains(shared_ptr<Circle> search) const {
    return a == search || b == search;
}
