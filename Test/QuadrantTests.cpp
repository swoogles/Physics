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

TEST_CASE("Get children", "[Quadrant]") {
    VecStruct pos;
    VecStruct dimensions{200, 200, 200};
    Quadrant quadrant(0, pos, dimensions);
    REQUIRE(quadrant.children().empty());

    auto a = TestUtils::testCircle();
    quadrant.insertShape(a);
    REQUIRE(quadrant.children().size() == 0);
    auto b = TestUtils::testCircle();
    b->setPos(10, 5, 2);
    quadrant.insertShape(b);
    REQUIRE(quadrant.children().size() == 1);


}
