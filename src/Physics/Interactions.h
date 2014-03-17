/*
 * Interactions.h
 *
 *  Created on: Jul 21, 2011
 *      Author: brasure
 */

#ifndef INTERACTIONS_H_
#define INTERACTIONS_H_

#include <boost/numeric/ublas/vector_sparse.hpp>
#include <iostream>
#include <plib/sg.h>

#include "../Parallelization/Quadrant.h"
#include "../ShapeFiles/Box.h"
#include "../ShapeFiles/Circle.h"
#include "../ShapeFiles/ShapeList.h"
#include "../Observation/Timer.h"
#include "Simulations.h"

using namespace std;

using boost::shared_ptr;
using boost::numeric::ublas::compressed_vector;
typedef boost::shared_ptr<MyShape> shape_pointer;

/*! \relates MyShape
 *  \brief Calculates new velocities after a collision
 *
 *  This function is called when 2 objects are determined to be too close to eachother, and either WorldSettings::isAllElastic() returns true or
 *  both objects have their elastic bools on.
 *
 */
void elasticCollision( boost::shared_ptr<MyShape> object1, boost::shared_ptr<MyShape> object2, float dt);

void calcForceOnObject_Octree( shape_pointer curObject, boost::shared_ptr<Quadrant> curQuadrant, float dt);


void calcForcesAll( boost::shared_ptr<Simulation> curSimulation );

/*! \relates MyShape
 *  \brief Calculates forces on all current objects
 *
 *  Loops through the shapes vector, calculating all currently active forces on and between each object. Can include drag, a constant gravity field,  and
 *  gravity between objects.
 *
 *  \param dt Determines time over which each force acts
 */
void calcForcesAll_Naive( boost::shared_ptr<Simulation> curSimulation );

/*! \relates MyShape
 *  \brief Determines if any objects are colliding and responds appropriately.
 *
 *  The action taken when collisions are detected depends on the values active in WorldSettings. Can be elastic, inelastic, or anywhere in between(TODO)
 */
void calcCollisionsAll(boost::shared_ptr<Simulation> curSimulation);

/*! \relates MyShape
 *  \brief Calculates magnitude of the force of gravity between 2 objects
 *
 *
 */
float calcForceGrav(boost::shared_ptr<MyShape> object1, boost::shared_ptr<MyShape> object2, SGfloat rSquared);

/*! \relates MyShape
 *  \brief Finds a vector pointing from object1 to object2
 *
 *  \param object1 Start Object
 *  \param object2 End Object
 *  \param sepVector Calculated separation vector
 */
void getVectorToQuadrant(boost::shared_ptr<MyShape> object1, boost::shared_ptr<Quadrant> quadrant, sgVec4 sepVector);

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
float getSplitBodyRadius(float volume, int numPieces );


/*! \relates MyShape
 *  \brief Gets a random starting position vector based on the second parameter
 *
 *  This function currently uses pieceRadius as the determining factor for the bounding box; this should be changed to an independent bounding variable
 *  \param startPos Random starting position is returned here
 *  \param pieceRadius Determines size of bounding box
 */
void randomSplitBodyPlacement(sgVec4 startPos, float pieceRadius, sgVec4 target);

/*! \relates MyShape
 *  \brief Gets a random starting position vector inside the volume vector
 *
 *  \param startPos Random starting position is returned here
 *  \param volume Determines size of bounding box
 */
void randomSplitBodyPlacementInZone(sgVec4 startPos, sgVec4 volume, sgVec4 target );


/*! \relates MyShape
 *  \brief Gets a random starting momentum vector based on second parameter
 *
 *  This function currently uses pieceMass as the determining factor for momentum range; figure out a better basis
 *
 *  \param startMom Random starting momentum is returned here
 *  \param pieceMass Determines magnitude of momentum
 */
void randomSplitBodyMomentum(sgVec4 startMom, float pieceMass);

/*! \relates MyShape
 *  \brief Returns true if new shape is placed on top of a previously created shape
 *
 *  Loops through shapes vector up until newShape, seeing if any shapes overlap with the newly-placed one. Only necessary when dealing with elastic collisions;
 *  otherwise, the new object will just be merged with the existing one.
 *
 *  \param newShape Index of newshape in shapes vector
 *
 *  \returns True if new object causes a conflict
 */
bool isConflict_ArbitraryList(compressed_vector<shape_pointer> physiaclObjects, shape_pointer newShape);


/*! \relates MyShape
 *  \brief Will eventually calculate drag force on a single object
 */
void calcDrag(float dt);

#endif /* INTERACTIONS_H_ */
