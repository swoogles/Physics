#include "catch.hpp"

#include "../src/Physics/Simulation.h"
#include "../src/Physics/Simulations.h"

#include "TestUtils.h"

TEST_CASE( "Items are being removed from Simulation when Octree force calculations are used", "[green]" ) {
    SimulationPointer_t  simulation = Simulations::QuadrantTesting_simplest(NAIVE);
    simulation->update(0);
    REQUIRE(simulation->getSize() == 3);
}

TEST_CASE( "Run simple simulation until merge happens", "[green]" ) {
    SimulationPointer_t  simulation = Simulations::QuadrantTesting_simplest(NAIVE);
    auto initialMass = simulation->getMass();
    REQUIRE(simulation->getSize() == 3);
    for (int i = 0; i < 1e4; i++) {
        simulation->update(0);
    }
    REQUIRE(simulation->getSize() == 1);
    auto mergedMass = simulation->getMass();
    REQUIRE(initialMass == mergedMass);
}

TEST_CASE( "New simulation is created from original Simulation and additional shapes", "[green]" ) {
    SimulationPointer_t simulation = Simulations::QuadrantTesting_simplest(NAIVE);
    REQUIRE(simulation->getSize() == 3);
    auto a = TestUtils::testCircle();
    ShapeList newShapes(a);
    Simulation & simRef = *simulation;
    Simulation newSimulation(std::move(simRef), newShapes);
    REQUIRE(newSimulation.getSize() == 4);
}

TEST_CASE( "Simulation is made", "[green]" ) {
    SimulationPointer_t  testSimulationPtr = Simulations::QuadrantTesting_simplest(NAIVE);

    // REQUIRE( testSimulation.getForceCalcMethod() == Simulation::FORCE_CALC_METHOD_OCTREE );
    REQUIRE( testSimulationPtr->getTimeElapsed() == 0 );
}

TEST_CASE( "non-ptr Simulation is made", "[green]" ) {
    shared_ptr<Simulation> simulation = Simulations::QuadrantTesting_simplest(NAIVE);
}
