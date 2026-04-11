#include "QuadrantPool.h"
#include "Quadrant.h"

// Global pool instance
QuadrantPool globalQuadrantPool;

QuadrantPool::QuadrantPool() : nextFree(0) {
    // Pre-allocate some capacity to reduce initial allocations
    pool.reserve(1000);
}

Quadrant* QuadrantPool::acquire(int level, PhysicalVector& pos, float width,
                                 meter_t radius, PhysicalVector weightedPosition,
                                 kilogram_t mass, const PhysicalVector particlePosition) {
    if (nextFree < pool.size()) {
        // Reuse existing quadrant from pool
        Quadrant* q = pool[nextFree].get();
        q->reinitialize(level, pos, width, radius, weightedPosition, mass, particlePosition);
        nextFree++;
        return q;
    } else {
        // Need to create a new quadrant
        pool.push_back(std::make_unique<Quadrant>(level, pos, width, radius, weightedPosition, mass, particlePosition));
        nextFree++;
        return pool.back().get();
    }
}

void QuadrantPool::releaseAll() {
    nextFree = 0;
}
