#ifndef INTERACTIONS_H_
#define INTERACTIONS_H_

#include <plib/sg.h>

#include "../Parallelization/Quadrant.h"
#include "../ShapeFiles/ShapeList.h"
#include "Simulations.h"

using boost::shared_ptr;
using boost::numeric::ublas::compressed_vector;

/*! \brief Calculates new velocities after a collision
 *
 *  This function is called when 2 objects are determined to be too close to eachother, and either WorldSettings::isAllElastic() returns true or
 *  both objects have their elastic bools on.
 *
 */
void elasticCollision( shape_pointer object1, shape_pointer object2, float dt);

ShapeList calcForceOnObject_Octree( shape_pointer curObject, boost::shared_ptr<Quadrant> curQuadrant, float dt);

void calcForcesAll( boost::shared_ptr<Simulation> curSimulation );

/*! \brief Calculates forces on all current objects
 *
 *  Loops through the shapes vector, calculating all currently active forces on and between each object. Can include drag, a constant gravity field,  and
 *  gravity between objects.
 *
 *  \param dt Determines time over which each force acts
 */
void calcForcesAll_LessNaive( boost::shared_ptr<Simulation> curSimulation );

/*! \brief Determines if any objects are colliding and responds appropriately.
 *
 *  The action taken when collisions are detected depends on the values active in WorldSettings. Can be elastic, inelastic, or anywhere in between(TODO)
 */
void calcCollisionsAll(boost::shared_ptr<Simulation> curSimulation);

/*! \brief Calculates magnitude of the force of gravity between 2 objects
 *
 */
void calcForceGrav( sgVec4 gravVec, shape_pointer object1, shape_pointer object2, float dt ) ;

/*! \brief Will eventually calculate drag force on a single object
 */
void calcDrag(float dt);

#endif /* INTERACTIONS_H_ */
