//
// Created by bfrasure on 11/20/18.
//
#include "catch.hpp"

#include "../src/Physics/Simulation.h"
#include "../src/Physics/Simulations.h"

#include "TestUtils.h"

TEST_CASE( "Randomly distribute objects in a sphere", "[current]" ) {
    srand (static_cast <unsigned> (time(0)));

    sgVec4 startPos;
    double maxDistance = 10;
    sgVec4 target{0,0,0,0};
    Simulations::randomPointInSphere(startPos, maxDistance, target);
    VecStruct res(startPos);
    cout << "res: " << res << endl;

}

