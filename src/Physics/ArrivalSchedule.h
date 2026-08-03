#ifndef PHYSICS_ARRIVALSCHEDULE_H
#define PHYSICS_ARRIVALSCHEDULE_H

/*! \brief When new groups should drop into a run that is already going.
 *
 *  The single place the arrival cadence is defined. Nothing else should carry
 *  its own copy of the interval or the limit - the whole point of this type is
 *  that "every 5 seconds instead of 20" is one edit, in one file.
 *
 *  Times are in seconds of rendered video, matching
 *  Simulation::getOutputViewingTime(), not wall clock.
 */
struct ArrivalSchedule {
    // ---------------------------------------------------------------------
    //  THE THREE KNOBS. This is the place to change the cadence.
    // ---------------------------------------------------------------------

    //! When the first extra group shows up.
    double firstAt = 5.0;

    //! Gap between arrivals. Zero or less disables arrivals entirely.
    double everySeconds = 5.0;

    //! How many groups may arrive over the whole run.
    int limit = 5;

    // ---------------------------------------------------------------------

    /*! \brief True when a group is due, consuming that slot.
     *
     *  At most one arrival per call, and slots do not bank: if the clock jumps
     *  several intervals - a long pause, a slow frame - the run gets one group
     *  and the schedule picks up from now, rather than dumping the backlog in
     *  at once.
     */
    bool due(double nowSeconds);

    int arrivalsSoFar() const { return arrivals; }

private:
    int arrivals = 0;
    double nextArrivalTime = -1.0;   //!< Negative until the first due() call.
};

#endif //PHYSICS_ARRIVALSCHEDULE_H
