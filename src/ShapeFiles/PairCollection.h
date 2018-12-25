#ifndef PHYSICS_PAIRCOLLECTION_H
#define PHYSICS_PAIRCOLLECTION_H

#include "TouchingPair.h"
#include <ShapeFiles/ParticleList.h>

#include <algorithm>
#include <optional>


using std::vector;
using std::optional;
using std::for_each;


// TODO Handle chained collisions
class PairCollection {
public:
    PairCollection();
    void insertIfUnique(const TouchingPair & newPair);
    void insertUniqueElements(PairCollection newPairs);
    size_t size() const;
    ParticleList doomed();
    void mergePairs();
//    bool any_of( std::function< bool(const TouchingPair &)>& const lambda );
    inline vector<TouchingPair> brittlePairs() { return pairs; }
    bool containsPair(const TouchingPair & newPair);

private:
    vector<TouchingPair> pairs;

};

#endif
