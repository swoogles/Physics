//
// Created by bfrasure on 6/10/18.
//

#include "TouchingPair.h"

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
    // Simpler when merge returns A's pointer directly.
    a->mergeWith(*b);
//    b = nullptr; // TODO Dunno if this is crazy or not...
}

ostream &operator<<(ostream &os, const TouchingPair &touchingPair) {
    os << "[a: " << touchingPair.a << " b: " << touchingPair.b << "]";
    return os;
}
