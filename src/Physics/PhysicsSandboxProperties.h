#ifndef PHYSICS_PHYSICSSANDBOXPROPERTIES_H
#define PHYSICS_PHYSICSSANDBOXPROPERTIES_H

#include <BillProperties.h>

#include "../Conversions/Serializer.h"
#include <../lib/units.h>
#include <chrono>

using namespace units::mass;

// TODO Figure out why I can't swap private/public here
class PhysicsSandboxProperties  {
private:
    int parseStringAsInt(std::string value) const;
    const BillProperties properties;
public:
    // TODO privatize these
    const int numShapes;
    const float dt;
    const float octreeTheta;
    const float sandboxWidth;
    const kilogram_t mass;
    const std::chrono::seconds  maximumRunTime;
    const float collisionRadiusMultiplier;  // Multiplier for collision detection distance (1.0 = realistic)
    const float mergeTargetFraction;  // Stop growing multiplier at this merge fraction (0.0 to 1.0)
    const int mergeTargetSteps;  // Target steps to reach merge fraction (at 24 fps)

    PhysicsSandboxProperties(string fileName);

};

#endif
