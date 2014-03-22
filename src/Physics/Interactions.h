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
 */
void calcForceGrav( sgVec4 gravVec, boost::shared_ptr<MyShape> object1, boost::shared_ptr<MyShape> object2, float dt ) ;

/*! \relates MyShape
 *  \brief Will eventually calculate drag force on a single object
 */
void calcDrag(float dt);

#endif /* INTERACTIONS_H_ */
