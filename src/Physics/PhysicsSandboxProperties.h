#ifndef PHYSICS_PHYSICSSANDBOXPROPERTIES_H
#define PHYSICS_PHYSICSSANDBOXPROPERTIES_H

#include <BillProperties.h>
#include <ForceCalculationMethod.h>

#include "../Conversions/Serializer.h"

class PhysicsSandboxProperties  {
private:
    ForceCalculationMethod parseForceCalculationProperty(std::string value);
    int parseStringAsInt(std::string value);
public:
    // TODO privatize these
    const ForceCalculationMethod forceCalculationMethod;
    const int numShapes;
    const float dt;
    const float octreeTheta;

    PhysicsSandboxProperties(BillProperties billProperties);

};

#endif
