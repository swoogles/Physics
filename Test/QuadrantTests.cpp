//
// Created by bfrasure on 11/2/18.
//

#include "catch.hpp"

#include "../src/ShapeFiles/TouchingPair.h"
#include "../src/ShapeFiles/PairCollection.h"
#include "../src/ShapeFiles/Circle.h"
#include "../src/Parallelization/Quadrant.h"
#include "TestUtils.h"
#include <plib/sg.h>

#include <memory>
#include <functional>

TEST_CASE("Get children", "[Quadrant]") {
    VecStruct pos;
    float width = 200;
    auto shape = TestUtils::circleAt(5, 5, 5);
    Quadrant quadrant(shape, 0, pos, width);
    SECTION("Basic properties") {
        REQUIRE(quadrant.children().empty());
        REQUIRE(quadrant.getMass() == shape->getMass());
        REQUIRE(quadrant.getShapeInQuadrant() == shape);
    }

    SECTION("Version with initial shape should match behavior of shape-added-later version") {
        auto a = TestUtils::circleAt(5, 5, 5);
        auto b = TestUtils::circleAt(5, 5, -5);
        auto c = TestUtils::circleAt(5, -5, 5);
        auto d = TestUtils::circleAt(-5, 5, 5);

        Quadrant lessMutableQuadrant(a, 0, pos, width);
        lessMutableQuadrant.insert(b);
        lessMutableQuadrant.insert(c);
        lessMutableQuadrant.insert(d);

        int counter = 0;
        function<void(Quadrant)> countingFunction = [&counter](Quadrant quadrant) {
            counter += 1;
        };
        lessMutableQuadrant.applyToAllChildren(countingFunction);
        REQUIRE(counter == 5);
    }

    SECTION("SubQuadrant") {
        Quadrant quadrantForSubdivision(shape, 0, pos, width);
        auto subWidth = width / 2.0f;
        auto offset = subWidth / 2.0f;
        SECTION("[0,0,0]") { // 1
            auto subShape =  TestUtils::circleAt(5, 5, 5);
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(subShape);
            auto subQuadrantPos = subQuadrant->getPos();
            REQUIRE(subQuadrantPos.hasValues(offset, offset, offset));
        }

        SECTION("[0,0,1]") { // 2
            auto subShape =  TestUtils::circleAt(5, 5, -5);
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(subShape);
            auto subQuadrantPos = subQuadrant->getPos();
            REQUIRE(subQuadrantPos.hasValues(offset, offset, -offset));
        }

        SECTION("[0,1,0]") { // 3
            auto subShape =  TestUtils::circleAt(5, -5, 5);
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(subShape);
            auto subQuadrantPos = subQuadrant->getPos();
            REQUIRE(subQuadrantPos.hasValues(offset, -offset, offset));
        }

        SECTION("[0,1,1]") { // 4
            auto subShape =  TestUtils::circleAt(5, -5, -5);
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(subShape);
            auto subQuadrantPos = subQuadrant->getPos();
            REQUIRE(subQuadrantPos.hasValues(offset, -offset, -offset));
        }

        SECTION("[1,0,0]") { // 5
            auto subShape =  TestUtils::circleAt(-5, 5, 5);
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(subShape);
            auto subQuadrantPos = subQuadrant->getPos();
            REQUIRE(subQuadrantPos.hasValues(-offset, offset, offset));
        }

        SECTION("[1,0,1]") { // 6
            auto subShape =  TestUtils::circleAt(-5, 5, -5);
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(subShape);
            auto subQuadrantPos = subQuadrant->getPos();
            REQUIRE(subQuadrantPos.hasValues(-offset, offset, -offset));
        }

        SECTION("[1,1,0]") { // 7
            auto subShape =  TestUtils::circleAt(-5, -5, 5);
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(subShape);
            auto subQuadrantPos = subQuadrant->getPos();
            REQUIRE(subQuadrantPos.hasValues(-offset, -offset, offset));
        }

        SECTION("[1,1,1]") { // 8
            auto subShape =  TestUtils::circleAt(-5, -5, -5);
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(subShape);
            auto subQuadrantPos = subQuadrant->getPos();
            REQUIRE(subQuadrantPos.hasValues(-offset, -offset, -offset));
        }
    }

    SECTION("Multiple Insertions") {
        auto a = TestUtils::circleAt(5, 5, 5);
        quadrant.insert(a);
        auto b = TestUtils::circleAt(5, 5, -5);
        quadrant.insert(b);
        quadrant.insert(TestUtils::circleAt(-5, 5, 5));
        quadrant.insert(TestUtils::circleAt(5, -5, 5));

        for (const auto &subQuadrant: quadrant.children()) {
            REQUIRE_FALSE(subQuadrant == nullptr);
        }
//        TODO Reinstate
//        REQUIRE(quadrant.children().size() == 2);

    }

    SECTION("Get null subquadrant") {
        REQUIRE(quadrant.getQuadrantFromCell(0, 0, 0) == nullptr);
        REQUIRE(quadrant.getQuadrantFromCell(0, 0, 1) == nullptr);
        REQUIRE(quadrant.getQuadrantFromCell(0, 1, 0) == nullptr);
        REQUIRE(quadrant.getQuadrantFromCell(0, 1, 1) == nullptr);
        REQUIRE(quadrant.getQuadrantFromCell(1, 0, 0) == nullptr);
        REQUIRE(quadrant.getQuadrantFromCell(1, 0, 1) == nullptr);
        REQUIRE(quadrant.getQuadrantFromCell(1, 1, 0) == nullptr);
        REQUIRE(quadrant.getQuadrantFromCell(1, 1, 1) == nullptr);
    }
}
