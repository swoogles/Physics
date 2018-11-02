//
// Created by bfrasure on 6/10/18.
//

#include "TouchingPair.h"

bool TouchingPair::contains(MyShape &shape) const {
    MyShape & aRef = *a;
    MyShape & bRef = *b;
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

TouchingPair::TouchingPair(const shared_ptr<MyShape> aIn, const shared_ptr<MyShape> bIn)
        :a(aIn)
        ,b(bIn)
{ }

shapePointer_t TouchingPair::merge() {
    // Simpler when merge returns A's pointer directly.
    a->mergeWith(*b);
    return a;
}

bool TouchingPair::contains(shared_ptr<MyShape> search) const {
    return a == search || b == search;
}
