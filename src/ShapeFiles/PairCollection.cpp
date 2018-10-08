//
// Created by bfrasure on 7/10/18.
//

#include "PairCollection.h"

void PairCollection::insertIfUnique(const TouchingPair & newPair) {
    auto pairFunc = [&newPair](const TouchingPair pair) { return newPair.sameItems(pair); };
    auto foundInDestination = std::any_of(pairs.begin(), pairs.end(), pairFunc);
    if (!foundInDestination) {
        cout << "Inserting" << endl;
        pairs.push_back(newPair);
    }
    cout << "Found: " << foundInDestination << endl;
}

size_t PairCollection::size() const {
    return pairs.size();
}
