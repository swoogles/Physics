#ifndef SIMULATIONS_H_
#define SIMULATIONS_H_

#include "ShapeFiles/Particle.h"
#include "Simulation.h"
#include "CollisionType.h"
#include "AstronomicalValues.h"
#include "PhysicsSandboxProperties.h"
#include "CraftedSimulation.h"

class Simulations {
public:

    Simulation QuadrantTesting_simplest();

    Simulation bodyFormation_ArbitraryList(int numPieces, PhysicsSandboxProperties properties);

    Simulation bodyFormation(int numPieces, PhysicsSandboxProperties properties);

    Simulation createSimulation(CraftedSimulation simulation, PhysicsSandboxProperties simulationProperties);

    PhysicalVector randomPointInSphere(double maxDistance, PhysicalVector target);
  private:
    ParticleList bodyPlacement(int numPieces, PhysicsSandboxProperties properties, PhysicalVector origin);

    /*! \relates MyShape
     *  \brief Gets a random starting momentum vector based on second parameter
     *
     *  This function currently uses pieceMass as the determining factor for momentum range; figure out a better basis
     *
     *  \param pieceMass Determines magnitude of momentum
     */
    PhysicalVector randomSplitBodyMomentum(kilogram_t pieceMass);

};

#endif
