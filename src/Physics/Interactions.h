/*
 * Interactions.h
 *
 *  Created on: Jul 21, 2011
 *      Author: brasure
 */

#ifndef INTERACTIONS_H_
#define INTERACTIONS_H_

#include <iostream>
#include <fstream>
#include <time.h>


#include <boost/numeric/ublas/vector.hpp>
#include <plib/sg.h>
#include "../ShapeFiles/Circle.h"
#include "../ShapeFiles/Box.h"
#include "../ShapeFiles/ShapeList.h"
#include "../Parallelization/Quadrant.h"

#include "WorldSettings.h"
#include "Simulations.h"

using namespace std;

typedef boost::shared_ptr<MyShape> shape_pointer;

//void inelasticCollisions(float dt);
//void elasticCollisions(float dt);

/*! \relates MyShape
 *  \brief Calculates new velocities after a collision
 *
 *  This function is called when 2 objects are determined to be too close to eachother, and either WorldSettings::isAllElastic() returns true or
 *  both objects have their elastic bools on.
 *
 */
void elasticCollision(boost::shared_ptr<MyShape> object1, boost::shared_ptr<MyShape> object2);

void calcForceOnObject_Octree(shape_pointer curObject, boost::shared_ptr<Quadrant> curQuadrant, float dt);


/*! \relates MyShape
 *  \brief Calculates forces on all current objects
 *
 *  Loops through the shapes vector, calculating all currently active forces on and between each object. Can include drag, a constant gravity field,  and
 *  gravity between objects.
 *
 *  \param dt Determines time over which each force acts
 */
void calcForcesAll_ArbitraryList(boost::numeric::ublas::vector<shape_pointer> physicalObjects, float dt);

/*! \relates MyShape
 *  \brief Calculates forces on all current objects
 *
 *  Loops through the shapes vector, calculating all currently active forces on and between each object. Can include drag, a constant gravity field,  and
 *  gravity between objects.
 *
 *  \param dt Determines time over which each force acts
 */
void calcForcesAll_ArbitraryListWithOctree(boost::numeric::ublas::vector<shape_pointer> physicalObjects, boost::shared_ptr<Quadrant> octree, float dt);


/*! \relates MyShape
 *  \brief Determines if any objects are colliding and responds appropriately.
 *
 *  The action taken when collisions are detected depends on the values active in WorldSettings. Can be elastic, inelastic, or anywhere in between(TODO)
 */
void calcCollisionsAll();

/*! \relates MyShape
 *  \brief Determines if any objects are colliding and responds appropriately.
 *
 *  The action taken when collisions are detected depends on the values active in WorldSettings. Can be elastic, inelastic, or anywhere in between(TODO)
 */
void calcCollisionsAll_ArbitraryList(boost::numeric::ublas::vector<shape_pointer> physicalObjects);

/*! \relates MyShape
 *  \brief Determines if any objects are colliding and responds appropriately.
 *
 *  The action taken when collisions are detected depends on the values active in WorldSettings. Can be elastic, inelastic, or anywhere in between(TODO)
 */
void calcCollisionsAll_ShapeList( ShapeList & physicalObjects);



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
void getVectorToObject2(boost::shared_ptr<MyShape> object1, boost::shared_ptr<MyShape> object2, sgVec4 sepVector);


/*! \relates MyShape
 *  \brief Determines the final angular momentum after 2 objects collide in a completely inelastic collision
 *
 *  The final angular momentum is determined with the following formula:
 *  \n v1i = the initial velocity of object 1
 *  \n v2i = the initial velocity of object 2
 *	\n m1 = the mass of object 1
 *	\n m2 = the mass of object 2
 *	\n e = the coefficient of restitution (e = 1 for elastic collision)
 *	\n n = normal unit vector drawn from object 1 to object 2
 *	\n c = n . (v1i - v2i)
 *	\n v1f = v1i - ((m2c)/(m1 + m2))(1 + e)n
 *	\n v2f = v2i + ((m1c)/(m1 + m2))(1 + e)n
 *
 *	\return The angular momentum to be assigned to the merged object
 */
void calcMergedAngMomentum(boost::shared_ptr<MyShape> object1, boost::shared_ptr<MyShape> object2, sgVec4 retAngMomentum);


/*! \relates Circle
 *  \brief Finds radius after 2 circular objects are merged
 *
 */
float calcMergedRadius(float massBoth, float density);


/*! \relates MyShape
 *  \brief Combines 2 distinct objects in an inelastic collision into the first object and eliminates the second
 *
 *  Combines 2 objects by:
 *  \n -Calculating their combined radius (calcMergedRadius)
 *  \n -Finding their center of mass and centering the new object there
 *  \n -Adding their masses and momentums
 *  \n -Calculating their combined angular momentum about the COM (calcMergedAngMomentum)
 *
 *  Note: This function does NOT delete the second object, currently that must be handled outside
 */
void mergeObjects(boost::shared_ptr<MyShape> object1, boost::shared_ptr<MyShape> object2);


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
 *  \brief Returns true if objects are touching eachother
 *
 */
bool contact(boost::shared_ptr<MyShape> object1, boost::shared_ptr<MyShape> object2);

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
bool isConflict(int newShape);

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
bool isConflict_ArbitraryList(boost::numeric::ublas::vector<shape_pointer> physiaclObjects, int newShape);


/*! \relates MyShape
 *  \brief Will eventually calculate drag force on a single object
 */
void calcDrag(float dt);


//TODO find good place for this function
/*! \relates sgVec
 *  \brief Easy outputting for Display/Debugging
 */
ostream & operator<<(ostream& os, sgVec4 outputVec);


#endif /* INTERACTIONS_H_ */
