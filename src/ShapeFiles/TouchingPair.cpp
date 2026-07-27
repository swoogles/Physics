//
// Created by bfrasure on 6/10/18.
//

#include "TouchingPair.h"

#include <algorithm>

bool TouchingPair::sameItems(const TouchingPair & other)  const {
    return (other.a == this->a || other.b == this->b)
           || (other.b == this->a || other.a == this->b);
//    return (other.a == this->a && other.b == this->b)
//           || (other.b == this->a && other.a == this->b);
}

TouchingPair::TouchingPair(shared_ptr<Particle> aIn, shared_ptr<Particle> bIn)
        :a(aIn)
        ,b(bIn)
{ }

void TouchingPair::merge() {
    // Keep the heavier body as the survivor. Either way the pair ends up at the
    // same centre of mass, but this way the big object persists and absorbs the
    // small one, rather than the speck surviving and jumping across to it.
    if (b->mass() > a->mass()) {
        std::swap(a, b);
    }

    // Simpler when merge returns A's pointer directly.
    a->mergeWith(*b);
//    b = nullptr; // TODO Dunno if this is crazy or not...
}

ostream &operator<<(ostream &os, const TouchingPair &touchingPair) {
    os << "[a: " << touchingPair.a << " b: " << touchingPair.b << "]";
    return os;
}
