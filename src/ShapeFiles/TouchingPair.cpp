//
// Created by bfrasure on 6/10/18.
//

#include "TouchingPair.h"

bool TouchingPair::sameItems(TouchingPair other) {
    if (
            (&other.a == &this->a && &other.b == &this->b)
            || (&other.b == &this->a && &other.a == &this->b)
            ) {
        return true;
    } else return false;
}
