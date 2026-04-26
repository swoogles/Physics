# Plan: Use Octree for Closest-Pair Finding

## Problem
The current `findClosestPairs()` implementation in `ParticleList.cpp` is O(n²), which is unacceptable for 5000+ particles. We already have an octree structure used for force calculation that provides O(n log n) spatial queries.

## Current State (Uncommitted Changes)
- `minimum_merges_per_frame` config added to `simulation.properties`
- `ParticleList::findClosestPairs(int n)` implemented with O(n²) brute force
- `Simulation::update()` calls `findClosestPairs()` and adds results to collision pairs
- Feature works but is too slow

## Goal
Replace the O(n²) `findClosestPairs()` with an O(n log n) implementation using the octree.

## Implementation Steps

### Step 1: Understand Current Octree Structure
- `Quadrant` class in `src/Parallelization/Quadrant.h` and `.cpp`
- Used in `Simulation::calcForcesAll()` for force calculation
- Already traverses particles spatially during force calculation
- Collects `TouchingPair`s during traversal (see `PairCollection`)

### Step 2: Modify Octree Traversal to Track Closest Pairs
The octree traversal in `calcForcesAll()` already visits particle pairs. Modify it to:

1. Add a data structure to track the N closest pairs seen so far:
   ```cpp
   struct ClosestPairTracker {
       int maxPairs;
       std::vector<std::pair<shared_ptr<Particle>, shared_ptr<Particle>, double>> pairs;

       void consider(shared_ptr<Particle> a, shared_ptr<Particle> b, double distance);
       std::vector<std::pair<shared_ptr<Particle>, shared_ptr<Particle>>> getPairs();
   };
   ```

2. During octree traversal (where we already compute distances for collision detection), also track if this pair is among the N closest.

3. Return the closest pairs along with the collision pairs from `calcForcesAll()`.

### Step 3: Update Simulation::update()
Change the signature or return type of `calcForcesAll()` to also return closest pairs, OR pass a `ClosestPairTracker` reference that gets populated during traversal.

Option A - Modify return type:
```cpp
struct ForceCalcResult {
    PairCollection collisionPairs;
    std::vector<std::pair<shared_ptr<Particle>, shared_ptr<Particle>>> closestPairs;
};
ForceCalcResult calcForcesAll(ParticleList &physicalObjects, hour_t dt, int numClosestToTrack);
```

Option B - Pass tracker as parameter:
```cpp
PairCollection calcForcesAll(ParticleList &physicalObjects, hour_t dt, ClosestPairTracker* tracker = nullptr);
```

### Step 4: Remove O(n²) Implementation
Delete `ParticleList::findClosestPairs()` from:
- `src/ShapeFiles/ParticleList.h`
- `src/ShapeFiles/ParticleList.cpp`

### Step 5: Update Simulation::update() to Use New Approach
```cpp
void Simulation::update(hour_t dt) {
    Particle::updateCollisionRadiusMultiplier(physicalObjects.size(), stepsElapsed);

    int closestToTrack = 0;
    if (Particle::minimumMergesPerFrame > 0 && Particle::getInitialParticleCount() > 0) {
        float progress = 1.0f - (float)physicalObjects.size() / (float)Particle::getInitialParticleCount();
        if (progress < Particle::getMergeTargetFraction()) {
            closestToTrack = Particle::minimumMergesPerFrame;
        }
    }

    auto [collisionPairs, closestPairs] = calcForcesAll(this->physicalObjects, dt, closestToTrack);

    // Add forced merges
    for (const auto& pair : closestPairs) {
        TouchingPair forcedPair(pair.first, pair.second);
        collisionPairs.insertIfUnique(forcedPair);
    }

    physicalObjects.updateWithCollisions(dt, collisionPairs);
    updateTimeElapsed(dt);
    // ...
}
```

## Key Files to Modify
1. `src/Parallelization/Quadrant.cpp` - Add closest-pair tracking during traversal
2. `src/Physics/Simulation.cpp` - Update `calcForcesAll()` and `update()`
3. `src/Physics/Simulation.h` - Update function signatures if needed
4. `src/ShapeFiles/ParticleList.cpp` - Remove `findClosestPairs()`
5. `src/ShapeFiles/ParticleList.h` - Remove `findClosestPairs()` declaration

## Complexity Analysis
- Current: O(n²) for finding closest pairs + O(n log n) for force calculation
- After: O(n log n) for both (single traversal)

## Testing
1. Verify simulation still runs correctly
2. Verify minimum merges per frame still works
3. Compare frame time before/after to confirm performance improvement
