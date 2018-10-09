//
// Created by bfrasure on 6/10/18.
//

#include "TouchingPair.h"

bool TouchingPair::contains(shapePointer_t shape) const {
    return (this->a.lock() == shape || this->b.lock() == shape);
}

bool TouchingPair::sameItems(const TouchingPair & other)  const {
    if (
            (other.a.lock() == this->a.lock() && other.b.lock() == this->b.lock())
            || (other.b.lock() == this->a.lock() && other.a.lock() == this->b.lock())
            ) {
        return true;
    } else return false;
}

TouchingPair::TouchingPair(const shared_ptr<MyShape> aIn, const shared_ptr<MyShape> bIn)
        :a(aIn)
        ,b(bIn)
{ }
