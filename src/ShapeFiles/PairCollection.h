#ifndef PHYSICS_PAIRCOLLECTION_H
#define PHYSICS_PAIRCOLLECTION_H

#include "TouchingPair.h"

using std::vector;

class PairCollection {
public:
    void insertIfUnique(const TouchingPair & newPair);
    size_t size() const;

private:
    std::vector<TouchingPair> pairs;

};

#endif
