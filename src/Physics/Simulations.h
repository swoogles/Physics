/*
 * Simulations.h
 *
 *  Created on: Jul 22, 2011
 *      Author: brasure
 */

#ifndef SIMULATIONS_H_
#define SIMULATIONS_H_

#include <iostream>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/ref.hpp>
#include <plib/sg.h>
//#include "../ShapeFiles/MyShape.h"
#include "../ShapeFiles/Circle.h"
#include "../ShapeFiles/Box.h"
#include "../ShapeFiles/ShapeList.h"
#include "../Parallelization/Quadrant.h"
#include "Interactions.h"

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

SGfloat massList[NUM_BODIES] = {

};
*/

using namespace boost::numeric::ublas;
using boost::numeric::ublas::vector;

class Simulations
{
  private:
    // Eg. Observers::getObserverFromList, Quadrant
    /*! \brief Vector that holds all currently active shapes
     *
     *  One of the biggest decisions still to be made is how/if to alter this to make it less scary
     */
    typedef boost::shared_ptr<MyShape> shape_pointer;
    static boost::numeric::ublas::vector< shape_pointer > physicalObjects;


  public:
    static void largeGridAlternating();

    static void simpleOrbit();

    static ShapeList disruption_ArbitraryList();

    static ShapeList bodyFormation_ArbitraryList( int numPieces );

    static ShapeList bodyFormationGeneric_ArbitraryList( int numPieces, sgVec4 target, sgVec4 groupMomentum );

    static ShapeList billiardsReturningList(int);

    //VERY EXPERIMENTAL
    static ShapeList billiards2_ArbitraryList(int);

    static void billiards3(int);
    static ShapeList billiards3_ArbitraryList(int);

    static boost::shared_ptr<Quadrant> octreeDemonstration(int);

    static ShapeList simpleCollision_ArbitraryList();

    /*! \brief Returns a shape from the static list of objects that should be considered during
     * physical interactions.
     *
     *  /param shapeIndex The index of the shape you want to retrieve from the main shapes list
     *  \return Pointer to desired shape
     */
    static shape_pointer getShapeFromList( unsigned int shapeIndex );

    /*! \brief Returns a shape from the main shapes list
     *
     *  /param shapeIndex The index of the shape you want to retrieve from the main shapes list
     *  \return Pointer to desired shape
     */
    static int addShapeToList( shape_pointer insertShape );
};


#endif /* SIMULATIONS_H_ */
