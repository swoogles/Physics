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

    static Simulation QuadrantTesting_simplest();

    static Simulation bodyFormation_ArbitraryList(int numPieces, PhysicsSandboxProperties properties);

    static Simulation bodyFormation(int numPieces, PhysicsSandboxProperties properties);

    static Simulation createSimulation(CraftedSimulation simulation, PhysicsSandboxProperties simulationProperties);

    static PhysicalVector randomPointInSphere(double maxDistance, PhysicalVector target);
  private:

    /*! \relates MyShape
     *  \brief Gets a random starting momentum vector based on second parameter
     *
     *  This function currently uses pieceMass as the determining factor for momentum range; figure out a better basis
     *
     *  \param pieceMass Determines magnitude of momentum
     */
    static PhysicalVector randomSplitBodyMomentum(kilogram_t pieceMass);

};

#endif
