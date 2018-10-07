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

auto mass = 100;
float radius = 10;
float density = 1;

TEST_CASE( "Simple pair test", "[pair]" ) {
    sgVec4 startPos = { 0, 0, 0, 1 };
    sgVec4 startMom = { 0, 0, 0, 0 };
    sgVec3 color = { 255, 255, 0 };
//    unique_ptr
    Circle a(
            startPos,
            mass,
            radius,
            startMom,
            density,
            color
    );

    auto p = std::make_unique<Circle>( // p is a unique_ptr that owns a D
            startPos,
            mass,
            radius,
            startMom,
            density,
            color
            );

    auto q = pass_through(std::move(p));
    assert(!p); // now p owns nothing and holds a null pointer
    q->getScale();   // and q owns the D object


}

shared_ptr<Circle> testCircle() {
    sgVec4 aPos = { 0, 0, 0, 1 };
    sgVec4 bPos = { 1, 0, 0, 1 };
    sgVec4 startMom = { 0, 0, 0, 0 };
    sgVec3 color = { 255, 255, 0 };

    return std::make_shared<Circle>(
            aPos,
            mass,
            radius,
            startMom,
            density,
            color
    );
}

TEST_CASE( "sameItems tests", "[pair]" ) {
    auto a = testCircle();
    auto b = testCircle();
    auto c = testCircle();

    TouchingPair pair1(a, b);

    SECTION("has same items as itself.") {
        REQUIRE(pair1.sameItems(pair1));
    }

    SECTION("has same items as reverse of itself.") {
        TouchingPair reversed(b, a);
        REQUIRE(pair1.sameItems(reversed));
    }


    SECTION("does not have same items.") {
        TouchingPair pair3(a, c);
        REQUIRE_FALSE(pair1.sameItems(pair3));
    }
}

TEST_CASE("normalize list of pairs", "[pair]") {
    auto a = testCircle();
    auto b = testCircle();
    auto c = testCircle();
    auto d = testCircle();

    const TouchingPair ab(a, b);
    const TouchingPair ba(b, a);
    const TouchingPair ac(a, c);
    const TouchingPair ca(c, a);
    const TouchingPair ad(c, d);

    std::vector<TouchingPair> originalPairs{
        ab,
        ba,
        ac,
        ca
    };

    std::vector<TouchingPair> normalizedPairs;
    auto acFunc = [&ac](const TouchingPair pair) { return ac.sameItems(pair); };
    auto result = std::any_of(originalPairs.begin(), originalPairs.end(), acFunc);
    REQUIRE(result);

    auto result2 = std::any_of(originalPairs.begin(), originalPairs.end(), [&ad](const TouchingPair pair) { return ad.sameItems(pair); });
    REQUIRE_FALSE(result2);

    for (auto const & curPair : originalPairs) {
        auto pairFunc = [&curPair](const TouchingPair pair) { return curPair.sameItems(pair); };
        auto foundInDestination = std::any_of(normalizedPairs.begin(), normalizedPairs.end(), pairFunc);
        if (!foundInDestination) {
            cout << "Inserting" << endl;
            normalizedPairs.push_back(curPair);
        }
        cout << "Found: " << foundInDestination << endl;


    }

}
