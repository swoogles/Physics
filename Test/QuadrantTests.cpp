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
        REQUIRE(quadrant.children().empty());
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

    SECTION("SubQuadrant") {
        Quadrant quadrantForSubdivision(0, pos, width, units::length::meter_t(), PhysicalVector(),
                                        units::mass::kilogram_t(), PhysicalVector());
        auto subWidth = width / 2.0f;
        auto offset = subWidth / 2.0f;
        SECTION("[0,0,0]") { // 1
            auto subShape =  TestUtils::circleAt(5, 5, 5);
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(units::length::meter_t(),
                                                                      PhysicalVector(), units::mass::kilogram_t(),
                                                                      PhysicalVector());
            auto subQuadrantPos = subQuadrant->position();
            REQUIRE(subQuadrantPos.hasValues(offset, offset, offset));
        }

        SECTION("[0,0,1]") { // 2
            auto subShape =  TestUtils::circleAt(5, 5, -5);
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(units::length::meter_t(),
                                                                      PhysicalVector(), units::mass::kilogram_t(),
                                                                      PhysicalVector());
            auto subQuadrantPos = subQuadrant->position();
            REQUIRE(subQuadrantPos.hasValues(offset, offset, -offset));
        }

        SECTION("[0,1,0]") { // 3
            auto subShape =  TestUtils::circleAt(5, -5, 5);
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(units::length::meter_t(),
                                                                      PhysicalVector(), units::mass::kilogram_t(),
                                                                      PhysicalVector());
            auto subQuadrantPos = subQuadrant->position();
            REQUIRE(subQuadrantPos.hasValues(offset, -offset, offset));
        }

        SECTION("[0,1,1]") { // 4
            auto subShape =  TestUtils::circleAt(5, -5, -5);
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(units::length::meter_t(),
                                                                      PhysicalVector(), units::mass::kilogram_t(),
                                                                      PhysicalVector());
            auto subQuadrantPos = subQuadrant->position();
            REQUIRE(subQuadrantPos.hasValues(offset, -offset, -offset));
        }

        SECTION("[1,0,0]") { // 5
            auto subShape =  TestUtils::circleAt(-5, 5, 5);
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(units::length::meter_t(),
                                                                      PhysicalVector(), units::mass::kilogram_t(),
                                                                      PhysicalVector());
            auto subQuadrantPos = subQuadrant->position();
            REQUIRE(subQuadrantPos.hasValues(-offset, offset, offset));
        }

        SECTION("[1,0,1]") { // 6
            auto subShape =  TestUtils::circleAt(-5, 5, -5);
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(units::length::meter_t(),
                                                                      PhysicalVector(), units::mass::kilogram_t(),
                                                                      PhysicalVector());
            auto subQuadrantPos = subQuadrant->position();
            REQUIRE(subQuadrantPos.hasValues(-offset, offset, -offset));
        }

        SECTION("[1,1,0]") { // 7
            auto subShape =  TestUtils::circleAt(-5, -5, 5);
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(units::length::meter_t(),
                                                                      PhysicalVector(), units::mass::kilogram_t(),
                                                                      PhysicalVector());
            auto subQuadrantPos = subQuadrant->position();
            REQUIRE(subQuadrantPos.hasValues(-offset, -offset, offset));
        }

        SECTION("[1,1,1]") { // 8
            auto subShape =  TestUtils::circleAt(-5, -5, -5);
            auto subQuadrant = quadrantForSubdivision.makeSubQuadrant(units::length::meter_t(),
                                                                      PhysicalVector(), units::mass::kilogram_t(),
                                                                      PhysicalVector());
            auto subQuadrantPos = subQuadrant->position();
            REQUIRE(subQuadrantPos.hasValues(-offset, -offset, -offset));
        }
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

        for (const auto &subQuadrant: quadrant.children()) {
            REQUIRE_FALSE(subQuadrant == nullptr);
        }
//        TODO Reinstate
//        REQUIRE(quadrant.children().size() == 2);

    }

    SECTION("Get null subquadrant") {
        REQUIRE(quadrant.getQuadrantFromCell(0, 0, 0) == nullptr);
        REQUIRE(quadrant.getQuadrantFromCell(0, 0, 1) == nullptr);
        REQUIRE(quadrant.getQuadrantFromCell(0, 1, 0) == nullptr);
        REQUIRE(quadrant.getQuadrantFromCell(0, 1, 1) == nullptr);
        REQUIRE(quadrant.getQuadrantFromCell(1, 0, 0) == nullptr);
        REQUIRE(quadrant.getQuadrantFromCell(1, 0, 1) == nullptr);
        REQUIRE(quadrant.getQuadrantFromCell(1, 1, 0) == nullptr);
        REQUIRE(quadrant.getQuadrantFromCell(1, 1, 1) == nullptr);
    }
}
