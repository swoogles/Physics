//
// Created by bfrasure on 10/31/18.
//

#ifndef PHYSICS_ASTRONOMICALVALUES_H
#define PHYSICS_ASTRONOMICALVALUES_H

#include <../lib/units.h>

using namespace units::mass;
using namespace units::density;

class AstronomicalValues {
public:
    const static kilogram_t MASS_SUN;

    const static kilograms_per_cubic_meter_t DENSITY_SUN;

};


#endif //PHYSICS_ASTRONOMICALVALUES_H
