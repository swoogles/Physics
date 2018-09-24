#ifndef PHYSICS_PHYSICSSANDBOXPROPERTIES_H
#define PHYSICS_PHYSICSSANDBOXPROPERTIES_H

#include <BillProperties.h>
#include <ForceCalculationMethod.h>

class PhysicsSandboxProperties  {
private:
    ForceCalculationMethod parseForceCalculationProperty(std::string value);
    int parseNumShapes(std::string value);
public:
    // TODO privatize these
    const ForceCalculationMethod forceCalculationMethod;
    const int numShapes;

    PhysicsSandboxProperties(BillProperties billProperties);

};

#endif //PHYSICS_PHYSICSSANDBOXPROPERTIES_H
