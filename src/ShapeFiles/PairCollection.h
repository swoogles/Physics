#ifndef PHYSICS_PAIRCOLLECTION_H
#define PHYSICS_PAIRCOLLECTION_H

#include "TouchingPair.h"
#include "ShapeList.h"

using std::vector;

class PairCollection {
public:
    void insertIfUnique(const TouchingPair & newPair);
    size_t size() const;
    ShapeList survivors();
    ShapeList doomed();
//    bool any_of( std::function< bool(const TouchingPair &)>& const lambda );
    inline std::vector<TouchingPair> brittlePairs() { return pairs; }

private:
    std::vector<TouchingPair> pairs;

};

#endif
