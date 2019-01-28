//
// Created by bfrasure on 6/10/18.
//

#include "TouchingPair.h"

bool TouchingPair::contains(Particle &shape) const {
    Particle & aRef = *a;
    Particle & bRef = *b;
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

TouchingPair::TouchingPair(shared_ptr<Particle> aIn, shared_ptr<Particle> bIn)
        :a(aIn)
        ,b(bIn)
{ }

shared_ptr<Particle> TouchingPair::merge() {
    // Simpler when merge returns A's pointer directly.
    a->mergeWith(*b);
    return a;
}

bool TouchingPair::contains(shared_ptr<Particle> search) const {
    return a == search || b == search;
}

ostream &operator<<(ostream &os, const TouchingPair &touchingPair) {
    os << "[a: " << touchingPair.a << " b: " << touchingPair.b << "]";
    return os;
}
