//
// Created by bfrasure on 11/13/18.
//

#include "catch.hpp"

#include <plib/sg.h>
#include "TestUtils.h"

#include "ShapeFiles/Particle.h"
#include "../src/Physics/Simulation.h"
#include "../src/Physics/Simulations.h"
#include "../src/Physics/Interactions.h"

float epsilon = 0.0001;
TEST_CASE( "Basics", "[VecStruct]" ) {
    VecStruct obj(1, 2, 3);
    REQUIRE(obj.x() == 1);
    REQUIRE(obj.y() == 2);
    REQUIRE(obj.z() == 3);
    REQUIRE(fabs(obj.length() - 3.74166f) < epsilon);

    SECTION("Unit vectors") {
        VecStruct expectedUnitResult(0.267261, 0.534522, 0.801784);
        REQUIRE( obj.unit() == expectedUnitResult);
    }

    VecStruct scaled = obj.scaledBy(-2.0);
    VecStruct expectedScaledResult(-2, -4, -6);
    REQUIRE( scaled == expectedScaledResult );

    VecStruct cancelledOut =
            obj.scaledBy(-1).plus(obj);
    VecStruct expectedCancelledOutResult(0, 0, 0);

    REQUIRE(cancelledOut == expectedCancelledOutResult);
}
