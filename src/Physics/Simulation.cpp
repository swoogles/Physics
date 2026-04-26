#include "Simulation.h"
#include "ShapeFiles/PairCollection.h"
#include "ShapeFiles/Particle.h"
#include "ShapeFiles/ClosestPairTracker.h"
#include <algorithm>
#include <cmath>
#include <mutex>
#include <omp.h>

Simulation::Simulation(
        ParticleList physicalObjects,
        CollisionType collisionType,
        float octreeTheta)
        :physicalObjects(std::move(physicalObjects))
        ,timeElapsed(0)
        ,stepsElapsed(0)
        ,minX(FLT_MAX)
        ,maxX(FLT_MIN)
        ,minY(FLT_MAX)
        ,maxY(FLT_MIN)
        ,collisionType(collisionType)
        ,octreeTheta(octreeTheta)
{
    this->updateMinsAndMaxes();
    this->refreshQuadrant(this->physicalObjects);
}

void Simulation::refreshQuadrant(ParticleList &physicalObjects) {
    PhysicalVector pos(0, 0, 0, true);

    float side = 10e7; //Formation Value . ACK!!!! How miserably hard-coded!!

    // Reuse existing quadrant to avoid repeated allocations
    if (quadrant) {
        quadrant->resetForRebuild(meter_t(0), PhysicalVector(), kilogram_t(0), PhysicalVector(), nullptr);
    } else {
        quadrant = std::make_unique<Quadrant>( 1, pos, side, meter_t(0), PhysicalVector(), kilogram_t(0), PhysicalVector(), nullptr );
    }

    this->physicalObjects.forEachWithPtr(
            [this](shared_ptr<Particle> curShape) {
                this->quadrant->insert(
                        curShape->radius(),
                        curShape->weightedPosition(),
                        curShape->mass(),
                        curShape->position(),
                        curShape);
            });
}

void Simulation::updateXYMinsAndMaxes(PhysicalVector curPos) {
    resetXYMinsAndMaxes();
    maxX = std::max(maxX, curPos.x());
    minX = std::min(minX, curPos.x());

    maxY = std::max(maxY, curPos.y());
    minY = std::min(minY, curPos.y());
}

MaximumValues Simulation::getXYMinsAndMaxes() const {
    return {
            this->minX,
            this->minY,
            this->maxX,
            this->maxY
    };
}

void Simulation::resetXYMinsAndMaxes() {
	minX = FLT_MAX;
	maxX = FLT_MIN;

	minY = FLT_MAX;
	maxY = FLT_MIN;
}

void Simulation::updateMinsAndMaxes() {
    // Smart autoscaling: filter by velocity, then find bounds for 95% of mass

    // Step 1: Calculate center of mass and total mass
    double totalMass = 0;
    PhysicalVector centerOfMass(0, 0, 0);
    PhysicalVector totalMomentum(0, 0, 0);

    physicalObjects.checkForAllParticles([&](const Particle& p) {
        double mass = p.mass().value();
        if (mass > 0) {
            totalMass += mass;
            centerOfMass = centerOfMass.plus(p.position().scaledBy(mass));
            totalMomentum = totalMomentum.plus(p.momentum());
        }
    });

    if (totalMass <= 0) return;

    centerOfMass = centerOfMass.scaledBy(1.0 / totalMass);
    PhysicalVector systemVelocity = totalMomentum.scaledBy(1.0 / totalMass);

    // Step 2: Calculate velocity dispersion (standard deviation relative to CoM)
    double velocityVarianceSum = 0;
    int particleCount = 0;

    physicalObjects.checkForAllParticles([&](const Particle& p) {
        double mass = p.mass().value();
        if (mass > 0) {
            PhysicalVector relVel = p.velocity().minus(systemVelocity);
            double speed = relVel.length();
            velocityVarianceSum += speed * speed;
            particleCount++;
        }
    });

    double velocityStdDev = (particleCount > 0)
        ? sqrt(velocityVarianceSum / particleCount)
        : 0;

    // Step 3: Collect particles that aren't escaping (velocity < 2.5x stddev)
    const double velocityThreshold = 2.5 * velocityStdDev;

    struct ParticleInfo {
        double mass;
        double distanceFromCoM;
        float x, y;
    };
    std::vector<ParticleInfo> boundParticles;
    double boundMass = 0;

    physicalObjects.checkForAllParticles([&](const Particle& p) {
        double mass = p.mass().value();
        if (mass > 0) {
            PhysicalVector relVel = p.velocity().minus(systemVelocity);
            double speed = relVel.length();

            // Include if velocity is below threshold (not escaping)
            if (speed <= velocityThreshold || velocityStdDev == 0) {
                PhysicalVector pos = p.position();
                double dist = pos.minus(centerOfMass).length();
                boundParticles.push_back({mass, dist, (float)pos.x(), (float)pos.y()});
                boundMass += mass;
            }
        }
    });

    if (boundParticles.empty()) return;

    // Step 4: Sort by distance from center of mass
    std::sort(boundParticles.begin(), boundParticles.end(),
        [](const ParticleInfo& a, const ParticleInfo& b) {
            return a.distanceFromCoM < b.distanceFromCoM;
        });

    // Step 5: Find bounds that contain 95% of bound mass
    const double targetMassFraction = 0.95;
    double targetMass = boundMass * targetMassFraction;
    double accumulatedMass = 0;

    resetXYMinsAndMaxes();

    for (const auto& info : boundParticles) {
        accumulatedMass += info.mass;

        maxX = std::max(maxX, info.x);
        minX = std::min(minX, info.x);
        maxY = std::max(maxY, info.y);
        minY = std::min(minY, info.y);

        if (accumulatedMass >= targetMass) {
            break;  // We've captured enough mass
        }
    }
}

void Simulation::update(hour_t dt) {
    // Grow collision multiplier every step until merge target is reached
    Particle::updateCollisionRadiusMultiplier(physicalObjects.size(), stepsElapsed);

    // Determine if we need to track closest pairs for forced merging
    int closestToTrack = 0;
    if (Particle::minimumMergesPerFrame > 0 && Particle::getInitialParticleCount() > 0) {
        float progress = 1.0f - (float)physicalObjects.size() / (float)Particle::getInitialParticleCount();
        if (progress < Particle::getMergeTargetFraction()) {
            closestToTrack = Particle::minimumMergesPerFrame;
        }
    }

    // Create tracker if needed (will be populated during octree traversal)
    ClosestPairTracker closestTracker(closestToTrack);

    // This is O(n log n) - collects both collision pairs AND closest pairs during octree traversal
    PairCollection collisionPairs = calcForcesAll(this->physicalObjects, dt,
        closestToTrack > 0 ? &closestTracker : nullptr);

    // Add forced merges from closest pairs (found during O(n log n) traversal, not O(n²))
    if (closestToTrack > 0) {
        auto closestPairs = closestTracker.getPairs();
        for (const auto& pair : closestPairs) {
            TouchingPair forcedPair(pair.first, pair.second);
            collisionPairs.insertIfUnique(forcedPair);
        }
    }

    physicalObjects.updateWithCollisions(dt, collisionPairs);
    updateTimeElapsed(dt);

    // TODO This causes another full iteration of all shapes. If it's going to happen,
    //  it should be done during one of the earlier iterations.
    updateMinsAndMaxes();
    // This is the first "n" part in "n log(n)"
    refreshQuadrant(this->physicalObjects);
}

void Simulation::updateTimeElapsed(hour_t dt) {
    timeElapsed += dt;
    stepsElapsed += 1;
}

hour_t Simulation::getTimeElapsed() const { return timeElapsed; }


//1.
//a. If the current node is an external node
//b. (and it is not body b),
//c. calculate the force exerted by the current node on b
//d. add this amount to b’s net force.
//2.
//a. Otherwise, calculate the ratio s/d. If s/d < θ, treat this internal node as a single body, and calculate the force it exerts on body b
//b. add this amount to b’s net force.
//3. Otherwise, run the procedure recursively on each of the current node’s children.


PairCollection Simulation::calcForcesAll(ParticleList &physicalObjects, hour_t dt, ClosestPairTracker* closestTracker) {
    PairCollection collectedPairs;
    std::mutex pairsMutex;

    // Per-thread trackers to avoid lock contention when tracking closest pairs
    std::vector<ClosestPairTracker> threadTrackers;
    int numThreads = 1;
    #ifdef _OPENMP
    #pragma omp parallel
    {
        #pragma omp single
        numThreads = omp_get_num_threads();
    }
    #endif
    if (closestTracker && closestTracker->isTracking()) {
        // Each thread gets its own tracker
        for (int i = 0; i < numThreads; i++) {
            threadTrackers.emplace_back(closestTracker->isTracking() ? 10 : 0); // Track more per thread, merge later
        }
    }

    this->physicalObjects.applyToAllParticlesParallelWithPtr(
            [this, dt, &collectedPairs, &pairsMutex, closestTracker, &threadTrackers](shared_ptr<Particle> particlePtr) {
                Particle& particle = *particlePtr;

                // Get thread-local tracker if we're tracking closest pairs
                ClosestPairTracker* localTracker = nullptr;
                if (closestTracker && closestTracker->isTracking() && !threadTrackers.empty()) {
                    int threadId = 0;
                    #ifdef _OPENMP
                    threadId = omp_get_thread_num();
                    #endif
                    if (threadId < static_cast<int>(threadTrackers.size())) {
                        localTracker = &threadTrackers[threadId];
                    }
                }

                auto quadrantFunction =
                [this, &particle, &particlePtr, &collectedPairs, &pairsMutex, localTracker, dt](Quadrant & quadrant) {
                    particle.adjustMomentum(
                            this->interactions.calcForceGravNew(particle, quadrant, dt)
                    );
                    // For external nodes (leaf nodes with particles), check collisions and track closest pairs
                    if (quadrant.isExternal() && quadrant.getParticlePtr() != particlePtr) {
                        auto otherParticle = quadrant.getParticlePtr();
                        if (otherParticle) {
                            // Calculate distance between particles
                            double distance = particle.position().minus(quadrant.getParticlePosition()).length();

                            // Track closest pairs if enabled (O(n log n) during traversal)
                            if (localTracker && localTracker->isTracking()) {
                                // Only track if particlePtr < otherParticle to avoid duplicates
                                if (particlePtr.get() < otherParticle.get()) {
                                    localTracker->consider(particlePtr, otherParticle, distance);
                                }
                            }

                            // Check for collision
                            if (particle.isTouching(quadrant.getParticlePosition(), quadrant.getParticleRadius())) {
                                TouchingPair pair(particlePtr, otherParticle);
                                std::lock_guard<std::mutex> lock(pairsMutex);
                                collectedPairs.insertIfUnique(pair);
                            }
                        }
                    }
                };
                auto terminalPredicate =
                        [this, &particle](Quadrant & quadrant) {
                            double distance = particle.distanceTo(quadrant);
                            return quadrant.getWidth() / distance < octreeTheta  && !quadrant.positionIsInQuadrantBoundaries(particle.position());
                };
                quadrant->applyToAllChildren(quadrantFunction, terminalPredicate);
            });

    // Merge thread-local trackers into the main tracker
    if (closestTracker && closestTracker->isTracking()) {
        std::lock_guard<std::mutex> lock(pairsMutex);
        for (const auto& tracker : threadTrackers) {
            closestTracker->merge(tracker);
        }
    }

    return collectedPairs;
}

void Simulation::applySideEffectingFunctionsToInnards(
        function<void(const Quadrant &)> quadrantFunctor,
        function<void (const Particle &)> particleFunctor)
        const {
    quadrant->applyToAllChildrenConstant(quadrantFunctor);
    physicalObjects.checkForAllParticles(particleFunctor);
}

second_t Simulation::getOutputViewingTime() const {
    return units::time::second_t(stepsElapsed/24);
}
