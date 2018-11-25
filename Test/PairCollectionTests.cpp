//
// Created by bfrasure on 8/10/18.
//

#include "catch.hpp"

#include "../src/ShapeFiles/TouchingPair.h"
#include "../src/ShapeFiles/PairCollection.h"
#include "../src/ShapeFiles/Circle.h"
#include "../src/ShapeFiles/ComplicatedFunctions.h"
#include "TestUtils.h"
#include <plib/sg.h>

#include <memory>

TEST_CASE("normalize list of pairs", "[pairCollection]") {
    shared_ptr<Circle> a = TestUtils::testCircle();
    cout << "a: " << &(*a) << endl;
    shared_ptr<Circle> b = TestUtils::testCircle();
    cout << "b: " << &(*b) << endl;
    auto c = TestUtils::testCircle();
    cout << "c: " << &(*c) << endl;
    auto d = TestUtils::testCircle();
    cout << "d: " << &(*d) << endl;

    const TouchingPair ab(a, b);
    const TouchingPair ac(a, c);
    const TouchingPair ba(b, a);
    const TouchingPair bc(b, c);
    const TouchingPair ca(c, a);
    const TouchingPair cb(c, b);

    vectorT originalCircles = {a, b, c, d};

    SECTION("Merged chained items") {

        /*
         *  [A, B]  => [A, B]
         *  [B, C]     [A, C]
         *  If the first item in the new pair is a doomed link,
         *       make Pair(existingPair.1, newPair.2)
         */
        SECTION("Handle first case") {
            PairCollection pairs;
            pairs.insertIfUnique(ab);
            pairs.insertIfUnique(bc);
            REQUIRE(pairs.size() == 2);
            REQUIRE(pairs.containsPair(TouchingPair(a, c)));

        }

        /*
        [A, B]  => [A, B]
        [C, B]     [A, C]
        If the 2nd, doomed item is the link in the chain,
                make Pair(existingPair.1, newPair.1)
                */
        SECTION("Handle second case") {
            PairCollection pairs;
            pairs.insertIfUnique(ab);
            pairs.insertIfUnique(cb);
            REQUIRE(pairs.size() == 2);
            REQUIRE(pairs.containsPair(TouchingPair(a, c)));

        }

        /*
        [A, B]  => [A, B]
        [A, C]     [A, C]
        If the first, surving item is the link in the chain,
                make Pair(newPair.1, newPair2) // Just use newPair as-is
                */
        SECTION("Handle c case") {
            PairCollection pairs;
            pairs.insertIfUnique(ab);
            pairs.insertIfUnique(ac);
            REQUIRE(pairs.size() == 2);
            REQUIRE(pairs.containsPair(TouchingPair(a, c)));

        }

        /*
        [A, B]  => [A, B]
        [C, A]     [A, C]

        If the first, surving item is the surving, non-link in the chain,
                make Pair(newPair.2, newPair1) // Just use newPair as-is
                */
        SECTION("Handle d case") {
            PairCollection pairs;
            pairs.insertIfUnique(ab);
            pairs.insertIfUnique(ca);
            REQUIRE(pairs.size() == 2);
            REQUIRE(pairs.containsPair(TouchingPair(a, c)));

        }

    }
/*
 * Original:
 * (a, b, c, d)
 *
 * Colliding Pairs:
 * (a, b)
 *
 * Remaining Items:
 * (a, c, d)
 *
 * Deleted Items:
 * (b)
 */

    std::vector<TouchingPair> originalPairs{
            ab,
            ba
    };

    /*
    SECTION("existence checking section") {
        PairCollection uniquePairs;
        for (auto const &curPair : originalPairs) {
            cout << "inserting" << endl;
            uniquePairs.insertIfUnique(curPair);
        }

        cout << "a uses inner: " << a.use_count() << endl;

        SECTION("provide correct survivors and doomed entries") {
            auto survivors = uniquePairs.survivors();
            REQUIRE(survivors.size() == 1);
            REQUIRE(survivors.contains(a));
            auto doomed = uniquePairs.doomed();
            REQUIRE(doomed.size() == 1);
//            REQUIRE(doomed.contains(*b));
        }

        SECTION("manage the non-colliding objects") {
            ShapeList originalShapes(originalCircles);
            auto nonColliders = ComplicatedFunctions::nonCollidingObjects(originalShapes, uniquePairs);
            // call new function here
            REQUIRE(nonColliders.size() == 2);
            REQUIRE(nonColliders.contains(c));
            REQUIRE(nonColliders.contains(d));

            std::vector<shared_ptr<Circle>> resultCircles(originalCircles.size());
        }

        SECTION("Merge the pairs") {
             // Merge them
             // make sure doomed ones are gone
             // ensure that merged items have reasonable properties
            uniquePairs.mergePairs();

//            REQUIRE(result.size() == 1);
//            REQUIRE(result.getShapes()[0]->getMass() == d->getMass() * 2);
        }
    }
    */

}
