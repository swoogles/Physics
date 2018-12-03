//
// Created by bfrasure on 8/10/18.
//

#include "catch.hpp"

#include <plib/sg.h>
#include "TestUtils.h"

#include "ShapeFiles/Particle.h"
#include "../src/Physics/Simulation.h"
#include "../src/Physics/Simulations.h"
#include "../src/Physics/Interactions.h"
#include "Moveable.h"

using namespace units::mass;

TEST_CASE( "Remove ref", "[ShapeList]" ) {
    ShapeList shapes(vectorT { TestUtils::testCircle(), TestUtils::testCircle()});
    REQUIRE(shapes.size() == 2);
    shapes.removeShapeFromList(shapes.getShapes()[0]);
    cout << "in between destruction" << endl;
    REQUIRE(shapes.size() == 1);
}

TEST_CASE( "contains ptr", "[ShapeList]" ) {
    auto a = TestUtils::testCircle();
    shared_ptr<Moveable> ptr = a;
    ShapeList shapes(a);
    REQUIRE(shapes.contains(ptr));
}

TEST_CASE( "Remove shared_ptr", "[ShapeList]" ) {
    auto a = TestUtils::testCircle();
    ShapeList shapes({a, TestUtils::testCircle()});
    shared_ptr<Moveable> ptr = a;
    REQUIRE(shapes.size() == 2);
    shapes.removeShapeFromList(ptr);
    cout << "in between destruction" << endl;
    REQUIRE(shapes.size() == 1);
}

TEST_CASE( "Remove a ShapeList from ShapeList", "[ShapeList]" ) {
    auto a = TestUtils::testCircle();
    auto b = TestUtils::testCircle();
    auto c = TestUtils::testCircle();
    auto d = TestUtils::testCircle();
    ShapeList permanentShapes({a, b});
    ShapeList doomedShapes({c, d});
    ShapeList allShapes({a, b, c, d});
    REQUIRE(allShapes.size() == 4);
    allShapes.remove(doomedShapes);
    REQUIRE(allShapes.size() == 2);
    REQUIRE(allShapes.contains(a));
    REQUIRE(allShapes.contains(b));
    REQUIRE_FALSE(allShapes.contains(c));
    REQUIRE_FALSE(allShapes.contains(d));
}


TEST_CASE( "Construct a list with a single item", "[ShapeList][red]" ) {
    auto a = TestUtils::testCircle();
    ShapeList singleItemList(a);
    REQUIRE(singleItemList.size() == 1);
    REQUIRE(singleItemList.getShapes()[0]->mass() == kilogram_t(100));
    REQUIRE(singleItemList.contains(a));
}

TEST_CASE( "hasConflicts with an existing item", "[ShapeList][green]" ) {
    auto a = TestUtils::testCircle();
    auto b = TestUtils::testCircle();
    ParticleList singleItemList(a);
    REQUIRE(singleItemList.hasConflictsWith(*b));
}

TEST_CASE( "Adding a ShapeList to ShapeList", "[ShapeList][green]" ) {
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
    REQUIRE(resultList.contains(a));
    REQUIRE(resultList.contains(b));
    REQUIRE(resultList.contains(c));
    REQUIRE(resultList.contains(d));
}
