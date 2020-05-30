#ifndef PHYSICS_PAIRCOLLECTION_H
#define PHYSICS_PAIRCOLLECTION_H

#include "TouchingPair.h"
#include <ShapeFiles/ParticleList.h>

#include <algorithm>
#include <optional>
#include <ostream>


using std::vector;
using std::optional;
using std::for_each;


// TODO Handle chained collisions
class PairCollection {
public:
    PairCollection();
    void insertIfUnique(const TouchingPair & newPair);

    ParticleList doomed();

    friend ostream &operator<<(ostream &os, const PairCollection &collection);

private:
    bool containsPair(const TouchingPair & newPair);
    inline vector<TouchingPair> brittlePairs() { return pairs; }
    vector<TouchingPair> pairs;

};

#endif
