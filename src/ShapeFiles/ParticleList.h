#ifndef PARTICLE_LIST_H
#define PARTICLE_LIST_H

#include "Particle.h"
#include <list>
#include <algorithm>
#include <../lib/units.h>
#include <ostream>
#include <functional>

using namespace units::time;

using std::size_t;

typedef particle_t item_t;

typedef typename std::vector<item_t> particleVector;

class ParticleList {
public:
    void applyToAllParticles(
            function<void (Particle &)> functor );
    void checkForAllParticles(
            function<void (const Particle &)> functor ) const;
    ParticleList();
    ParticleList(item_t initialShape);
    ParticleList(particleVector shapesIn);
    bool hasConflictsWith(Particle &insertShape);
    size_t addShapeToList(item_t insertShape);
    size_t addList(ParticleList addList);

    int removeShapeFromList(item_t shapeToRemove);
    int remove(ParticleList & shapesToRemove);
    size_t clearShapes();
    particleVector getShapes() const;
    void update(hour_t dt);
    inline size_t size() const { return shapes.size(); } ;

    bool contains(item_t searchShape) const;

    friend ostream &operator<<(ostream &os, const ParticleList &particleList);

private:
    particleVector shapes;
    void ensureNoNullEntries(string caller);

};
#endif
