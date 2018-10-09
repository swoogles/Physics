//
// Created by bfrasure on 6/10/18.
//

#include "TouchingPair.h"

bool TouchingPair::contains(shapePointer_t shape) const {
    return (this->a == shape || this->b == shape);
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
