#ifndef PHYSICS_TOUCHINGPAIR_H
#define PHYSICS_TOUCHINGPAIR_H

#include <ostream>
#include "Particle.h"

class TouchingPair {
public:
    TouchingPair(shared_ptr<Particle> aIn, shared_ptr<Particle> bIn);

    bool sameItems(const TouchingPair & touchingPair) const;

    inline shared_ptr<Particle> getA() const { return a; };
    inline shared_ptr<Particle> getB() const { return b; };
    void merge();

    friend ostream &operator<<(ostream &os, const TouchingPair &touchingPair);

private:
    shared_ptr<Particle> a;
    shared_ptr<Particle> b;

};

#endif
