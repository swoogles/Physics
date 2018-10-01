#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <plib/sg.h>

#include "../src/ShapeFiles/Circle.h"
#include "../src/Physics/Simulation.h"

unsigned int Factorial( unsigned int number ) {
  return number > 1 ? Factorial(number-1)*number : 1;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
  REQUIRE( Factorial(0) == 1 );
  REQUIRE( Factorial(1) == 1 );
  REQUIRE( Factorial(2) == 2 );
  REQUIRE( Factorial(3) == 6 );
  REQUIRE( Factorial(10) == 3628800 );
}

TEST_CASE( "Simulation is made", "[simulation]" ) {

  Simulation testSimulation;

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

    // SECTION( "resizing gives a new radius" ) {
    //   testCircle.setRadius( 5 );
    //   REQUIRE( testCircle.getRadius() == 5 );
  }

  }
