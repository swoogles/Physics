#ifndef PHYSICS_PAIRCOLLECTION_H
#define PHYSICS_PAIRCOLLECTION_H

#include "TouchingPair.h"
#include "ShapeList.h"

using std::vector;

class PairCollection {
public:
    void insertIfUnique(const TouchingPair & newPair);
    size_t size() const;

private:
    std::vector<TouchingPair> pairs;
    std::vector<shared_ptr<Circle>> survivors();
    std::vector<shared_ptr<Circle>> doomed();

};

#endif
