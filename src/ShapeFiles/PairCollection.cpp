//
// Created by bfrasure on 7/10/18.
//

#include "PairCollection.h"

PairCollection::PairCollection() {
}

void PairCollection::insertIfUnique(const TouchingPair & newPair) {
    cout << "insertingUnique" << endl;
    auto pairFunc = [&newPair](const TouchingPair pair) { return newPair.sameItems(pair); };
    auto foundInDestination = std::any_of(pairs.begin(), pairs.end(), pairFunc);
    if (!foundInDestination) {
        pairs.push_back(newPair);
    }
}

void PairCollection::insertUniqueElements(PairCollection newPairs) {
    auto brittlePairs = newPairs.brittlePairs();
    std::for_each(brittlePairs.begin(), brittlePairs.end(), [this](const auto & pair) { this->insertIfUnique(pair); });
}

size_t PairCollection::size() const {
    return pairs.size();
}

ShapeList  PairCollection::survivors() {
    cout << "Getting number of survivors: " << pairs.size() << endl;
    // Yikes, this was what was killing me
    std::vector<shared_ptr<MyShape>> survivors(pairs.size());
    std::transform (pairs.begin(), pairs.end(), survivors.begin(), [](TouchingPair pair) { return pair.getA();});
    cout << "transformed without dying" << endl;
    ShapeList retSurvivors(survivors);
    cout << "ret number of survivors: " << retSurvivors.size() << endl;
    return retSurvivors;
}
ShapeList PairCollection::doomed() {
    std::vector<shared_ptr<MyShape>> doomed(pairs.size());
    std::transform (pairs.begin(), pairs.end(), doomed.begin(), [](TouchingPair pair) { return pair.getB();});
    return ShapeList (doomed);
}

//bool any_of( std::function< bool(const TouchingPair &)>& const lambda ) {
//    return std::any_of(pairs.begin(), pairs.end(), lambda);
//}
