//
// Created by bfrasure on 11/20/18.
//
#include "catch.hpp"

#include "../src/Physics/Simulation.h"
#include "../src/Physics/Simulations.h"

#include "TestUtils.h"

TEST_CASE( "Randomly distribute objects in a sphere", "[current]" ) {
    srand (static_cast <unsigned> (time(0)));

    PhysicalVector startPos;
    double maxDistance = 10;
    PhysicalVector target(0,0,0,false);
    Simulations::randomPointInSphere(maxDistance, target);
    PhysicalVector res(startPos);
    cout << "res: " << res << endl;

}

