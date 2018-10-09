#ifndef PHYSICS_PAIRCOLLECTION_H
#define PHYSICS_PAIRCOLLECTION_H

#include "TouchingPair.h"
#include "ShapeList.h"
#include "WeakShapeList.h"

using std::vector;

class PairCollection {
public:
    PairCollection();
    void insertIfUnique(const TouchingPair & newPair);
    void insertUniqueElements(PairCollection newPairs);
    size_t size() const;
    WeakShapeList survivors();
    WeakShapeList doomed();
//    bool any_of( std::function< bool(const TouchingPair &)>& const lambda );
    inline std::vector<TouchingPair> brittlePairs() { return pairs; }

private:
    std::vector<TouchingPair> pairs;

};

#endif
