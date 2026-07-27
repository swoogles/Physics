#ifndef PHYSICS_PHYSICSSANDBOXPROPERTIES_H
#define PHYSICS_PHYSICSSANDBOXPROPERTIES_H

#include <BillProperties.h>

#include "../Conversions/Serializer.h"
#include <../lib/units.h>
#include <chrono>
#include <Observation/Observer.h>

using namespace units::mass;

// TODO Figure out why I can't swap private/public here
class PhysicsSandboxProperties  {
private:
    int parseStringAsInt(std::string value) const;
    const BillProperties properties;
public:
    // TODO privatize these
    const int numShapes;

    /*! Seconds of simulated time per frame.
     *
     *  `dt=auto` in the config leaves this at 0 until the scenario is built,
     *  at which point it is set to crossingTime / framesPerCrossing so every
     *  scenario plays back at a watchable speed regardless of its scale.
     */
    float dt;

    //! Frames one crossing time should take on screen. 0 keeps dt as written.
    const float framesPerCrossing;
    const float octreeTheta;
    const float sandboxWidth;
    const kilogram_t mass;
    const std::chrono::seconds  maximumRunTime;
    const float collisionRadiusMultiplier;  // Multiplier for collision detection distance (1.0 = realistic)
    const float mergeTargetFraction;  // Stop growing multiplier at this merge fraction (0.0 to 1.0)
    const int mergeTargetSteps;  // Target steps to reach merge fraction (at 24 fps)
    const float velocityDamping;  // Damping factor per frame (1.0 = none, 0.999 = slight)
    const float boundaryStrength;  // Soft boundary force strength
    const int minimumMergesPerFrame;  // Guaranteed merges per frame
    const float boundaryRadiusFactor;  // Boundary radius as a multiple of the starting system radius
    const float maxPointSize;  // Pixel cap on how large a merged body is drawn
    const float maxCollisionRadiusMultiplier;  // Ceiling on the collision multiplier ramp
    const float maxMergeJumpFraction;  // Cap on merge-induced displacement, as a fraction of system radius

    PhysicsSandboxProperties(string fileName);

    //! The underlying key/value pairs, for scenario parsing and run reports.
    const BillProperties & raw() const { return properties; }

    //! How smoothly the auto-scaling camera should follow the simulation.
    AutoScaleTuning autoScaleTuning() const;

};

#endif
