//
// Created by bfrasure on 8/10/18.
//

#include "TestUtils.h"

shared_ptr<Particle> TestUtils::testCircle() {
    auto mass = kilogram_t(100);
    auto density = kilograms_per_cubic_meter_t(1);

    sgVec4 aPos = { 0, 0, 0, 1 };
    sgVec4 startMom = { 0, 0, 0, 0 };
    sgVec3 color = { 255, 255, 0 };

    return std::move(std::make_shared<Particle>(
            aPos,
            mass,
            startMom,
            density,
            color
    ));
}

unique_ptr<Particle> TestUtils::improvedTestCircle() {
    auto mass = kilogram_t(100);
    auto density = kilograms_per_cubic_meter_t(1);

    sgVec4 aPos = { 0, 0, 0, 1 };
    VecStruct startMom(0, 0, 0);
    sgVec3 color = { 255, 255, 0 };

    return std::make_unique<Particle>(
            aPos,
            mass,
            startMom,
            density,
            color
    );
}

shared_ptr<Particle> TestUtils::circleAt(float x, float y, float z) {
    auto mass = kilogram_t(100);
    auto density = kilograms_per_cubic_meter_t(1);

    sgVec4 aPos = { x, y, z, 1 };
    VecStruct startMom(0, 0, 0);
    sgVec3 color = { 255, 255, 0 };

    return std::make_shared<Particle>(
            aPos,
            mass,
            startMom,
            density,
            color
    );
}

 Particle TestUtils::simpleCircleAt(float x, float y, float z) {
    auto mass = kilogram_t(100);
    auto density = kilograms_per_cubic_meter_t(1);

    // TODO VecStruct<Pos> can be temporarily be played by a PosVec (SpatialVec?) class
    sgVec4 aPos = { x, y, z, 1 };
    VecStruct startMom(0, 0, 0);
    sgVec3 color = { 255, 255, 0 };

    return Particle(
            aPos,
            mass,
            startMom,
            density,
            color
    );
}
