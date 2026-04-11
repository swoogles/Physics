#ifndef QUADRANT_POOL_H
#define QUADRANT_POOL_H

#include <vector>
#include <memory>
#include "Quadrant.h"

// Simple object pool to reduce allocation overhead for Quadrant objects.
// Instead of creating/destroying Quadrants each frame, we recycle them.
class QuadrantPool {
public:
    QuadrantPool();

    // Get a quadrant from the pool, creating one if necessary
    Quadrant* acquire(int level, PhysicalVector& pos, float width,
                      meter_t radius, PhysicalVector weightedPosition,
                      kilogram_t mass, const PhysicalVector particlePosition);

    // Mark all quadrants as available for reuse (call at start of each frame)
    void releaseAll();

    // Get current pool statistics
    size_t poolSize() const { return pool.size(); }
    size_t inUse() const { return nextFree; }

private:
    std::vector<std::unique_ptr<Quadrant>> pool;
    size_t nextFree;
};

// Global pool instance for the simulation
extern QuadrantPool globalQuadrantPool;

#endif
