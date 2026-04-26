#include "PhysicsSandboxProperties.h"
#include <stdexcept>

int PhysicsSandboxProperties::parseStringAsInt(std::string value) const {
  return atoi(value.c_str());
}

static float parseOptionalFloat(const BillProperties& props, const char* key, float defaultValue) {
    try {
        return Serializer::parseStringAsFloat(props.at(key));
    } catch (const std::out_of_range&) {
        return defaultValue;
    }
}

PhysicsSandboxProperties::PhysicsSandboxProperties(string fileName)
:properties(fileName),
numShapes(parseStringAsInt(properties.at("numShapes"))),
dt(Serializer::parseStringAsFloat(properties.at("dt"))),
octreeTheta(Serializer::parseStringAsFloat(properties.at("octree_theta"))),
sandboxWidth(Serializer::parseStringAsFloat(properties.at("sandbox_width"))),
mass(kilogram_t(Serializer::parseStringAsFloat(properties.at("mass")))),
maximumRunTime(Serializer::parseStringAsLong(properties.at("maximum_runtime"))*60),
collisionRadiusMultiplier(parseOptionalFloat(properties, "collision_radius_multiplier", 1.0f)),
mergeTargetFraction(parseOptionalFloat(properties, "merge_target_fraction", 0.9f)),
mergeTargetSteps(static_cast<int>(parseOptionalFloat(properties, "merge_target_seconds", 180.0f) * 24.0f)),
velocityDamping(parseOptionalFloat(properties, "velocity_damping", 1.0f)),
boundaryStrength(parseOptionalFloat(properties, "boundary_strength", 0.0f))
{

}
