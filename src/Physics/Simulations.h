#ifndef SIMULATIONS_H_
#define SIMULATIONS_H_

#include "ShapeFiles/Particle.h"
#include "../ShapeFiles/ShapeList.h"
#include "Simulation.h"
#include "CollisionType.h"
#include "ForceCalculationMethod.h"
#include "AstronomicalValues.h"
#include "PhysicsSandboxProperties.h"

#define MASS_VAR 1e21
//#define MASS_SUN 1989100000

//#define MASS_LARGEST_STAR
#define MASS_JUPITER 1898600
#define MASS_SATURN 568460
#define MASS_URANUS 86832
#define MASS_NEPTUNE 102430
#define MASS_EARTH 5973.6
#define MASS_VENUS 4868.5
#define MASS_MARS 641.84
#define MASS_MERCURY 330

//Volume = MASS/(DENSITY*CONVERSION_CONST)
#define CONVERSION_CONST 1e12

#define DENSITY_JUPITER 1.33
#define DENSITY_SATURN 0.70
#define DENSITY_URANUS 1.30
#define DENSITY_NEPTUNE 1.76
#define DENSITY_EARTH 5.515
#define DENSITY_VENUS 5.24
#define DENSITY_MARS 3.94
#define DENSITY_MERCURY 5.43

#define VOLUME_VAR 1e9


/*
enum bodies {
	SUN,
	JUPITER,
	SATURN,
	URANUS,
	NEPTUNE,
	EARTH,
	VENUS,
	MARS,
	MERCURY,
	LAST_BODY
};

int NUM_BODIES = LAST_BODY+1;

};
*/

typedef unique_ptr<Simulation> SimulationPointer_t;

class Simulations
{
  private:

    /*! \relates MyShape
     *  \brief Gets a random starting position vector based on the second parameter
     *
     *  This function currently uses pieceRadius as the determining factor for the bounding box; this should be changed to an independent bounding variable
     *  \param pieceRadius Determines size of bounding box
     */
    static VecStruct randomSplitBodyPlacement(float pieceRadius, VecStruct target);

    /*! \relates MyShape
     *  \brief Gets a random starting momentum vector based on second parameter
     *
     *  This function currently uses pieceMass as the determining factor for momentum range; figure out a better basis
     *
     *  \param pieceMass Determines magnitude of momentum
     */
    static VecStruct randomSplitBodyMomentum(kilogram_t pieceMass);

    typedef unique_ptr<Simulation> SimulationPointer_t;
  public:
    // TODO put this in a more appropriate "Physical Constants" location
    static void simpleOrbit();

    static SimulationPointer_t
    disruption_ArbitraryList(PhysicsSandboxProperties properties);

    static SimulationPointer_t QuadrantTesting_simplest(ForceCalculationMethod forceCalculationMethod);

    static SimulationPointer_t
    bodyFormation_ArbitraryList(int numPieces, PhysicsSandboxProperties properties);

    static SimulationPointer_t bodyFormationGeneric_ArbitraryList(PhysicsSandboxProperties properties, VecStruct target,
                                                                  VecStruct groupMomentum);

    static SimulationPointer_t billiards1(int, ForceCalculationMethod forceCalculationMethod);

    static SimulationPointer_t billiards2_ReturnSimulation(int, ForceCalculationMethod forceCalculationMethod);

    static SimulationPointer_t billiards3_ArbitraryList(int, ForceCalculationMethod forceCalculationMethod);

    static SimulationPointer_t
    createSimulation(char simNumber, PhysicsSandboxProperties simulationProperties);

    static VecStruct randomPointInSphere(double maxDistance, VecStruct target);

};


#endif /* SIMULATIONS_H_ */
