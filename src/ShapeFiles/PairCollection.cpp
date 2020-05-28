//
// Created by bfrasure on 7/10/18.
//

#include "PairCollection.h"

PairCollection::PairCollection() {
}

/*
*  [A, B]  => [A, B]
*  [B, C]     [A, C]
*  If the first item in the new pair is a doomed link,
*       make Pair(existingPair.1, newPair.2)
*       */

optional<TouchingPair> aCheck(const TouchingPair & newPair, const TouchingPair & existingPair) {
    auto newPairA = newPair.getA().get();
    auto pairB = existingPair.getB().get();

    return (newPairA == pairB)
           ? std::optional(TouchingPair(existingPair.getA(), newPair.getB()))
           : std::nullopt;
}

/*
[A, B]  => [A, B]
[C, B]     [A, C]
If the 2nd, doomed item is the link in the chain,
        make Pair(existingPair.1, newPair.1)
        */
optional<TouchingPair> bCheck(const TouchingPair & newPair, const TouchingPair & existingPair) {
    auto newPairB = newPair.getB().get();
    auto pairB = existingPair.getB().get();
    return (newPairB == pairB)
        ? std::optional(TouchingPair(existingPair.getA(), newPair.getA()))
        : std::nullopt;
}

/*
[A, B]  => [A, B]
[A, C]     [A, C]
If the first, surving item is the link in the chain,
        make Pair(newPair.1, newPair2) // Just use newPair as-is
        */
optional<TouchingPair> cCheck(const TouchingPair & newPair, const TouchingPair & existingPair) {
    auto newPairA = newPair.getA().get();
    auto pairA = existingPair.getA().get();

    return (newPairA == pairA)
           ? std::optional(newPair)
           : std::nullopt;
}

/*
[A, B]  => [A, B]
[C, A]     [A, C]

If the first item is the surviving, non-link in the chain,
        make Pair(newPair.2, newPair1) // Just use newPair as-is
        */
optional<TouchingPair> dCheck(const TouchingPair & newPair, const TouchingPair & existingPair) {
    auto newPairB = newPair.getB().get();
    auto pairA = existingPair.getA().get();

    return (newPairB == pairA)
           ? std::optional(TouchingPair(newPair.getB(), newPair.getA()))
           : std::nullopt;
}

void PairCollection::insertIfUnique(const TouchingPair & newPair) {
     // This scans the whole vector before the additional checks trigger a second sweep.
     // Might be necessary though.
    if (!containsPair(newPair)) {

        optional<TouchingPair> result = nullopt;
        for_each(
                pairs.begin(),
                pairs.end(),
                [&newPair,&result] (auto pair) {
                    if (!result.has_value()) {
                        auto aResult = aCheck(newPair, pair);
                        auto bResult = bCheck(newPair, pair);
                        auto cResult = cCheck(newPair, pair);
                        auto dResult = dCheck(newPair, pair);
                        if (aResult.has_value()) {
                            result = aResult;
                        } else if (bResult.has_value()) {
                            result = bResult;
                        } else if (cResult.has_value()) {
                            result = cResult;
                        } else if (dResult.has_value()) {
                            result = dResult;
                        } else { // Nothing special about this collision
                        }
                    }
                });
        pairs.push_back(result.value_or(newPair));

    } else {
        cout << "Not inserting pair because it's already in this colleciton." << endl;
    }
}

void PairCollection::insertUniqueElements(PairCollection newPairs) {
    auto brittlePairs = newPairs.brittlePairs();
    for_each(brittlePairs.begin(), brittlePairs.end(), [this](const auto & pair) { this->insertIfUnique(pair); });
}

size_t PairCollection::size() const {
    return pairs.size();
}

ParticleList PairCollection::doomed() {
    particleVector doomed(pairs.size());
    std::transform (pairs.begin(), pairs.end(), doomed.begin(), [](const TouchingPair& pair) { return pair.getB();});
    return ParticleList (doomed);
}

void PairCollection::mergePairs() {
    for_each(pairs.begin(), pairs.end(), [](TouchingPair pair) {
        pair.merge();
    });
}

bool PairCollection::containsPair(const TouchingPair & newPair) {
    auto pairFunc = [&newPair](const TouchingPair& pair) { return newPair.sameItems(pair); };
    return std::any_of(pairs.begin(), pairs.end(), pairFunc);
}

ostream &operator<<(ostream &os, const PairCollection &collection) {
    os << "[";
    for (auto i : collection.pairs)
        os  << i << ' ';
    os << "]";

    return os;
}
