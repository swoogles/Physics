//
// Created by bfrasure on 10/30/18.
//

#include "catch.hpp"

#include "../src/ShapeFiles/TouchingPair.h"
#include "../src/ShapeFiles/PairCollection.h"
#include "ShapeFiles/Particle.h"
#include "TestUtils.h"

TEST_CASE( "confirm size of Particle", "[green]" ) {
    auto a = TestUtils::testCircle();
    cout << "Size of Particle: " << sizeof(*a) << endl;
}


TEST_CASE( "merge Circles", "[green]" ) {
    shared_ptr<Particle> a = TestUtils::improvedTestCircle();
    shared_ptr<Particle> b = TestUtils::improvedTestCircle();
    auto originalMass = a->mass() + b->mass();
    cout << "Radius of a: " << a->radius() << endl;
    cout << "Radius of b: " << b->radius() << endl;
    a->mergeWith(*b);
    REQUIRE(a->radius() > b->radius());
    REQUIRE(a->mass() == originalMass);
}

TEST_CASE("Equally distribute points on a sphere", "[todo]") {
    auto numPoints = 5;
    auto radius = 1;

    auto results = Particle::pointsEvenlyDistributedOnSphere(numPoints, radius);
    SECTION("Numerous points to be validated" ) {
    }
}
