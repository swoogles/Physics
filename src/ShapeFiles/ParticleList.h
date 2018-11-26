#ifndef PARTICLE_LIST_H
#define PARTICLE_LIST_H

#include "Particle.h"
#include <list>
#include <algorithm>

using std::size_t;

typedef particle_t item_t;

typedef typename std::vector<item_t> particleVector;

class ParticleList {
  private:
    particleVector shapes;
    void ensureNoNullEntries(string caller);

  public:
    ParticleList();
    ParticleList(item_t initialShape);
    ParticleList(particleVector shapesIn);
    bool hasConflictsWith(Particle &insertShape);
    size_t addShapeToList(item_t insertShape);
    size_t addList(ParticleList addList);

    int removeShapeFromList(item_t shapeToRemove);
    int remove(ParticleList & shapesToRemove);
    size_t clearShapes();
    particleVector getShapes();
    void update(const float dt);
    inline size_t size() { return shapes.size(); };

    bool contains(item_t searchShape) const;
};
#endif
