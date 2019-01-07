#ifndef PHYSICS_TOUCHINGPAIR_H
#define PHYSICS_TOUCHINGPAIR_H

#include "Particle.h"

class TouchingPair {
public:
    TouchingPair(shared_ptr<Particle> aIn, shared_ptr<Particle> bIn);

    bool sameItems(const TouchingPair & touchingPair) const;
    bool contains(Particle &shape) const;
    bool contains(shared_ptr<Particle> searchShape) const;
    inline shared_ptr<Particle> getA() const { return a; };
    inline shared_ptr<Particle> getB() const { return b; };
    shared_ptr<Particle> merge();
private:
    shared_ptr<Particle> a;
    shared_ptr<Particle> b;

};

#endif
