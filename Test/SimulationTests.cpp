#include "catch.hpp"

#include <plib/sg.h>

#include "../src/ShapeFiles/Circle.h"
#include "../src/Physics/Simulation.h"
#include "../src/Physics/Simulations.h"
#include "../src/Physics/Interactions.h"

TEST_CASE( "Items are being removed from Simulation when Octree force calculations are used", "[removing items]" ) {
    bool testWritten = false;
    SimulationPointer_t  simulation = Simulations::QuadrantTesting_simplest();
    // TODO these should *NOT* be necessary after constructing the simulation
    simulation->updateMinsAndMaxes();
    simulation->setForceCalcMethod(ForceCalculationMethod::OCTREE);
    simulation->refreshQuadrant();
    // TODO put this in Interactions namespace
    calcForcesAll(simulation);
    simulation->update();
    // Even though the simulation shapes list is updated, something is still wrong. Possibly I'm losing the handle on
    // the shapes elsewhere?
    REQUIRE(simulation->shapes.getShapes().size() == 1);
    REQUIRE( testWritten );
}

TEST_CASE( "Simulation is made", "[simulation]" ) {

    int sideLength = 100;
    Simulation testSimulation(sideLength);

    // REQUIRE( testSimulation.getForceCalcMethod() == Simulation::FORCE_CALC_METHOD_OCTREE );
    REQUIRE( testSimulation.getCurStep() == 0 );
    REQUIRE( testSimulation.getDT() == 1 );
    REQUIRE( testSimulation.getTimeElapsed() == 0 );
    REQUIRE( testSimulation.isPaused() == true );
    REQUIRE( testSimulation.isAllElastic() == false );
    REQUIRE( testSimulation.isAllInelastic() == true );
    REQUIRE( testSimulation.isConstGravField() == false );
    REQUIRE( testSimulation.isGravBetweenObjects() == true );

    float distance = 3000;
    SECTION( "Update dimensions" ) {
        sgVec4 pos;
        pos[0] = distance;
        pos[1] = distance;
        pos[2] = distance;
        testSimulation.updateXYMinsAndMaxes( pos );

    }

}
