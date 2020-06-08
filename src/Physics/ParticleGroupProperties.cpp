//
// Created by bfrasure on 6/7/20.
//

#include "ParticleGroupProperties.h"

ParticleGroupProperties::ParticleGroupProperties(
        const int numShapes,
        const float sandboxWidth,
        const kilogram_t &mass,
        const PhysicalVector color,
        const float momentumMultiplier)
        : numShapes(numShapes),
          sandboxWidth(sandboxWidth),
          mass(mass),
          color(color),
          momentumMultiplier(momentumMultiplier ){}
