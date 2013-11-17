/*
 * Simulation.cpp
 *
 *  Created on: Jul 22, 2011
 *      Author: bfrasure
 */

#include "Simulation.h"

const char Simulation::FORCE_CALC_METHOD_OCTREE_STRING[] = "octree";
const char Simulation::FORCE_CALC_METHOD_NAIVE_STRING[] = "naive";

Simulation::Simulation()
            :forceCalcMethod(FORCE_CALC_METHOD_OCTREE)
{
  // forceCalcMethod = 0;
}

void Simulation::setForceCalcMethodByString( const string& forceCalcMethod )
{
  if ( forceCalcMethod.compare( Simulation::FORCE_CALC_METHOD_OCTREE_STRING ) )
  {
    setForceCalcMethod( Simulation::FORCE_CALC_METHOD_OCTREE );
  }
  else if ( forceCalcMethod.compare( Simulation::FORCE_CALC_METHOD_NAIVE_STRING ) )
  {
    setForceCalcMethod( Simulation::FORCE_CALC_METHOD_NAIVE );
  }
  else
  {
    cout << "Unrecognized string! forceCalcMethod not set. Using default/current value." << endl;
  }
}
