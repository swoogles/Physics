//
// Created by bfrasure on 8/10/18.
//

#include "catch.hpp"

#include <plib/sg.h>
#include "TestUtils.h"

#include "../src/ShapeFiles/Circle.h"
#include "../src/Physics/Simulation.h"
#include "../src/Physics/Simulations.h"
#include "../src/Physics/Interactions.h"

TEST_CASE( "Remove ref", "[current]" ) {
    auto a = TestUtils::testCircle();
    auto b = TestUtils::testCircle();
    ShapeList shapes(vectorT { a, b});
    REQUIRE(shapes.size() == 2);
    shapes.removeShapeFromList(*a);
    cout << "in between destruction" << endl;
    REQUIRE(shapes.size() == 1);
}

TEST_CASE( "Construct a list with a single item", "[red]" ) {
    auto a = TestUtils::testCircle();
    ShapeList singleItemList(a);
    REQUIRE(singleItemList.size() == 1);
    REQUIRE(singleItemList.getShapes()[0]->getMass() == 100);
    REQUIRE(singleItemList.contains(*a));
}

TEST_CASE( "hasConflicts with an existing item", "[green]" ) {
    auto a = TestUtils::testCircle();
    auto b = TestUtils::testCircle();
    ShapeList singleItemList(a);
    REQUIRE(singleItemList.hasConflictsWith(*b));
}

TEST_CASE( "Adding a ShapeList to ShapeList", "[green]" ) {
    auto a = TestUtils::testCircle();
    auto b = TestUtils::testCircle();
    auto c = TestUtils::testCircle();
    auto d = TestUtils::testCircle();
    ShapeList aList(a);
    ShapeList bList(b);
    ShapeList cList(c);
    ShapeList dList(d);
    ShapeList resultList;
    resultList.addList(aList);
    resultList.addList(bList);
    resultList.addList(cList);
    resultList.addList(dList);
    REQUIRE(resultList.size() == 4);
    REQUIRE(resultList.contains(*a));
    REQUIRE(resultList.contains(*b));
    REQUIRE(resultList.contains(*c));
    REQUIRE(resultList.contains(*d));
}
