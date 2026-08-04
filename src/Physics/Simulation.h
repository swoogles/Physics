#ifndef SIMULATION_H_
#define SIMULATION_H_

#include "../Parallelization/Quadrant.h"
#include "CollisionType.h"
#include <ShapeFiles/ParticleList.h>
#include <ShapeFiles/PairCollection.h>
#include <ShapeFiles/ClosestPairTracker.h>
#include "../MaximumValues.h"
#include <Physics/Interactions.h>
#include "lib/units.h"

using namespace units::time;


/*! \brief A cheap snapshot of what the simulation looks like right now.
 *
 *  Sampled during a run and written into the sidecar so runs can be ranked
 *  without a human watching every video.
 */
struct SimulationStats {
    int stepsElapsed = 0;
    int particleCount = 0;
    int totalMerges = 0;
    int mergesLastStep = 0;
    double totalMass = 0;
    double largestMassFraction = 0;  //!< Mass of the biggest body over total mass.
    double rmsRadius = 0;            //!< Mass-weighted spread about the centre of mass.
    PhysicalVector centerOfMass = PhysicalVector(0, 0, 0, true);  //!< Where the action currently is.
    double largestMergeJump = 0;     //!< Furthest a merge has moved a body, in meters.
    double largestVisibleMergeJump = 0;  //!< The same, for bodies drawn 4px or larger.
};

class Simulation {
public:
    Simulation(ParticleList physicalObjects, CollisionType collisionType, float octreeTheta);

    SimulationStats getStats() const;

	void applySideEffectingFunctionsToInnards(
			function<void (const Quadrant &)> quadrantFunctor,
			function<void (const Particle &)> particleFunctor
	)const ;


    hour_t getTimeElapsed() const;
    second_t getOutputViewingTime() const;

    //! Cheap enough to ask every frame, unlike getStats().
    int particleCount() const { return (int) physicalObjects.size(); }

    // TODO Take dt as a parameter here.
    void update(hour_t dt);

    MaximumValues getXYMinsAndMaxes() const;
    
    /*! \brief Drops a new group of particles into a run already in progress.
     *
     *  Stamps the arrivals so the forced-merge floor leaves them alone for a
     *  moment, and re-paces the merge schedule around the new material.
     */
    void addGroup(ParticleList newGroup);

private:
    ParticleList physicalObjects;
    hour_t timeElapsed;
    int stepsElapsed;

    int totalMerges;
    int mergesLastStep;

	float minX, maxX, minY, maxY;

    float octreeTheta;

    unique_ptr<Quadrant>  quadrant;

    CollisionType collisionType;

    Interactions interactions;

	void updateTimeElapsed(hour_t dt);

	void updateMinsAndMaxes();
    void resetXYMinsAndMaxes();
    void updateXYMinsAndMaxes(PhysicalVector curPos);

	void refreshQuadrant(ParticleList &physicalObjects);

	PairCollection calcForcesAll(ParticleList &physicalObjects, hour_t dt, ClosestPairTracker* closestTracker = nullptr);

} ;

#endif
