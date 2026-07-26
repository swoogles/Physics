#ifndef PHYSICS_SCENARIOBUILDER_H
#define PHYSICS_SCENARIOBUILDER_H

#include <random>
#include <string>

#include <ShapeFiles/ParticleList.h>

#include "ScenarioSpec.h"

/*! \brief What the starting configuration actually came out to.
 *
 *  Printed at startup and written into the run's sidecar, so a video can be
 *  traced back to the numbers that produced it.
 */
struct SetupDiagnostics {
    int particleCount = 0;
    double totalMass = 0;

    double kineticEnergy = 0;
    double potentialEnergy = 0;    //!< Negative, estimated analytically.

    double virialRatioBefore = 0;  //!< KE / |PE| as specified.
    double virialRatioAfter = 0;   //!< KE / |PE| after any rescaling.
    double velocityScale = 1.0;    //!< Factor applied to reach the target ratio.

    double systemRadius = 0;             //!< Max particle distance from the centre of mass.
    double characteristicVelocity = 0;   //!< sqrt(G*M/R) for the whole system.
    double crossingTimeSeconds = 0;      //!< R / characteristic velocity.
    double stepsPerCrossing = 0;         //!< Crossing time in units of dt: pacing on screen.
    double secondsOfVideoPerCrossing = 0;//!< At 24 fps, how long a crossing lasts on screen.

    PhysicalVector centerOfMass = PhysicalVector(0, 0, 0, true);

    //! Recomputes the dt-dependent pacing numbers after dt is chosen.
    void applyDt(double dtSeconds);

    std::string toJson() const;
};

class ScenarioBuilder {
public:
    /*! Places every group's particles, applies spin, dispersion and the target
     *  virial ratio, and reports what it ended up with.
     */
    static ParticleList build(
            const ScenarioSpec &spec,
            double dtSeconds,
            SetupDiagnostics &diagnostics,
            std::mt19937 &rng);

    //! Uniform point inside a sphere of the given radius, centred on the origin.
    static PhysicalVector randomPointInSphere(double radius, std::mt19937 &rng);

private:
    static double estimatePotentialEnergy(const ScenarioSpec &spec);
};

#endif //PHYSICS_SCENARIOBUILDER_H
