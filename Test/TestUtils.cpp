//
// Created by bfrasure on 8/10/18.
//

#include "TestUtils.h"

shared_ptr<Circle> TestUtils::testCircle() {
    auto mass = 100;
    float radius = 10;
    float density = 1;

    sgVec4 aPos = { 0, 0, 0, 1 };
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

unique_ptr<Circle> TestUtils::improvedTestCircle() {
    auto mass = 100;
    float density = 1;

    sgVec4 aPos = { 0, 0, 0, 1 };
    sgVec4 startMom = { 0, 0, 0, 0 };
    sgVec3 color = { 255, 255, 0 };

    return std::make_unique<Circle>(
            aPos,
            mass,
            startMom,
            density,
            color
    );
}

shared_ptr<Circle> TestUtils::circleAt(float x, float y, float z) {
    auto mass = 100;
    float radius = 10;
    float density = 1;

    sgVec4 aPos = { x, y, z, 1 };
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
