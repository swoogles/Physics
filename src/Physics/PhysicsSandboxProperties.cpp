#include "PhysicsSandboxProperties.h"

ForceCalculationMethod PhysicsSandboxProperties::parseForceCalculationProperty(std::string value) {
  // Should these go somewhere else?
  const std::string FORCE_CALC_METHOD_OCTREE_STRING = "octree";
  const std::string FORCE_CALC_METHOD_NAIVE_STRING = "naive";

  if ( value == FORCE_CALC_METHOD_OCTREE_STRING  ) {
    return ForceCalculationMethod::OCTREE;
  }
  else if ( value == FORCE_CALC_METHOD_NAIVE_STRING ) {
    return ForceCalculationMethod::NAIVE;
  } else {
    throw ( "That is not a real force-calculation method: " );
  }
}

int PhysicsSandboxProperties::parseNumShapes(std::string value) {
  return atoi(value.c_str());
}

PhysicsSandboxProperties::PhysicsSandboxProperties(BillProperties properties)
: forceCalculationMethod(parseForceCalculationProperty(properties.at("forceCalculationMethod" ))),
  numShapes(parseNumShapes(properties.at( "numShapes" ))) {}