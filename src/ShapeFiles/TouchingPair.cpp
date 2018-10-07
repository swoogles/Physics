//
// Created by bfrasure on 6/10/18.
//

#include "TouchingPair.h"

bool TouchingPair::sameItems(const TouchingPair & other)  const {
    if (
            (other.a == this->a && other.b == this->b)
            || (other.b == this->a && other.a == this->b)
            ) {
        return true;
    } else return false;
}

TouchingPair::TouchingPair(const shared_ptr<Circle> aIn, const shared_ptr<Circle> bIn)
        :a(aIn)
        ,b(bIn)
{ }
