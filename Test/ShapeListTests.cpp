//
// Created by bfrasure on 8/10/18.
//

#include "catch.hpp"

#include <plib/sg.h>
#include "TestUtils.h"

#include "../src/ShapeFiles/Circle.h"
#include "../src/Physics/Simulation.h"
#include "../src/Physics/Simulations.h"
#include "../src/Physics/Interactions.h"

TEST_CASE( "Ensure that ShapeList.remove invokes destructor", "[current]" ) {

    auto a = TestUtils::testCircle();
    auto b = TestUtils::testCircle();
    ShapeList shapes(vectorT { a, b});
    a.reset();
    shapes.removeShapeFromList(a);
    cout << "ending test" << endl;
}
