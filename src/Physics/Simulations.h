#ifndef SIMULATIONS_H_
#define SIMULATIONS_H_

#include "Simulation.h"
#include "PhysicsSandboxProperties.h"
#include "ParticleGroupProperties.h"
#include "ScenarioBuilder.h"
#include "ScenarioSpec.h"

#include <random>

class Simulations {
public:
    /*! \brief Builds a simulation from a data-defined scenario.
     *
     *  This is the only path into a real run: nothing about the starting
     *  arrangement is compiled in, so new configurations are new text files
     *  rather than new code.
     */
    static Simulation fromScenario(
            const ScenarioSpec &scenario,
            PhysicsSandboxProperties &properties,
            SetupDiagnostics &diagnostics,
            std::mt19937 &rng);

    //! Four particles in a fixed arrangement, used by the octree tests.
    static Simulation QuadrantTesting_simplest();
};

#endif
