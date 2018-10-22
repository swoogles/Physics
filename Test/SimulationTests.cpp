#include "catch.hpp"

#include <plib/sg.h>

#include "../src/ShapeFiles/Circle.h"
#include "../src/Physics/Simulation.h"
#include "../src/Physics/Simulations.h"
#include "../src/Physics/Interactions.h"

#include "TestUtils.h"

TEST_CASE( "Items are being removed from Simulation when Octree force calculations are used", "[green]" ) {
    SimulationPointer_t  simulation = Simulations::QuadrantTesting_simplest();
    simulation->setForceCalcMethod(ForceCalculationMethod::OCTREE);

    simulation->update();
    REQUIRE(simulation->getPhysicalObjects().getShapes().size() == 1);
}

TEST_CASE( "New simulation is created from original Simulation and additional shapes", "[XXX]" ) {
    SimulationPointer_t simulation = Simulations::QuadrantTesting_simplest();
    REQUIRE(simulation->getPhysicalObjects().getShapes().size() == 2);
    auto a = TestUtils::testCircle();
    ShapeList newShapes(a);
    Simulation & simRef = *simulation;
    Simulation newSimulation(std::move(simRef), newShapes);
    REQUIRE(newSimulation.getPhysicalObjects().getShapes().size() == 3);
}

TEST_CASE( "Simulation is made", "[simulation]" ) {
    SimulationPointer_t  testSimulationPtr = Simulations::QuadrantTesting_simplest();

    // REQUIRE( testSimulation.getForceCalcMethod() == Simulation::FORCE_CALC_METHOD_OCTREE );
    REQUIRE( testSimulationPtr->getDT() == 1000.0 );
    REQUIRE( testSimulationPtr->getTimeElapsed() == 0 );
    REQUIRE( testSimulationPtr->isPaused() );
}
