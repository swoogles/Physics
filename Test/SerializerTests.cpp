//
// Created by bfrasure on 10/30/18.
//

#include "catch.hpp"

#include "../src/Conversions/Serializer.h"
#include <plib/sg.h>

TEST_CASE( "Convert a string to a float", "[current]" ) {
    float value = Serializer::parseStringAsFloat("100");
    REQUIRE(value == 100.0f);

    REQUIRE(Serializer::parseStringAsFloat("1000.0") == 1000.0f);
}
