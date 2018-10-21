#include "catch.hpp"

#include <plib/sg.h>

#include "../src/ShapeFiles/Circle.h"
#include "../src/Physics/Simulation.h"
#include "../src/Physics/Simulations.h"
#include "../src/Physics/Interactions.h"

TEST_CASE( "Items are being removed from Simulation when Octree force calculations are used", "[XXX]" ) {
    SimulationPointer_t  simulation = Simulations::QuadrantTesting_simplest();
    simulation->setForceCalcMethod(ForceCalculationMethod::OCTREE);

    simulation->update();
    REQUIRE(simulation->getPhysicalObjects().getShapes().size() == 1);
}

TEST_CASE( "Simulation is made", "[simulation]" ) {
    SimulationPointer_t  testSimulationPtr = Simulations::QuadrantTesting_simplest();

    // REQUIRE( testSimulation.getForceCalcMethod() == Simulation::FORCE_CALC_METHOD_OCTREE );
    REQUIRE( testSimulationPtr->getDT() == 1000.0 );
    REQUIRE( testSimulationPtr->getTimeElapsed() == 0 );
    REQUIRE( testSimulationPtr->isPaused() );
    REQUIRE( testSimulationPtr->getCollisionType() == CollisionType::INELASTIC );
    REQUIRE_FALSE( testSimulationPtr->isConstGravField() );
    REQUIRE( testSimulationPtr->isGravBetweenObjects() );

    float distance = 3000;
    SECTION( "Update dimensions" ) {
        sgVec4 pos;
        pos[0] = distance;
        pos[1] = distance;
        pos[2] = distance;
        testSimulationPtr->updateXYMinsAndMaxes( pos );

    }

}
