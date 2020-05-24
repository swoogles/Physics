#include "PhysicsSandboxProperties.h"

int PhysicsSandboxProperties::parseStringAsInt(std::string value) const {
  return atoi(value.c_str());
}

PhysicsSandboxProperties::PhysicsSandboxProperties(string fileName)
:properties(fileName),
numShapes(parseStringAsInt(properties.at("numShapes"))),
dt(Serializer::parseStringAsFloat(properties.at("dt"))),
octreeTheta(Serializer::parseStringAsFloat(properties.at("octree_theta"))),
sandboxWidth(Serializer::parseStringAsFloat(properties.at("sandbox_width"))),
mass(kilogram_t(Serializer::parseStringAsFloat(properties.at("mass")))),
maximumRunTime(Serializer::parseStringAsLong(properties.at("maximum_runtime"))*60)
{

}
