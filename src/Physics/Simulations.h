/*
 * Simulations.h
 *
 *  Created on: Jul 22, 2011
 *      Author: brasure
 */

#ifndef SIMULATIONS_H_
#define SIMULATIONS_H_

#include "../Parallelization/Quadrant.h"
#include "octree.h"
#include <iostream>
#include <boost/numeric/ublas/vector.hpp>
#include <plib/sg.h>
//#include "../ShapeFiles/MyShape.h"
#include "../ShapeFiles/Circle.h"
#include "../ShapeFiles/Box.h"
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


void simpleOrbit();

void largeGridAlternating();

void disruption();

void bodyFormation( unsigned int numPieces );
void bodyFormationGeneric( unsigned int numPieces, sgVec4 target, sgVec4 groupMomentum );

void billiards(int);

void billiards2(int);

void billiards3(int);

Quadrant * octreeDemonstration(int);

void simpleCollision();

#endif /* SIMULATIONS_H_ */
