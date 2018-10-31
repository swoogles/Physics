//
// Created by bfrasure on 10/30/18.
//

#include "catch.hpp"

#include "../src/ShapeFiles/TouchingPair.h"
#include "../src/ShapeFiles/PairCollection.h"
#include "../src/ShapeFiles/Circle.h"
#include "TestUtils.h"
#include <plib/sg.h>

#include <memory>

TEST_CASE( "confirm size of Circle", "[green]" ) {
    auto a = TestUtils::testCircle();
    cout << "Size of Circle: " << sizeof(*a) << endl;
}


TEST_CASE( "merge Circles", "[current]" ) {
    auto a = TestUtils::improvedTestCircle();
    auto b = TestUtils::improvedTestCircle();
    cout << "Radius of a: " << a->getRadius() << endl;
    cout << "Radius of b: " << b->getRadius() << endl;
    a->mergeWith(*b);
    REQUIRE(a->getRadius() > b->getRadius());
    REQUIRE(a->getMass() == b->getMass() * 2);
}
