//
// Created by bfrasure on 10/30/18.
//

#include "catch.hpp"

#include "../src/ShapeFiles/TouchingPair.h"
#include "../src/ShapeFiles/PairCollection.h"
#include "../src/ShapeFiles/Circle.h"
#include "TestUtils.h"
#include <plib/sg.h>

#include <memory>

TEST_CASE( "confirm size of Circle", "[current]" ) {
    auto a = TestUtils::testCircle();
    cout << "Size of Circle: " << sizeof(*a) << endl;
}
