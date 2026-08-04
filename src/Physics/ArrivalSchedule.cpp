#include "ArrivalSchedule.h"

int ArrivalSchedule::roomFor(int currentParticleCount) const {
    if (target <= 0) {
        return 0;
    }

    if (limit > 0 && arrivals >= limit) {
        return 0;
    }

    const int deficit = target - currentParticleCount;
    if (deficit <= 0) {
        return 0;
    }

    /* Hold off until the gap is worth filling. Without this the run would
     * dribble in a handful of particles every few frames, and every one of
     * those would reset the merge ramp and start another grace period. */
    const int worthFilling = (int) (target * minimumDeficitFraction);
    if (deficit < worthFilling) {
        return 0;
    }

    return deficit;
}
