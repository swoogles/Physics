//
// Created by bfrasure on 11/2/18.
//

#include "catch.hpp"

#include "../src/ShapeFiles/TouchingPair.h"
#include "../src/ShapeFiles/PairCollection.h"
#include "../src/ShapeFiles/Circle.h"
#include "../src/Parallelization/Quadrant.h"
#include "TestUtils.h"
#include <plib/sg.h>

#include <memory>

TEST_CASE("Get children") {
    VecStruct pos;
    VecStruct dimensions{200, 200, 200};
    Quadrant quadrant(0, pos, dimensions);
    REQUIRE(quadrant.children().empty());

    auto a = TestUtils::improvedTestCircle();

}
