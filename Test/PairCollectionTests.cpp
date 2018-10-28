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

TEST_CASE("normalize list of pairs", "[red]") {
    auto a = TestUtils::testCircle();
    auto b = TestUtils::testCircle();
    auto c = TestUtils::testCircle();
    auto d = TestUtils::testCircle();

    cout << "a uses outter start: " << a.use_count() << endl;
    const TouchingPair ab(a, b);
    const TouchingPair ba(b, a);

    vectorT originalCircles = {a, b, c, d};
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
            REQUIRE(doomed.contains(b));
        }

        SECTION("manage the non-colliding objects") {
            ShapeList originalShapes(originalCircles);
            auto nonColliders = ComplicatedFunctions::nonCollidingObjects(originalShapes, uniquePairs);
            // call new function here
            REQUIRE(nonColliders.size() == 2);
            REQUIRE(nonColliders.contains(c));
            REQUIRE(nonColliders.contains(d));

            std::vector<shared_ptr<Circle>> resultCircles(originalCircles.size());
            /*
            std::for_each(nonCollidingObjects.begin(), nonCollidingObjects.end(),
                          [](auto circle) { cout << "Use count: " << circle.use_count() << endl; });

            std::for_each(originalCircles.begin(), originalCircles.end(), [](auto circle) { circle.reset(); });
            std::for_each(nonCollidingObjects.begin(), nonCollidingObjects.end(),
                          [](auto circle) { cout << "Use count: " << circle.use_count() << endl; });
            cout << "Done manually releasing" << endl;
             */
        }

        SECTION("Merge the pairs") {
            /*
             * Merge them
             * make sure doomed ones are gone
             * ensure that merged items have reasonable properties
             */
            uniquePairs.mergePairs();

//            REQUIRE(result.size() == 1);
//            REQUIRE(result.getShapes()[0]->getMass() == d->getMass() * 2);
        }
    }

    cout << "a uses outter end: " << a.use_count() << endl;
}
