#ifndef SIMULATIONS_H_
#define SIMULATIONS_H_

#include "ShapeFiles/Particle.h"
#include "Simulation.h"
#include "CollisionType.h"
#include "ForceCalculationMethod.h"
#include "AstronomicalValues.h"
#include "PhysicsSandboxProperties.h"

typedef unique_ptr<Simulation> SimulationPointer_t;

class Simulations {
public:
    typedef unique_ptr<Simulation> SimulationPointer_t;
    static SimulationPointer_t disruption_ArbitraryList(PhysicsSandboxProperties properties);

    static SimulationPointer_t multipleOrbiters(PhysicsSandboxProperties properties);

    static SimulationPointer_t QuadrantTesting_simplest(ForceCalculationMethod forceCalculationMethod);

    static SimulationPointer_t bodyFormation_ArbitraryList(int numPieces, PhysicsSandboxProperties properties);

    static SimulationPointer_t bodyFormationGeneric_ArbitraryList(
            PhysicsSandboxProperties properties,
            PhysicalVector target,
            PhysicalVector groupMomentum);

    static SimulationPointer_t createSimulation(char simNumber, PhysicsSandboxProperties simulationProperties);

    static PhysicalVector randomPointInSphere(double maxDistance, PhysicalVector target);
  private:

    /*! \relates MyShape
     *  \brief Gets a random starting position vector based on the second parameter
     *
     *  This function currently uses pieceRadius as the determining factor for the bounding box; this should be changed to an independent bounding variable
     *  \param pieceRadius Determines size of bounding box
     */
    static PhysicalVector randomSplitBodyPlacement(float pieceRadius, PhysicalVector target);

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
