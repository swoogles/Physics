#ifndef SHAPE_LIST_H
#define SHAPE_LIST_H

#include "Circle.h"
#include <list>
#include <algorithm>

using std::size_t;

typedef typename std::vector<shared_ptr<Circle>> particleVector;

class ParticleList {
  private:
    particleVector shapes;
    void ensureNoNullEntries(string caller);

  public:
    ParticleList();
    ParticleList(shared_ptr<Circle> initialShape);
    ParticleList(particleVector shapesIn);
    bool hasConflictsWith(Circle &insertShape);
    size_t addShapeToList(shared_ptr<Circle> insertShape);
    size_t addList(ParticleList addList);

    int removeShapeFromList(shared_ptr<Circle> shapeToRemove);
    int remove(ParticleList & shapesToRemove);
    size_t clearShapes();
    particleVector getShapes();
    void update(const float dt);
    inline size_t size() { return shapes.size(); };

    bool contains(shared_ptr<Circle> searchShape) const;
};
#endif
