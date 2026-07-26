//
// Covers the pieces the batch workflow depends on: a run must be reproducible
// from its seed, and the knobs in a properties file must mean what they say.
//

#include "catch.hpp"

#include <cmath>
#include <cstdio>
#include <fstream>
#include <random>

#include "../src/BillProperties.h"
#include "../src/Physics/ScenarioBuilder.h"
#include "../src/Physics/ScenarioSpec.h"

namespace {

    const char *SCRATCH_CONFIG = "scenario_test_generated.properties";

    void writeConfig(const std::string &extra) {
        std::ofstream file(SCRATCH_CONFIG);
        file << "numShapes=200\n"
             << "dt=1000\n"
             << "octree_theta=0.5\n"
             << "sandbox_width=1.0e6\n"
             << "mass=598910\n"
             << "maximum_runtime=1\n"
             << extra;
    }

    ScenarioSpec parseGenerated(const std::string &extra, unsigned int seed = 99) {
        writeConfig(extra);
        PhysicsSandboxProperties properties(SCRATCH_CONFIG);
        return ScenarioParser::parse(properties.raw(), properties, seed);
    }

}

TEST_CASE("Properties are trimmed and defaulted", "[scenario]") {
    writeConfig("  spaced.key  =  spaced value  \n# comment=ignored\n");
    BillProperties properties(SCRATCH_CONFIG);

    REQUIRE(properties.get("spaced.key", "") == "spaced value");
    REQUIRE(properties.has("spaced.key"));
    REQUIRE_FALSE(properties.has("comment"));
    REQUIRE(properties.get("absent.key", "fallback") == "fallback");

    remove(SCRATCH_CONFIG);
}

TEST_CASE("Explicit groups are read straight out of the config", "[scenario]") {
    ScenarioSpec spec = parseGenerated(
            "scenario.type=explicit\n"
            "group.0=label=core; pos=1e6,2e6,0; vel=3,4,0; n=500; mass=1e9; radius=2e6; spin=0.5; color=1,0,0\n"
            "group.1=pos=-1e6,0,0; n=250\n");

    REQUIRE(spec.groups.size() == 2);

    const GroupSpec &first = spec.groups[0];
    REQUIRE(first.label == "core");
    REQUIRE(first.count == 500);
    REQUIRE(first.mass == Approx(1e9));
    REQUIRE(first.radius == Approx(2e6));
    REQUIRE(first.position.x() == Approx(1e6));
    REQUIRE(first.velocity.y() == Approx(4));
    REQUIRE(first.spin.z() == Approx(0.5));
    REQUIRE(first.color.x() == Approx(1));

    // Unspecified fields fall back to the file-level defaults.
    REQUIRE(spec.groups[1].count == 250);
    REQUIRE(spec.groups[1].radius == Approx(1.0e6));

    remove(SCRATCH_CONFIG);
}

TEST_CASE("A scalar spin means rotation about z", "[scenario]") {
    PhysicalVector spin = ScenarioParser::parseVector("0.4", PhysicalVector(0, 0, 0, false));
    REQUIRE(spin.x() == Approx(0));
    REQUIRE(spin.y() == Approx(0));
    REQUIRE(spin.z() == Approx(0.4));
}

TEST_CASE("The same seed rebuilds the identical arrangement", "[scenario]") {
    ScenarioSpec spec = parseGenerated("scenario.type=cluster\ncluster.count=300\ncluster.spin=0.3\n");

    SetupDiagnostics firstDiagnostics;
    std::mt19937 firstRng(1234);
    ParticleList first = ScenarioBuilder::build(spec, 1000, firstDiagnostics, firstRng);

    SetupDiagnostics secondDiagnostics;
    std::mt19937 secondRng(1234);
    ParticleList second = ScenarioBuilder::build(spec, 1000, secondDiagnostics, secondRng);

    REQUIRE(first.size() == second.size());
    REQUIRE(firstDiagnostics.kineticEnergy == Approx(secondDiagnostics.kineticEnergy));
    REQUIRE(firstDiagnostics.systemRadius == Approx(secondDiagnostics.systemRadius));

    std::vector<PhysicalVector> firstPositions;
    first.checkForAllParticles([&](const Particle &particle) {
        firstPositions.push_back(particle.position());
    });

    size_t index = 0;
    second.checkForAllParticles([&](const Particle &particle) {
        REQUIRE(particle.position() == firstPositions.at(index));
        index++;
    });

    // A different seed must not produce the same arrangement.
    SetupDiagnostics otherDiagnostics;
    std::mt19937 otherRng(4321);
    ParticleList other = ScenarioBuilder::build(spec, 1000, otherDiagnostics, otherRng);
    bool sawDifference = false;
    index = 0;
    other.checkForAllParticles([&](const Particle &particle) {
        if (!(particle.position() == firstPositions.at(index))) {
            sawDifference = true;
        }
        index++;
    });
    REQUIRE(sawDifference);

    remove(SCRATCH_CONFIG);
}

TEST_CASE("Velocities are rescaled to the requested virial ratio", "[scenario]") {
    ScenarioSpec spec = parseGenerated(
            "scenario.type=cluster\n"
            "scenario.virial_ratio=0.25\n"
            "cluster.count=400\n"
            "cluster.dispersion=0.3\n");

    SetupDiagnostics diagnostics;
    std::mt19937 rng(7);
    ParticleList particles = ScenarioBuilder::build(spec, 1000, diagnostics, rng);

    REQUIRE(particles.size() == 400);
    REQUIRE(diagnostics.virialRatioAfter == Approx(0.25).epsilon(0.02));
    REQUIRE(diagnostics.potentialEnergy < 0);
    REQUIRE(diagnostics.velocityScale > 0);

    remove(SCRATCH_CONFIG);
}

TEST_CASE("Particles stay inside the sphere they were placed in", "[scenario]") {
    std::mt19937 rng(11);
    for (int i = 0; i < 500; i++) {
        PhysicalVector point = ScenarioBuilder::randomPointInSphere(1000.0, rng);
        REQUIRE(point.length() <= Approx(1000.0));
    }
}

TEST_CASE("A pair scenario is centred and separated as asked", "[scenario]") {
    ScenarioSpec spec = parseGenerated(
            "scenario.type=pair\n"
            "pair.separation=1.0e7\n"
            "pair.impact_parameter=0.0\n"
            "pair.mass_ratio=3.0\n");

    REQUIRE(spec.groups.size() == 2);

    const double separation = spec.groups[0].position.minus(spec.groups[1].position).length();
    REQUIRE(separation == Approx(1.0e7).epsilon(0.01));

    // The heavier group carries three times the mass and moves proportionally slower.
    REQUIRE(spec.groups[0].mass == Approx(3.0 * spec.groups[1].mass).epsilon(0.01));
    REQUIRE(fabs(spec.groups[0].velocity.x()) < fabs(spec.groups[1].velocity.x()));

    remove(SCRATCH_CONFIG);
}
