//
// Created by bfrasure on 8/10/18.
//

#include "catch.hpp"

#include "../src/ShapeFiles/TouchingPair.h"
#include "../src/ShapeFiles/PairCollection.h"
#include "../src/ShapeFiles/Circle.h"
#include "TestUtils.h"
#include <plib/sg.h>

#include <memory>

TEST_CASE("normalize list of pairs", "[green]") {
    auto a = TestUtils::testCircle();
    auto b = TestUtils::testCircle();
    auto c = TestUtils::testCircle();
    auto d = TestUtils::testCircle();

    const TouchingPair ab(a, b);
    const TouchingPair ba(b, a);
    const TouchingPair ac(a, c);
    const TouchingPair ca(c, a);
    const TouchingPair ad(c, d);

    std::vector<shared_ptr<Circle>> originalCircles = {a, b, c, d};
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

        SECTION("provide correct survivors and doomed entries") {
            auto survivors = uniquePairs.survivors();
            REQUIRE(survivors.size() == 1);
            REQUIRE(survivors.contains(a));
            auto doomed = uniquePairs.doomed();
            REQUIRE(doomed.size() == 1);
            REQUIRE(doomed.contains(b));
        }

        SECTION("manage the non-colliding objects") {
            auto brittlePairs = uniquePairs.brittlePairs();

            // YES! Ugly as fuck, but this is generally what I need.
            std::vector<shared_ptr<MyShape>> nonCollidingObjects(originalCircles.size());
            auto it = std::copy_if(
                    originalCircles.begin(),
                    originalCircles.end(),
                    nonCollidingObjects.begin(),
                    [brittlePairs](const auto &circle) {
                        return !std::any_of(
                                brittlePairs.begin(),
                                brittlePairs.end(),
                                [circle](const TouchingPair &pair) {
                                    return pair.getA() == circle || pair.getB() == circle;
                                });
                    });

            nonCollidingObjects.resize(std::distance(nonCollidingObjects.begin(), it));

            ShapeList nonColliders(nonCollidingObjects);
            REQUIRE(nonCollidingObjects.size() == 2);
            REQUIRE(nonColliders.contains(c));
            REQUIRE(nonColliders.contains(d));

            std::vector<shared_ptr<Circle>> resultCircles(originalCircles.size());
            std::for_each(nonCollidingObjects.begin(), nonCollidingObjects.end(),
                          [](auto circle) { cout << "Use count: " << circle.use_count() << endl; });

            std::for_each(originalCircles.begin(), originalCircles.end(), [](auto circle) { circle.reset(); });
            std::for_each(nonCollidingObjects.begin(), nonCollidingObjects.end(),
                          [](auto circle) { cout << "Use count: " << circle.use_count() << endl; });
            cout << "Done manually releasing" << endl;
        }
    }
}
