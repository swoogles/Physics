#ifndef PHYSICS_SCENARIOSPEC_H
#define PHYSICS_SCENARIOSPEC_H

#include <map>
#include <random>
#include <string>
#include <vector>

#include <BillProperties.h>
#include <Dimensions/PhysicalVector.h>

#include "PhysicsSandboxProperties.h"

namespace physics_constants {
    //! Kept identical to the value Interactions::calcForceGravNew integrates with.
    constexpr double G = 6.67384e-11;
}

/*! \brief One blob of particles, described in SI units.
 *
 *  Everything here is data, so a scenario can be written by a script instead
 *  of compiled into Simulations.cpp.
 */
struct GroupSpec {
    //! Centre of the group, in meters.
    PhysicalVector position = PhysicalVector(0, 0, 0, true);

    //! Bulk velocity of the whole group, in m/s.
    PhysicalVector velocity = PhysicalVector(0, 0, 0, false);

    /*! Solid-body rotation, as a fraction of the group's break-up spin
     *  (sqrt(G*M/R^3)). The vector's direction is the rotation axis.
     *  0.3-0.6 on the z axis is what turns a blob into a disk.
     */
    PhysicalVector spin = PhysicalVector(0, 0, 0, false);

    int count = 1000;

    //! Total mass of the group in kg, split evenly across its particles.
    double mass = 5.9891e8;

    //! Radius of the sphere the particles are placed in, in meters.
    double radius = 6.0e5;

    PhysicalVector color = PhysicalVector(1, 1, 1);

    /*! Random internal velocity spread, as a fraction of the group's own
     *  virial velocity sqrt(G*M/R). 0 is perfectly cold.
     */
    double dispersion = 0.0;

    std::string label;
};

/*! \brief A whole starting configuration plus the global knobs that shape it. */
struct ScenarioSpec {
    std::string type = "diamond";
    std::string name = "unnamed";

    std::vector<GroupSpec> groups;

    /*! Target ratio of kinetic to gravitational potential energy at t=0,
     *  measured about the system's centre of mass.
     *
     *  0.5  - virial equilibrium, slow and stately
     *  0.1-0.3 - cold collapse: violent infall, bounce, re-expansion
     *  >1.0 - unbound, everything flies apart
     *
     *  A value <= 0 leaves the velocities exactly as specified.
     */
    double virialRatio = -1.0;

    unsigned int seed = 0;

    //! Every scenario.* / archetype key that was actually consulted.
    std::map<std::string, std::string> knobs;

    int totalParticles() const;
    double totalMass() const;
};

/*! \brief Turns a properties file into a ScenarioSpec. */
class ScenarioParser {
public:
    static ScenarioSpec parse(
            const BillProperties &properties,
            const PhysicsSandboxProperties &sandboxProperties,
            unsigned int seed);

    static PhysicalVector parseVector(const std::string &value, PhysicalVector fallback);
    static double parseDouble(const std::string &value, double fallback);

private:
    static std::vector<GroupSpec> explicitGroups(const BillProperties &p, const GroupSpec &defaults);
    static std::vector<GroupSpec> cluster(const BillProperties &p, const GroupSpec &defaults, ScenarioSpec &spec);
    static std::vector<GroupSpec> pair(const BillProperties &p, const GroupSpec &defaults, ScenarioSpec &spec);
    static std::vector<GroupSpec> chaotic(const BillProperties &p, const GroupSpec &defaults, ScenarioSpec &spec, std::mt19937 &rng);
    static std::vector<GroupSpec> diamond(const BillProperties &p, const GroupSpec &defaults, ScenarioSpec &spec);
};

#endif //PHYSICS_SCENARIOSPEC_H
