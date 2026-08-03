#include "ScenarioBuilder.h"

#include <cmath>
#include <iomanip>
#include <sstream>

#include "AstronomicalValues.h"
#include <ShapeFiles/Particle.h>

using physics_constants::G;

/*! Uniform in volume rather than in radius, so the sphere doesn't come out
 *  dense in the middle.
 *  Algorithm: https://karthikkaranth.me/blog/generating-random-points-in-a-sphere/
 */
PhysicalVector ScenarioBuilder::randomPointInSphere(double radius, std::mt19937 &rng) {
    std::uniform_real_distribution<double> unit(0.0, 1.0);

    const double theta = unit(rng) * 2.0 * M_PI;
    const double phi = acos(2.0 * unit(rng) - 1.0);
    const double r = radius * cbrt(unit(rng));

    return PhysicalVector(
            (float) (r * sin(phi) * cos(theta)),
            (float) (r * sin(phi) * sin(theta)),
            (float) (r * cos(phi)),
            false);   // a displacement, not a point
}

namespace {

    PhysicalVector randomUnitVector(std::mt19937 &rng) {
        std::normal_distribution<double> gaussian(0.0, 1.0);
        PhysicalVector candidate(
                (float) gaussian(rng), (float) gaussian(rng), (float) gaussian(rng), false);
        return candidate.length() > 0 ? candidate.unit() : PhysicalVector(1, 0, 0, false);
    }

    /*! Random internal speed for one group, in m/s.
     *
     *  Either straight from `dispersion`, or solved from `virialRatio` when the
     *  group asked for a temperature instead of a velocity fraction. For a
     *  uniform sphere with isotropic speed d*sqrt(G*M/R), the internal kinetic
     *  energy is 0.5*d^2*G*M^2/R against a binding energy of 0.6*G*M^2/R, so
     *  the ratio comes out at 0.833*d^2 and inverting gives d = sqrt(1.2*q).
     */
    double internalSpeed(const GroupSpec &group) {
        if (group.radius <= 0 || group.mass <= 0) {
            return 0.0;
        }

        const double fraction = group.virialRatio > 0
                ? sqrt(1.2 * group.virialRatio)
                : group.dispersion;

        return fraction > 0 ? fraction * sqrt(G * group.mass / group.radius) : 0.0;
    }

    //! Angular velocity vector for a group, from its spin fraction.
    PhysicalVector angularVelocity(const GroupSpec &group) {
        const double fraction = group.spin.length();
        if (fraction <= 0 || group.radius <= 0 || group.mass <= 0) {
            return PhysicalVector(0, 0, 0, false);
        }

        // Break-up spin: centrifugal balance at the group's edge.
        const double breakup = sqrt(G * group.mass / pow(group.radius, 3));
        return group.spin.unit().scaledBy(fraction * breakup);
    }

}

/*! Analytic estimate rather than an O(n^2) sum, so start-up stays instant even
 *  at 60k particles. Each group contributes its own uniform-sphere binding
 *  energy, plus a point-mass term against every other group.
 */
double ScenarioBuilder::estimatePotentialEnergy(const ScenarioSpec &spec) {
    double energy = 0;

    for (size_t i = 0; i < spec.groups.size(); i++) {
        const GroupSpec &group = spec.groups[i];
        if (group.radius > 0) {
            energy += -0.6 * G * group.mass * group.mass / group.radius;
        }

        for (size_t j = i + 1; j < spec.groups.size(); j++) {
            const GroupSpec &other = spec.groups[j];
            const double distance = group.position.minus(other.position).length();

            // Soften once the groups overlap, where the point-mass term blows up.
            const double softened = std::max(distance, 0.5 * (group.radius + other.radius));
            if (softened > 0) {
                energy += -G * group.mass * other.mass / softened;
            }
        }
    }

    return energy;
}

ParticleList ScenarioBuilder::build(
        const ScenarioSpec &spec,
        double dtSeconds,
        SetupDiagnostics &diagnostics,
        std::mt19937 &rng) {

    const kilograms_per_cubic_meter_t density = AstronomicalValues::DENSITY_SUN;

    struct PlacedParticle {
        PhysicalVector position;
        PhysicalVector velocity;
        PhysicalVector color;
        double mass;
        size_t group;
    };

    std::vector<PlacedParticle> placed;
    placed.reserve(spec.totalParticles());

    for (size_t index = 0; index < spec.groups.size(); index++) {
        const GroupSpec &group = spec.groups[index];
        if (group.count <= 0 || group.mass <= 0) {
            continue;
        }

        const double particleMass = group.mass / group.count;
        const PhysicalVector omega = angularVelocity(group);

        // A group given only `dispersion` is placed hot, as it always was, and
        // takes part in the virial rescale below. A group that asked for a
        // `virialRatio` is placed cold and warmed up afterwards - see the note
        // on that loop.
        const double placedSpeed = group.virialRatio > 0 ? 0.0 : internalSpeed(group);

        for (int i = 0; i < group.count; i++) {
            const PhysicalVector offset = ScenarioBuilder::randomPointInSphere(group.radius, rng);

            PhysicalVector velocity = group.velocity.plus(omega.vectorProduct3(offset));
            if (placedSpeed > 0) {
                velocity = velocity.plus(randomUnitVector(rng).scaledBy(placedSpeed));
            }

            placed.push_back({group.position.plus(offset), velocity, group.color,
                              particleMass, index});
        }
    }

    // Centre of mass and bulk velocity of everything that was placed.
    double totalMass = 0;
    PhysicalVector centerOfMass(0, 0, 0, false);
    PhysicalVector totalMomentum(0, 0, 0, false);
    for (const auto &particle : placed) {
        totalMass += particle.mass;
        centerOfMass = centerOfMass.plus(particle.position.scaledBy(particle.mass));
        totalMomentum = totalMomentum.plus(particle.velocity.scaledBy(particle.mass));
    }
    if (totalMass <= 0) {
        return ParticleList();
    }
    centerOfMass = centerOfMass.scaledBy(1.0 / totalMass);
    const PhysicalVector systemVelocity = totalMomentum.scaledBy(1.0 / totalMass);

    auto kineticEnergyAboutCom = [&placed, &systemVelocity]() {
        double energy = 0;
        for (const auto &particle : placed) {
            const double speed = particle.velocity.minus(systemVelocity).length();
            energy += 0.5 * particle.mass * speed * speed;
        }
        return energy;
    };

    const double potentialEnergy = estimatePotentialEnergy(spec);
    const double kineticEnergy = kineticEnergyAboutCom();
    const double bindingEnergy = std::abs(potentialEnergy);

    diagnostics.virialRatioBefore = bindingEnergy > 0 ? kineticEnergy / bindingEnergy : 0;
    diagnostics.velocityScale = 1.0;

    // Rescale motion about the centre of mass to hit the requested virial ratio.
    if (spec.virialRatio > 0 && kineticEnergy > 0 && bindingEnergy > 0) {
        const double scale = sqrt(spec.virialRatio / diagnostics.virialRatioBefore);
        diagnostics.velocityScale = scale;
        for (auto &particle : placed) {
            particle.velocity = systemVelocity.plus(
                    particle.velocity.minus(systemVelocity).scaledBy(scale));
        }
    }

    /* Per-group temperatures go on after that rescale, deliberately.
     *
     * A group's virialRatio is a statement about that group against its own
     * gravity, so letting the system-wide rescale multiply it would quietly
     * break its meaning: ask for a stable cloud beside a cold clump, watch
     * both get multiplied by the same 0.4, and end up with two cold clumps -
     * which is exactly the effect this exists to produce. Setting it last
     * means a group is the temperature it asked for whatever the system around
     * it is doing, and virial_ratio_after reports the resulting total honestly
     * rather than the target.
     *
     * Groups using the older `dispersion` knob are already hot by this point
     * and are left alone, so every scenario written before this still builds
     * exactly the same way.
     */
    for (auto &particle : placed) {
        const GroupSpec &group = spec.groups[particle.group];
        if (group.virialRatio <= 0) {
            continue;
        }

        const double speed = internalSpeed(group);
        if (speed > 0) {
            particle.velocity = particle.velocity.plus(
                    randomUnitVector(rng).scaledBy(speed));
        }
    }

    ParticleList particles;
    double maxDistance = 0;
    for (const auto &particle : placed) {
        const kilogram_t mass = kilogram_t(particle.mass);
        particles.addShapeToList(make_shared<Particle>(
                particle.position,
                mass,
                particle.velocity.scaledBy(particle.mass),
                density,
                particle.color));

        maxDistance = std::max(maxDistance, (double) particle.position.minus(centerOfMass).length());
    }

    diagnostics.particleCount = (int) particles.size();
    diagnostics.totalMass = totalMass;
    diagnostics.potentialEnergy = potentialEnergy;
    diagnostics.kineticEnergy = kineticEnergyAboutCom();
    diagnostics.virialRatioAfter = bindingEnergy > 0 ? diagnostics.kineticEnergy / bindingEnergy : 0;
    diagnostics.centerOfMass = centerOfMass;
    diagnostics.systemRadius = maxDistance;
    diagnostics.characteristicVelocity = maxDistance > 0 ? sqrt(G * totalMass / maxDistance) : 0;
    diagnostics.crossingTimeSeconds = diagnostics.characteristicVelocity > 0
            ? maxDistance / diagnostics.characteristicVelocity
            : 0;
    diagnostics.applyDt(dtSeconds);

    return particles;
}

void SetupDiagnostics::applyDt(double dtSeconds) {
    stepsPerCrossing = dtSeconds > 0 ? crossingTimeSeconds / dtSeconds : 0;
    secondsOfVideoPerCrossing = stepsPerCrossing / 24.0;
}

std::string SetupDiagnostics::toJson() const {
    std::ostringstream json;
    json << std::setprecision(6);
    json << "{"
         << "\"particle_count\": " << particleCount
         << ", \"total_mass_kg\": " << totalMass
         << ", \"kinetic_energy\": " << kineticEnergy
         << ", \"potential_energy\": " << potentialEnergy
         << ", \"virial_ratio_before\": " << virialRatioBefore
         << ", \"virial_ratio_after\": " << virialRatioAfter
         << ", \"velocity_scale\": " << velocityScale
         << ", \"system_radius_m\": " << systemRadius
         << ", \"characteristic_velocity_mps\": " << characteristicVelocity
         << ", \"crossing_time_s\": " << crossingTimeSeconds
         << ", \"steps_per_crossing\": " << stepsPerCrossing
         << ", \"video_seconds_per_crossing\": " << secondsOfVideoPerCrossing
         << ", \"center_of_mass\": [" << centerOfMass.x() << ", " << centerOfMass.y() << ", " << centerOfMass.z() << "]"
         << "}";
    return json.str();
}
