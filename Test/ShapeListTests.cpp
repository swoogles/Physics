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

TEST_CASE( "Remove ref", "[ParticleList]" ) {
    ParticleList shapes(particleVector { TestUtils::testCircle(), TestUtils::testCircle()});
    REQUIRE(shapes.size() == 2);
}

TEST_CASE( "contains ptr", "[ParticleList]" ) {
    auto a = TestUtils::testCircle();
    ParticleList shapes(a);
    REQUIRE(shapes.contains(a));
}

TEST_CASE( "Remove shared_ptr", "[ParticleList]" ) {
    auto a = TestUtils::testCircle();
    ParticleList shapes({a, TestUtils::testCircle()});
    REQUIRE(shapes.size() == 2);
    shapes.removeShapeFromList(a);
    cout << "in between destruction" << endl;
    REQUIRE(shapes.size() == 1);
}

TEST_CASE( "Remove a ParticleList from ParticleList", "[ParticleList]" ) {
    auto a = TestUtils::testCircle();
    auto b = TestUtils::testCircle();
    auto c = TestUtils::testCircle();
    auto d = TestUtils::testCircle();
    ParticleList permanentShapes({a, b});
    ParticleList doomedShapes({c, d});
    ParticleList allShapes({a, b, c, d});
    REQUIRE(allShapes.size() == 4);
    allShapes.remove(doomedShapes);
    REQUIRE(allShapes.size() == 2);
    REQUIRE(allShapes.contains(a));
    REQUIRE(allShapes.contains(b));
    REQUIRE_FALSE(allShapes.contains(c));
    REQUIRE_FALSE(allShapes.contains(d));
}


TEST_CASE( "Construct a list with a single item", "[ParticleList][red]" ) {
    auto a = TestUtils::testCircle();
    ParticleList singleItemList(a);
    REQUIRE(singleItemList.size() == 1);
    REQUIRE(singleItemList.contains(a));
}

TEST_CASE( "hasConflicts with an existing item", "[ParticleList][green]" ) {
    auto a = TestUtils::testCircle();
    auto b = TestUtils::testCircle();
    ParticleList singleItemList(a);
    REQUIRE(singleItemList.hasConflictsWith(*b));
}

TEST_CASE( "Adding a ParticleList to ParticleList", "[ShapeList][green]" ) {
    auto a = TestUtils::testCircle();
    auto b = TestUtils::testCircle();
    auto c = TestUtils::testCircle();
    auto d = TestUtils::testCircle();
    ParticleList aList(a);
    ParticleList bList(b);
    ParticleList cList(c);
    ParticleList dList(d);
    ParticleList resultList;
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
