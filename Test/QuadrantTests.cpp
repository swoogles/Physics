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
        REQUIRE(quadrant.mass() == shape->mass());
    }

    SECTION("Version with initial shape should match behavior of shape-added-later version") {
        auto a = TestUtils::circleAt(5, 5, 5);
        auto b = TestUtils::circleAt(5, 5, -5);
        auto c = TestUtils::circleAt(5, -5, 5);
        auto d = TestUtils::circleAt(-5, 5, 5);

        Quadrant lessMutableQuadrant(0, pos, width, units::length::meter_t(), PhysicalVector(),
                                     units::mass::kilogram_t(), PhysicalVector());
        lessMutableQuadrant.insert(units::length::meter_t(), PhysicalVector(), units::mass::kilogram_t(),
                                   PhysicalVector());
        lessMutableQuadrant.insert(units::length::meter_t(), PhysicalVector(), units::mass::kilogram_t(),
                                   PhysicalVector());
        lessMutableQuadrant.insert(units::length::meter_t(), PhysicalVector(), units::mass::kilogram_t(),
                                   PhysicalVector());

        int counter = 0;
        function<void(Quadrant)> countingFunction = [&counter](Quadrant quadrant) {
            counter += 1;
        };
//        lessMutableQuadrant.applyToAllChildren(countingFunction);
        REQUIRE(counter == 5);
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
