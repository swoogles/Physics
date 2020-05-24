#include "catch.hpp"

#include "../src/Physics/Simulation.h"
#include "../src/Physics/Simulations.h"

#include "TestUtils.h"

TEST_CASE( "Items are being removed from Simulation when Octree force calculations are used", "[green]" ) {
    Simulation  simulation = Simulations::QuadrantTesting_simplest();
    simulation.update(hour_t(1));
    REQUIRE(simulation.getSize() == 3);
}

TEST_CASE( "Run simple simulation until merge happens", "[green]" ) {
    Simulation  simulation = Simulations::QuadrantTesting_simplest();
    REQUIRE(simulation.getSize() == 3);
    for (int i = 0; i < 5e5; i++) {
        simulation.update(hour_t(10000));
    }
    REQUIRE(simulation.getSize() == 1);
}

TEST_CASE( "Simulation is made", "[green]" ) {
    Simulation  testSimulation = Simulations::QuadrantTesting_simplest();

    // REQUIRE( testSimulation.getForceCalcMethod() == Simulation::FORCE_CALC_METHOD_OCTREE );
    REQUIRE( testSimulation.getTimeElapsed() == hour_t(0) );
}



