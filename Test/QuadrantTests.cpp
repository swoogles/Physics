//
// Created by bfrasure on 11/2/18.
//

#include "catch.hpp"

#include "../src/ShapeFiles/TouchingPair.h"
#include "../src/ShapeFiles/PairCollection.h"
#include "ShapeFiles/Particle.h"
#include "../src/Parallelization/Quadrant.h"
#include "TestUtils.h"
#include "Moveable.h"
#include <plib/sg.h>

#include <memory>

TEST_CASE("Get children", "[Quadrant]") {
    PhysicalVector pos;
    float width = 200;
    shared_ptr<Particle> shape = TestUtils::circleAt(5, 5, 5);
    
    Quadrant quadrant(0, pos, width, units::length::meter_t(), PhysicalVector(), units::mass::kilogram_t(),
                      PhysicalVector());
    SECTION("Basic properties") {
        // An empty quadrant weighs nothing until something is inserted into it.
        REQUIRE(quadrant.mass() == units::mass::kilogram_t(0));

        quadrant.insert(shape->radius(), shape->weightedPosition(), shape->mass(),
                        shape->position(), shape);
        REQUIRE(quadrant.mass() == shape->mass());
    }

    SECTION("Version with initial shape should match behavior of shape-added-later version") {
        auto a = TestUtils::circleAt(5, 5, 5);
        auto b = TestUtils::circleAt(5, 5, -5);
        auto c = TestUtils::circleAt(5, -5, 5);
        auto d = TestUtils::circleAt(-5, 5, 5);

        Quadrant lessMutableQuadrant(0, pos, width, units::length::meter_t(), PhysicalVector(),
                                     units::mass::kilogram_t(), PhysicalVector());
        for (const auto &particle : {a, b, c, d}) {
            lessMutableQuadrant.insert(particle->radius(), particle->weightedPosition(),
                                       particle->mass(), particle->position(), particle);
        }

        int counter = 0;
        // By reference: a Quadrant owns unique_ptrs and can't be copied.
        // The root was built around an empty body, so only count the leaves
        // that ended up holding one of the particles inserted above.
        function<void(Quadrant &)> countingFunction = [&counter](Quadrant &quadrant) {
            if (quadrant.getParticlePtr() != nullptr) {
                counter += 1;
            }
        };
        // The functor only fires where the predicate says to stop descending,
        // so stopping at leaves visits each particle's own quadrant exactly once.
        function<bool(Quadrant &)> stopAtLeaves = [](Quadrant &quadrant) {
            return quadrant.isExternal();
        };

        lessMutableQuadrant.applyToAllChildren(countingFunction, stopAtLeaves);

        // Four particles in four different octants, each in its own leaf.
        REQUIRE(counter == 4);
    }

    SECTION("Multiple Insertions") {
        auto a = TestUtils::circleAt(5, 5, 5);
        quadrant.insert(units::length::meter_t(), PhysicalVector(), units::mass::kilogram_t(), PhysicalVector());
        auto b = TestUtils::circleAt(5, 5, -5);
        quadrant.insert(units::length::meter_t(), PhysicalVector(), units::mass::kilogram_t(), PhysicalVector());
        quadrant.insert(units::length::meter_t(), PhysicalVector(),
                        units::mass::kilogram_t(), PhysicalVector());
        quadrant.insert(units::length::meter_t(), PhysicalVector(),
                        units::mass::kilogram_t(), PhysicalVector());

//        TODO Reinstate
//        REQUIRE(quadrant.children().size() == 2);

    }

}
