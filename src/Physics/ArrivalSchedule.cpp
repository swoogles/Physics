#include "ArrivalSchedule.h"

bool ArrivalSchedule::due(double nowSeconds) {
    if (everySeconds <= 0 || limit <= 0 || arrivals >= limit) {
        return false;
    }

    if (nextArrivalTime < 0) {
        nextArrivalTime = firstAt;
    }

    if (nowSeconds < nextArrivalTime) {
        return false;
    }

    arrivals++;

    /* Move to the next slot, but never leave it in the past: a schedule that
     * has fallen behind should resume from here, not fire every frame until it
     * has caught up. */
    nextArrivalTime += everySeconds;
    if (nextArrivalTime <= nowSeconds) {
        nextArrivalTime = nowSeconds + everySeconds;
    }

    return true;
}
