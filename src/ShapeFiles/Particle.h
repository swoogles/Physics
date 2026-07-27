#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "../Dimensions/Moveable.h"

#include <../lib/units.h>

using namespace units::mass;
using namespace units::length;

class Particle: public Moveable {
public:
    // Multiplier for collision detection distance (1.0 = realistic, higher = more collisions)
    static float collisionRadiusMultiplier;
    static void setCollisionRadiusMultiplier(float startMultiplier, float mergeTargetFraction, int mergeTargetSteps, int initialParticleCount);
    static void updateCollisionRadiusMultiplier(int currentParticleCount, int currentStep);

    // Velocity damping and boundary settings
    static float velocityDamping;
    static float boundaryStrength;
    static float boundaryRadius;
    static float systemCenterX;
    static float systemCenterY;
    static float systemCenterZ;
    static void setDampingAndBoundary(float damping, float strength, float radius, PhysicalVector center);

    // Minimum merges per frame
    static int minimumMergesPerFrame;
    static void setMinimumMergesPerFrame(int minMerges);

    /*! \brief Limits that keep merging from looking like teleportation.
     *
     *  A merged body lands at the centre of mass of the two originals, so the
     *  heavier one shifts by distance * lighterMass / totalMass. Radius grows
     *  with mass and the collision multiplier ramps over time, so without a
     *  ceiling that shift grows into a large fraction of the frame.
     *
     *  Capping the separation outright would also block the harmless case - a
     *  big body swallowing a speck barely moves at all - and merging would
     *  stall. So the limit is on the resulting shift, not on the distance.
     *  Either value at 0 means no limit.
     */
    static float maxCollisionRadiusMultiplier;
    static double maxMergeJump;
    static void setMergeLimits(float maxMultiplier, double maxJump);

    //! How far the heavier of the two would move if these merged, in meters.
    static double mergeJumpFor(const Particle &a, const Particle &b);

    //! False when merging these two would visibly teleport the heavier one.
    static bool mergeAllowed(const Particle &a, const Particle &b);

    //! Jump budget for a body of this mass, scaled by how large it draws.
    static double allowedJumpFor(double mass);

    //! Largest distance any body has been moved by a merge, for the run report.
    static double largestMergeJump;

    //! The same, restricted to bodies big enough on screen to notice (>= 4px).
    static double largestVisibleMergeJump;

    /*! \brief How big a particle should be drawn, in pixels.
     *
     *  Physical radius is useless on screen: a body holding 20% of the
     *  system's mass is still only 17x the radius of a single particle, which
     *  works out to a few hundredths of a pixel. Without this, every particle
     *  renders as the same one-pixel dot and the biggest body is
     *  indistinguishable from a stray - it looks like it vanished.
     *
     *  Size follows the cube root of mass, so it tracks real radius ratios.
     */
    static double renderReferenceMass;
    static double maxPointSize;
    static void setRenderScale(double referenceMass, double maxPointSize);
    static float pointSizeFor(double mass);

    // Getters for merge progress checking
    static int getInitialParticleCount() { return initialParticleCount; }
    static float getMergeTargetFraction() { return mergeTargetFraction; }

private:
    static float collisionRadiusStartMultiplier;
    static float mergeTargetFraction;
    static int mergeTargetSteps;
    static int initialParticleCount;
    static int lastMergeStep;
    static int previousParticleCount;

public:
    Particle(
            PhysicalVector pos,
            kilogram_t mass,
            PhysicalVector momentum,
            kilograms_per_cubic_meter_t density,
            PhysicalVector color
    );

    Particle(
            PhysicalVector pos,
            double mass,
            float radius,
            PhysicalVector momentum,
            PhysicalVector color
    );

    double scale() const override;

    meter_t radius() const;
    void setRadius(meter_t radius);

    double momentOfInertia() const override;

    // TODO The next 2 functions might be better placed in a Sphere/Geometry class.
    static meter_t calcRadius(kilogram_t mass, kilograms_per_cubic_meter_t density);

    static vector<PhysicalVector> pointsEvenlyDistributedOnSphere(int numPoints, float radius);

    /*! \relates MyShape
     *  \brief Combines 2 distinct objects in an inelastic collision into the first object and eliminates the second
     *
     *  Combines 2 objects by:
     *  \n -Calculating their combined radius (calcMergedRadius)
     *  \n -Finding their center of mass and centering the new object there
     *  \n -Adding their masses and momentums
     *  \n -Calculating their combined angular momentum about the COM (calcMergedAngMomentum)
     *
     *  Note: This function does NOT delete the second object, currently that must be handled outside
     */
    void mergeWith(Particle &otherShape);

    bool isTouching(Particle &otherShape) const;
    bool isTouching(PhysicalVector pos, meter_t radius) const;

private:
    meter_t _radius;

    /*! \relates MyShape
     *  \brief Determines the final angular momentum after 2 objects collide in a completely inelastic collision
     *
     *  The final angular momentum is determined with the following formula:
     *  \n v1i = the initial velocity of object 1
     *  \n v2i = the initial velocity of object 2
     *	\n m1 = the mass of object 1
     *	\n m2 = the mass of object 2
     *	\n e = the coefficient of restitution (e = 1 for elastic collision)
     *	\n n = normal unit vector drawn from object 1 to object 2
     *	\n c = n . (v1i - v2i)
     *	\n v1f = v1i - ((m2c)/(m1 + m2))(1 + e)n
     *	\n v2f = v2i + ((m1c)/(m1 + m2))(1 + e)n
     *
     *	\return The angular momentum to be assigned to the merged object
     */
    PhysicalVector calcMergedAngMomentum(Particle &otherShape) const;

};
#endif
