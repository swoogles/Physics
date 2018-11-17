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
        cout << "1 " << endl;
        lessMutableQuadrant.insertShape(b);
        cout << "2 " << endl;
        lessMutableQuadrant.insertShape(c);
        cout << "3 " << endl;
        lessMutableQuadrant.insertShape(d);
        cout << "4 " << endl;

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
            cout << "Making subQuadrant for shape: " << subShape->getPos() << endl;
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(subShape);
            cout << "Made subQuadrant for shape: " << subShape->getPos() << endl;
            auto subQuadrantPos = subQuadrant->getPos();
            REQUIRE(subQuadrantPos.hasValues(offset, offset, offset));
        }

        /*
        SECTION("[0,0,1]") { // 2
            OctreeCoordinates coordinates(false, false, true);
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(coordinates);
            auto subQuadrantPos = subQuadrant->getPos();
            REQUIRE(subQuadrantPos.hasValues(offset, offset, -offset));
        }

        SECTION("[0,1,0]") { // 3
            OctreeCoordinates coordinates(false, true, false);
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(coordinates);
            auto subQuadrantPos = subQuadrant->getPos();
            REQUIRE(subQuadrantPos.hasValues(offset, -offset, offset));
        }

        SECTION("[0,1,1]") { // 4
            OctreeCoordinates coordinates(false, true, true);
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(coordinates);
            auto subQuadrantPos = subQuadrant->getPos();
            REQUIRE(subQuadrantPos.hasValues(offset, -offset, -offset));
        }

        SECTION("[1,0,0]") { // 5
            OctreeCoordinates coordinates(true, false, false);
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(coordinates);
            auto subQuadrantPos = subQuadrant->getPos();
            REQUIRE(subQuadrantPos.hasValues(-offset, offset, offset));
        }

        SECTION("[1,0,1]") { // 6
            OctreeCoordinates coordinates(true, false, true);
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(coordinates);
            auto subQuadrantPos = subQuadrant->getPos();
            REQUIRE(subQuadrantPos.hasValues(-offset, offset, -offset));
        }

        SECTION("[1,1,0]") { // 7
            OctreeCoordinates coordinates(true, true, false);
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(coordinates);
            auto subQuadrantPos = subQuadrant->getPos();
            REQUIRE(subQuadrantPos.hasValues(-offset, -offset, offset));
        }

        SECTION("[1,1,1]") { // 8
            OctreeCoordinates coordinates(true, true, true);
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(coordinates);
            auto subQuadrantPos = subQuadrant->getPos();
            REQUIRE(subQuadrantPos.hasValues(-offset, -offset, -offset));
        }
         */
    }

    SECTION("Multiple Insertions") {
        auto a = TestUtils::circleAt(5, 5, 5);
        quadrant.insertShape(a);
        auto b = TestUtils::circleAt(5, 5, -5);
        quadrant.insertShape(b);
        quadrant.insertShape(TestUtils::circleAt(-5, 5, 5));
        quadrant.insertShape(TestUtils::circleAt(5, -5, 5));

        for (const auto &child: quadrant.children()) {
            cout << "CHILD: " << child << endl;
        }

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
