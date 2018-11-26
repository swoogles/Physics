//
// Created by bfrasure on 10/30/18.
//

#include "catch.hpp"

#include "../src/ShapeFiles/TouchingPair.h"
#include "../src/ShapeFiles/PairCollection.h"
#include "ShapeFiles/Particle.h"
#include "TestUtils.h"
#include <plib/sg.h>

#include <memory>

TEST_CASE( "confirm size of Particle", "[green]" ) {
    auto a = TestUtils::testCircle();
    cout << "Size of Particle: " << sizeof(*a) << endl;
}


TEST_CASE( "merge Circles", "[green]" ) {
    particle_t a = TestUtils::improvedTestCircle();
    particle_t b = TestUtils::improvedTestCircle();
    auto originalMass = a->getMass() + b->getMass();
    cout << "Radius of a: " << a->getRadius() << endl;
    cout << "Radius of b: " << b->getRadius() << endl;
    a->mergeWith(*b);
    REQUIRE(a->getRadius() > b->getRadius());
    REQUIRE(a->getMass() == originalMass);
}

TEST_CASE("Equally distribute points on a sphere", "[todo]") {
    auto numPoints = 5;
    auto radius = 1;

    auto results = Particle::pointsEvenlyDistributedOnSphere(numPoints, radius);
    SECTION("Numerous points to be validated" ) {
        auto pointsInCorrectPosition = false;
        REQUIRE(pointsInCorrectPosition);
    }
}
