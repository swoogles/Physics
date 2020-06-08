//
// Created by bfrasure on 6/7/20.
//

#ifndef PHYSICS_PARTICLEGROUPPROPERTIES_H
#define PHYSICS_PARTICLEGROUPPROPERTIES_H

#include <BillProperties.h>

#include "../Conversions/Serializer.h"
#include <../lib/units.h>
#include <chrono>
#include <Dimensions/PhysicalVector.h>

using namespace units::mass;

struct ParticleGroupProperties {
    ParticleGroupProperties(const int numShapes, const float sandboxWidth, const kilogram_t &mass, const PhysicalVector color, const float momentumMultiplier);
    const int numShapes;
    const float sandboxWidth;
    const kilogram_t mass;
    const PhysicalVector color;
    const float momentumMultiplier;
};


#endif //PHYSICS_PARTICLEGROUPPROPERTIES_H
