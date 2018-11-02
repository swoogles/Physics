#include "catch.hpp"

#include "../src/ShapeFiles/TouchingPair.h"
#include "../src/ShapeFiles/PairCollection.h"
#include "../src/ShapeFiles/Circle.h"
#include "TestUtils.h"
#include <plib/sg.h>

#include <memory>

// a function consuming a unique_ptr can take it by value or by rvalue reference
std::unique_ptr<Circle> pass_through(std::unique_ptr<Circle> p)
{
    p->getScale();
    return p;
}

TEST_CASE( "sameItems tests", "[green]" ) {
    auto a = TestUtils::testCircle();
    auto b = TestUtils::testCircle();
    auto c = TestUtils::testCircle();

    TouchingPair pair1(a, b);

    SECTION("contains first item") {
        REQUIRE(pair1.contains(*a));
    }

    SECTION("contains second item") {
        REQUIRE(pair1.contains(*b));
    }

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

TEST_CASE("basic checks", "[green]") {
    auto a = TestUtils::testCircle();
    auto b = TestUtils::testCircle();
    auto c = TestUtils::testCircle();
    auto d = TestUtils::testCircle();

    const TouchingPair ab(a, b);
    const TouchingPair ba(b, a);
    const TouchingPair ac(a, c);
    const TouchingPair ca(c, a);
    const TouchingPair ad(c, d);

    std::vector<TouchingPair> originalPairs{
        ab,
        ba
    };

    SECTION("Can find a pair in a vector") {
        auto baFunc = [&ba](const TouchingPair pair) { return ba.sameItems(pair); };
        auto result = std::any_of(originalPairs.begin(), originalPairs.end(), baFunc);
        REQUIRE(result);
    }

    SECTION("Won't find a pair that is not in a vector") {
        auto result2 = std::any_of(originalPairs.begin(), originalPairs.end(),
                                   [&ad](const TouchingPair pair) { return ad.sameItems(pair); });
        REQUIRE_FALSE(result2);
    }

    SECTION("Only insert unique elements into destination vector") {
        PairCollection pairCollection;
        for (auto const &curPair : originalPairs) {
            pairCollection.insertIfUnique(curPair);
        }
        REQUIRE(pairCollection.size() == 1);
    }
}

TEST_CASE("add collections", "[green]") {
    const auto a = TestUtils::testCircle();
    const auto b = TestUtils::testCircle();
    const auto c = TestUtils::testCircle();

    const TouchingPair ab(a, b);
    const TouchingPair ba(b, a);
    const TouchingPair ac(a, c);
    const TouchingPair ca(c, a);

    SECTION("Add collection to collection") {
        PairCollection destination;
        cout << "1" << endl;
        PairCollection source;
        source.insertIfUnique(ab);
        source.insertIfUnique(ac);
        cout << "2" << endl;
        destination.insertUniqueElements(source);
        cout << "3" << endl;
        REQUIRE(destination.survivors().contains(a));
        REQUIRE(destination.doomed().contains(b));
        REQUIRE(destination.doomed().contains(c));
    }
}

