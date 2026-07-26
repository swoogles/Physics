/*
 * Simulations.cpp
 *
 *  Created on: Jul 22, 2011
 *      Author: brasure
 *
 *  Starting arrangements used to be written here, one hard-coded function per
 *  idea, selected by commenting lines in and out. They now live in properties
 *  files and are assembled by ScenarioParser / ScenarioBuilder, so this file
 *  only wires a scenario up to the simulation's global settings.
 */

#include "Simulations.h"

#include "AstronomicalValues.h"
#include "CollisionType.h"
#include "ShapeFiles/Particle.h"

Simulation Simulations::fromScenario(
        const ScenarioSpec &scenario,
        PhysicsSandboxProperties &properties,
        SetupDiagnostics &diagnostics,
        std::mt19937 &rng) {

    ParticleList physicalObjects = ScenarioBuilder::build(scenario, properties.dt, diagnostics, rng);

    /* Pace the playback off the scenario's own crossing time. Without this,
     * doubling the size of a scenario silently makes the video 3x slower. */
    const bool dtIsAuto = properties.dt <= 0;
    if ((dtIsAuto || properties.framesPerCrossing > 0) && diagnostics.crossingTimeSeconds > 0) {
        const float framesPerCrossing = properties.framesPerCrossing > 0
                ? properties.framesPerCrossing
                : 400.0f;
        properties.dt = (float) (diagnostics.crossingTimeSeconds / framesPerCrossing);
        diagnostics.applyDt(properties.dt);

        cout << "Resolved dt=" << properties.dt << "s from "
             << framesPerCrossing << " frames per crossing time" << endl;
    }

    Particle::setCollisionRadiusMultiplier(
            properties.collisionRadiusMultiplier,
            properties.mergeTargetFraction,
            properties.mergeTargetSteps,
            physicalObjects.size()
    );

    // Keep the run in frame: the soft boundary sits just outside where the
    // particles started, measured from the same centre of mass.
    Particle::setDampingAndBoundary(
            properties.velocityDamping,
            properties.boundaryStrength,
            diagnostics.systemRadius * properties.boundaryRadiusFactor,
            diagnostics.centerOfMass
    );

    Particle::setMinimumMergesPerFrame(properties.minimumMergesPerFrame);

    return Simulation(physicalObjects, CollisionType::INELASTIC, properties.octreeTheta);
}

Simulation Simulations::QuadrantTesting_simplest() {
    ParticleList physicalObjects;

    const int numPieces = 3;
    const kilograms_per_cubic_meter_t objectDensity = AstronomicalValues::DENSITY_SUN;
    const kilogram_t pieceMass = AstronomicalValues::MASS_SUN / numPieces;
    const PhysicalVector startMomentum(0, 0, 0);
    const PhysicalVector white(1, 1, 1);

    const float d = 2.0e4;

    const PhysicalVector placements[] = {
            PhysicalVector(+5 / 8.0f * d, +7 / 8.0f * d, 1, true),
            PhysicalVector(-7 / 8.0f * d, +7 / 8.0f * d, 1, true),
            PhysicalVector(-1 / 8.0f * d, -5 / 8.0f * d, 1, true),
            PhysicalVector(+1 / 8.0f * d, -5 / 8.0f * d, 1, true),
    };

    for (const auto &placement : placements) {
        physicalObjects.addShapeToList(
                make_shared<Particle>(placement, pieceMass, startMomentum, objectDensity, white));
    }

    return Simulation(physicalObjects, CollisionType::INELASTIC, 0.5);
}
