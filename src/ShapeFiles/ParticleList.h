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

typedef typename std::vector<shared_ptr<Particle>> particleVector;

class ParticleList {
public:
    void applyToAllParticles(
            function<void (Particle &)> functor );
    void checkForAllParticles(
            function<void (const Particle &)> functor ) const;
    ParticleList();

    ParticleList(std::vector<shared_ptr<Particle>> shapesIn);
    bool hasConflictsWith(Particle &insertShape);
    size_t addShapeToList(shared_ptr<Particle> insertShape);
    size_t addList(ParticleList addList);

    int removeShapeFromList(shared_ptr<Particle> shapeToRemove);
    int remove(ParticleList & shapesToRemove);

    void update(hour_t dt);
    inline size_t size() const { return shapes.size(); } ;

    bool contains(shared_ptr<Particle> searchShape) const;

    friend ostream &operator<<(ostream &os, const ParticleList &particleList);

private:
    std::vector<shared_ptr<Particle>> shapes;
    void ensureNoNullEntries(string caller);

};
#endif
