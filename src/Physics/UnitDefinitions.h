//
// Created by bfrasure on 12/23/18.
//

#ifndef PHYSICS_UNITDEFINITIONS_H
#define PHYSICS_UNITDEFINITIONS_H

#include <../lib/units.h>
using namespace units::time;
using namespace units::mass;
using namespace units::density;

typedef hour_t duration;
typedef kilogram_t mass;
typedef kilograms_per_cubic_meter_t density;

namespace sandbox {
    using Mass = kilogram_t;

    class UnitDefinition {

    }; // sandbox
}

#endif //PHYSICS_UNITDEFINITIONS_H
