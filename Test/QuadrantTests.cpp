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

    SECTION("Single Insertion") {
        auto a = TestUtils::testCircle();
        quadrant.insertShape(a);
        REQUIRE(quadrant.children().empty());
    }
    SECTION("Multiple Insertions") {
        auto a = TestUtils::testCircle();
        quadrant.insertShape(a);
        auto b = TestUtils::circleAt(5, 5, 5);
        quadrant.insertShape(b);
        REQUIRE(quadrant.children().size() == 1);
        quadrant.insertShape(TestUtils::circleAt(-5, 5, 5));
        quadrant.insertShape(TestUtils::circleAt(5, -5, 5));

        for (const auto &child: quadrant.children()) {
            cout << "CHILD: " << child << endl;
        }

        REQUIRE(quadrant.children().size() == 2);

        PairCollection deleteList;
        for (const auto &subQuadrant: quadrant.children()) {
            REQUIRE_FALSE(subQuadrant == nullptr);
//        deleteList.insertUniqueElements(calcForceOnObject_Octree(curObject, subQuadrant, 10, recursionLevel + 1)) ;
        }
    }
}
