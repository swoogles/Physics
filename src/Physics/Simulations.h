#ifndef SIMULATIONS_H_
#define SIMULATIONS_H_

#include "Simulation.h"
#include "PhysicsSandboxProperties.h"
#include "ParticleGroupProperties.h"
#include "CraftedSimulation.h"

class Simulations {
public:

    Simulation QuadrantTesting_simplest();

    Simulation bodyFormation_ArbitraryList(int numPieces, PhysicsSandboxProperties properties);

    Simulation bodyFormation(int numPieces, PhysicsSandboxProperties properties);
    Simulation bodyFormationCollision(PhysicsSandboxProperties properties);

    Simulation createSimulation(CraftedSimulation simulation, PhysicsSandboxProperties simulationProperties);

private:
    ParticleList manipulatedGroup(ParticleGroupProperties properties, PhysicalVector origin, PhysicalVector momentum);

};

#endif
