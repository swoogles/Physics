#include "catch.hpp"

#include "../src/ShapeFiles/TouchingPair.h"
#include <plib/sg.h>

#include <memory>

// a function consuming a unique_ptr can take it by value or by rvalue reference
std::unique_ptr<Circle> pass_through(std::unique_ptr<Circle> p)
{
    p->getScale();
    return p;
}

TEST_CASE( "Simple pair test", "[factorial]" ) {
    sgVec4 startPos = { 0, 0, 0, 1 };
    sgVec4 startMom = { 0, 0, 0, 0 };
    auto mass = 100;
    float radius = 10;
    float density = 1;
    sgVec3 color = { 255, 255, 0 };
//    unique_ptr
    Circle a(
            startPos,
            mass,
            10,
            startMom,
            density,
            color
    );

    auto p = std::make_unique<Circle>( // p is a unique_ptr that owns a D
            startPos,
            mass,
            10,
            startMom,
            density,
            color

            );
    auto q = pass_through(std::move(p));
    assert(!p); // now p owns nothing and holds a null pointer
    q->getScale();   // and q owns the D object


    REQUIRE( 1 == 2);
}
