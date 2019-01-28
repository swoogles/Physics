#include "catch.hpp"

#include "../src/Physics/Simulation.h"
#include "../src/Physics/Simulations.h"

#include "TestUtils.h"

TEST_CASE( "Items are being removed from Simulation when Octree force calculations are used", "[green]" ) {
    Simulation  simulation = Simulations::QuadrantTesting_simplest(NAIVE);
    simulation.update(hour_t(1));
    REQUIRE(simulation.getSize() == 3);
}

TEST_CASE( "Run simple simulation until merge happens", "[green]" ) {
    Simulation  simulation = Simulations::QuadrantTesting_simplest(NAIVE);
    auto initialMass = simulation.getMass();
    REQUIRE(simulation.getSize() == 3);
    for (int i = 0; i < 5e5; i++) {
        simulation.update(hour_t(10000));
    }
    REQUIRE(simulation.getSize() == 1);
    auto mergedMass = simulation.getMass();
    REQUIRE(initialMass == mergedMass);
}

TEST_CASE( "Simulation is made", "[green]" ) {
    Simulation  testSimulation = Simulations::QuadrantTesting_simplest(NAIVE);

    // REQUIRE( testSimulation.getForceCalcMethod() == Simulation::FORCE_CALC_METHOD_OCTREE );
    REQUIRE( testSimulation.getTimeElapsed() == hour_t(0) );
}

TEST_CASE("Detonate", "[simulation]") {
    auto a = TestUtils::circleAt(0, 0, 0);
    PhysicalVector newMomentum(5, 0, 0);
    a->adjustMomentum(newMomentum);
    SECTION("All objects are moving within certain range of original momentum.") {
        REQUIRE(false);
    }
    SECTION("Interactions") {
        SECTION("Some objects deflect without fragmenting") {
            REQUIRE(false);
        }
        SECTION("High energy impacts generate more fragments than low energy") {
            REQUIRE(false);
        }

    }
}


