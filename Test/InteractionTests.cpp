//
// Created by bfrasure on 11/14/18.
//

#include "catch.hpp"

#include "TestUtils.h"

#include "../src/ShapeFiles/Circle.h"
#include "../src/Physics/Interactions.h"

TEST_CASE( "Split a physical object", "[todo]" ) {
    Interactions interactions;
    auto object = TestUtils::simpleCircleAt(0, 0, 0);
    auto joulesOfKineticEnergy = 100;
    auto numberOfPieces = 3;
    auto fragments = interactions.crackPhysicalObject(object, joulesOfKineticEnergy, numberOfPieces);
    SECTION("Broke into the desired number of fragments") {
        REQUIRE(fragments.size() == numberOfPieces);
    }
    SECTION("Momentum of all fragments sums to momentum of initial object") {
        auto properMomentumValues = false;
        REQUIRE(properMomentumValues);
    }
}
