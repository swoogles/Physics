#ifndef PHYSICS_PHYSICSSANDBOXPROPERTIES_H
#define PHYSICS_PHYSICSSANDBOXPROPERTIES_H

#include <BillProperties.h>
#include <ForceCalculationMethod.h>

#include "../Conversions/Serializer.h"
#include <../lib/units.h>
#include <chrono>

using namespace units::mass;

// TODO Figure out why I can't swap private/public here
class PhysicsSandboxProperties  {
private:
    ForceCalculationMethod parseForceCalculationProperty(std::string value) const;
    int parseStringAsInt(std::string value) const;
    const BillProperties properties;
public:
    // TODO privatize these
    const ForceCalculationMethod forceCalculationMethod;
    const int numShapes;
    const float dt;
    const float octreeTheta;
    const float sandboxWidth;
    const kilogram_t mass;
    const std::chrono::seconds  maximumRunTime;

    PhysicsSandboxProperties(string fileName);

};

#endif
