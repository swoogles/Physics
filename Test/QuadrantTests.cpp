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

TEST_CASE("Get children", "[Quadrant]") {
    VecStruct pos;
    float width = 200;
    Quadrant quadrant(0, pos, width);
    SECTION("Basic properties") {
        REQUIRE(quadrant.children().empty());
        REQUIRE(quadrant.getMass() == 0);
        REQUIRE(quadrant.getShapeInQuadrant() == nullptr);
    }

    SECTION("SubQuadrant") {
        Quadrant quadrantForSubdivision(0, pos, width);
        auto subWidth = width / 2.0f;
        auto offset = subWidth / 2.0f;
        SECTION("[0,0,0]") { // 1
            OctreeCoordinates coordinates(false, false, false);
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(coordinates);
            auto subQuadrantPos = subQuadrant->getPos();
            REQUIRE(subQuadrantPos.hasValues(offset, offset, offset));
        }

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
    }

    SECTION("Single Insertion") {
        auto a = TestUtils::testCircle();
        quadrant.insertShape(a);
        REQUIRE(quadrant.children().empty());
    }
    SECTION("Multiple Insertions") {
        auto a = TestUtils::circleAt(5, 5, 5);
        quadrant.insertShape(a);
        auto b = TestUtils::circleAt(5, 5, -5);
        quadrant.insertShape(b);
        REQUIRE(quadrant.children().size() == 2);
        quadrant.insertShape(TestUtils::circleAt(-5, 5, 5));
        REQUIRE(quadrant.children().size() == 3);
        quadrant.insertShape(TestUtils::circleAt(5, -5, 5));
        REQUIRE(quadrant.children().size() == 4);

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
