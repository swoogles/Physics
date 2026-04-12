//
// Created by bfrasure on 12/23/18.
//

#ifndef PHYSICS_UNITDEFINITIONS_H
#define PHYSICS_UNITDEFINITIONS_H

#include <../lib/units.h>
using namespace units::time;
using namespace units::mass;
using namespace units::density;
using namespace units::velocity;
using namespace units::force;

typedef hour_t local_duration;
typedef kilogram_t mass;
typedef kilograms_per_cubic_meter_t density;

namespace sandbox {
    using Mass = kilogram_t;
    using Velocity = meters_per_second_t;

    // Momentum = Mass × Velocity (kg⋅m/s)
    // The units library will automatically create the compound type when multiplying
    using Momentum = decltype(kilogram_t() * meters_per_second_t());

    class UnitDefinition {

    }; // sandbox
}

#endif //PHYSICS_UNITDEFINITIONS_H
