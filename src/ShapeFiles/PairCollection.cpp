//
// Created by bfrasure on 7/10/18.
//

#include "PairCollection.h"

PairCollection::PairCollection() {
}

void PairCollection::insertIfUnique(const TouchingPair & newPair) {
    if (!containsPair(newPair)) {

        /*
        *  [A, B]  => [A, B]
        *  [B, C]     [A, C]
        *  If the first item in the new pair is a doomed link,
        *       make Pair(existingPair.1, newPair.2)
        *       */

        auto singleFunc = [&newPair](const TouchingPair pair) {
            auto newPairA = newPair.getA().get();
            auto pairB = pair.getB().get();
            //Extra
            return newPairA == pairB;
        };
        vector<TouchingPair>::iterator foundAchainedItem = std::find_if(pairs.begin(), pairs.end(), singleFunc);


        /*
        [A, B]  => [A, B]
        [C, B]     [A, C]
        If the 2nd, doomed item is the link in the chain,
                make Pair(existingPair.1, newPair.1)
                */
        auto bFunc = [&newPair](const TouchingPair pair) {
            auto newPairB = newPair.getB().get();
            auto pairB = pair.getB().get();
            //Extra
            return newPairB == pairB;
        };
        vector<TouchingPair>::iterator foundAchainedItemB = std::find_if(pairs.begin(), pairs.end(), bFunc);
        /*
        [A, B]  => [A, B]
        [A, C]     [A, C]
        If the first, surving item is the link in the chain,
                make Pair(newPair.1, newPair2) // Just use newPair as-is
                */
        auto cFunc = [&newPair](const TouchingPair pair) {
            auto newPairA = newPair.getA().get();
            auto pairA = pair.getA().get();
            //Extra
            return newPairA == pairA;
        };

        /*
        [A, B]  => [A, B]
        [C, A]     [A, C]

        If the first, surving item is the surving, non-link in the chain,
                make Pair(newPair.2, newPair1) // Just use newPair as-is
                */
        vector<TouchingPair>::iterator caseC = std::find_if(pairs.begin(), pairs.end(), cFunc);
        auto dFunc = [&newPair](const TouchingPair pair) {
            auto newPairB = newPair.getB().get();
            auto pairA = pair.getA().get();
            //Extra
            return newPairB == pairA;
        };
        vector<TouchingPair>::iterator caseD = std::find_if(pairs.begin(), pairs.end(), dFunc);
        if (foundAchainedItem != pairs.end()) {
            cout << "Found first case !" << endl;
            auto chainedA = foundAchainedItem->getA();
            pairs.push_back(TouchingPair(chainedA, newPair.getB()));
        } else if (foundAchainedItemB != pairs.end()) {
            cout << "Found second case !" << endl;
            auto chainedA = foundAchainedItemB->getA();
            pairs.push_back(TouchingPair(chainedA, newPair.getA()));
        } else if (caseC != pairs.end()) {
            cout << "Found third case !" << endl;
            auto chainedA = caseC->getA();
            pairs.push_back(newPair);
        } else if (caseD != pairs.end()) {
            cout << "Found fourth case !" << endl;
            auto chainedA = caseD->getA();
            pairs.push_back(TouchingPair(newPair.getB(), newPair.getA()));
        } else {
            pairs.push_back(newPair);
        }
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
// Yikes, this was what was killing me
    vectorT survivors(pairs.size());
    std::transform (pairs.begin(), pairs.end(), survivors.begin(), [](TouchingPair pair) { return pair.getA();});
    ShapeList retSurvivors(survivors);
    return retSurvivors;
}
ShapeList PairCollection::doomed() {
    vectorT doomed(pairs.size());
    std::transform (pairs.begin(), pairs.end(), doomed.begin(), [](TouchingPair pair) { return pair.getB();});
    return ShapeList (doomed);
}

void PairCollection::mergePairs() {
    std::for_each(pairs.begin(), pairs.end(), [](TouchingPair pair) {
        pair.merge();
    });
}

bool PairCollection::containsPair(const TouchingPair & newPair) {
    auto pairFunc = [&newPair](const TouchingPair pair) { return newPair.sameItems(pair); };
    return std::any_of(pairs.begin(), pairs.end(), pairFunc);
}

//bool any_of( std::function< bool(const TouchingPair &)>& const lambda ) {
//    return std::any_of(pairs.begin(), pairs.end(), lambda);
//}
