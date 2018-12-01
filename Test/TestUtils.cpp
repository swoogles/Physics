//
// Created by bfrasure on 8/10/18.
//

#include "TestUtils.h"

shared_ptr<Particle> TestUtils::testCircle() {
    auto mass = kilogram_t(100);
    auto density = kilograms_per_cubic_meter_t(1);

    PhysicalVector aPos (0, 0, 0, 1);
    PhysicalVector startMom (0, 0, 0, false);
    PhysicalVector color(255, 255, 0, false);

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

    PhysicalVector aPos(0, 0, 0, 1);
    PhysicalVector startMom(0, 0, 0);
    PhysicalVector color(255, 255, 0, false);

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

    PhysicalVector aPos(x, y, z, 1);
    PhysicalVector startMom(0, 0, 0);
    PhysicalVector color(255, 255, 0, false);

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

    // TODO PhysicalVector<Pos> can be temporarily be played by a PosVec (SpatialVec?) class
    PhysicalVector aPos(x, y, z, true);
    PhysicalVector startMom(0, 0, 0);
    PhysicalVector color(255, 255, 0, false);

    return Particle(
            aPos,
            mass,
            startMom,
            density,
            color
    );
}
