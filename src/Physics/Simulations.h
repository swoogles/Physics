#ifndef SIMULATIONS_H_
#define SIMULATIONS_H_

#include <plib/sg.h>
#include "../ShapeFiles/Circle.h"
#include "../ShapeFiles/ShapeList.h"
#include "Simulation.h"
#include "CollisionType.h"
#include "ForceCalculationMethod.h"

#define MASS_VAR 1e21
//#define MASS_SUN 1989100000

#define MASS_SUN 598910000
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

#define DENSITY_SUN 1.409
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

typedef shared_ptr<Simulation> SimulationPointer_t;

class Simulations
{
  private:
    /*! \relates Circle
     * \brief SplitRadius!
     *
     * Given a volume and number of pieces for it to be split into, this function calculates the radius needed for the volume of the individual pieces to add up
     * to the original volume.
     * \param volume Total volume to be split
     * \param numPieces Number of desired pieces
     *
     * \return Radius for the smaller sub-pieces
     */
    static float getSplitBodyRadius(float volume, int numPieces );

    /*! \relates MyShape
     *  \brief Gets a random starting position vector based on the second parameter
     *
     *  This function currently uses pieceRadius as the determining factor for the bounding box; this should be changed to an independent bounding variable
     *  \param startPos Random starting position is returned here
     *  \param pieceRadius Determines size of bounding box
     */
    static void randomSplitBodyPlacement(sgVec4 startPos, float pieceRadius, sgVec4 target);

    /*! \relates MyShape
     *  \brief Gets a random starting position vector inside the volume vector
     *
     *  \param startPos Random starting position is returned here
     *  \param volume Determines size of bounding box
     */
    static void randomSplitBodyPlacementInZone(sgVec4 startPos, sgVec4 volume, sgVec4 target );

    /*! \relates MyShape
     *  \brief Gets a random starting momentum vector based on second parameter
     *
     *  This function currently uses pieceMass as the determining factor for momentum range; figure out a better basis
     *
     *  \param startMom Random starting momentum is returned here
     *  \param pieceMass Determines magnitude of momentum
     */
    static void randomSplitBodyMomentum(sgVec4 startMom, float pieceMass);

    typedef shared_ptr<Simulation> SimulationPointer_t;
  public:
    // TODO put this in a more appropriate "Physical Constants" location
    static float G;

    static void largeGridAlternating();

    static void simpleOrbit();

    static Simulation disruption_ArbitraryList(ForceCalculationMethod forceCalculationMethod);

    static Simulation bodyFormation_NonRandom(ForceCalculationMethod forceCalculationMethod);
    static Simulation QuadrantTestingNonRandom(ForceCalculationMethod forceCalculationMethod);
    static Simulation QuadrantTesting_simplest(ForceCalculationMethod forceCalculationMethod);
    static Simulation QuadrantTesting_simplest_move(ForceCalculationMethod forceCalculationMethod);

    static Simulation
    bodyFormation_ArbitraryList(int numPieces, ForceCalculationMethod forceCalculationMethod);

    static Simulation bodyFormationGeneric_ArbitraryList(int numPieces, float *target, float *groupMomentum,
                                                         ForceCalculationMethod forceCalculationMethod);

    static Simulation billiards1(int numPieces, ForceCalculationMethod forceCalculationMethod);

    static Simulation billiards2_ReturnSimulation(int numShapes, ForceCalculationMethod forceCalculationMethod);

    static Simulation billiards3_ArbitraryList(int numShapes, ForceCalculationMethod forceCalculationMethod);

    static Simulation createSimulation(char simNumber, int numShapes, ForceCalculationMethod forceCalculationMethod);
};


#endif /* SIMULATIONS_H_ */
