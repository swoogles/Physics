#ifndef PHYSICS_ARRIVALSCHEDULE_H
#define PHYSICS_ARRIVALSCHEDULE_H

/*! \brief Decides when a run has room for another group.
 *
 *  Not a clock. A group arrives when merging has thinned the run out enough
 *  that adding one brings the population back to roughly what it started with,
 *  which means the run refills itself at whatever rate it is actually merging
 *  rather than at a rate someone had to guess in advance. A run that merges
 *  fast gets groups often; one that merges slowly gets them rarely; and the
 *  particle count - which is what costs time - never climbs above where it
 *  started.
 *
 *  The size of each arrival follows from the same rule: it is built to fit the
 *  space available, so arrivals cannot overshoot.
 */
struct ArrivalSchedule {
    // ---------------------------------------------------------------------
    //  THE KNOBS. This is the place to change how arrivals are governed.
    // ---------------------------------------------------------------------

    /*! How far the population must fall before a group is worth adding, as a
     *  fraction of the starting count. Small values top the run up constantly
     *  with little groups; large ones wait and drop in something substantial.
     */
    double minimumDeficitFraction = 0.15;

    //! Safety net on the total number of arrivals. 0 means no limit.
    int limit = 0;

    // ---------------------------------------------------------------------

    //! The population to refill toward - the count the run started with.
    void setTargetPopulation(int count) { target = count; }

    /*! \brief How many particles the run has room for right now.
     *
     *  0 when the gap is not yet worth filling, when the limit is used up, or
     *  before a target has been set. Otherwise the exact shortfall, which is
     *  also the size the arriving group should be built to.
     */
    int roomFor(int currentParticleCount) const;

    //! Records that a group has arrived, for the limit.
    void noteArrival() { arrivals++; }

    int arrivalsSoFar() const { return arrivals; }
    int targetPopulation() const { return target; }

private:
    int target = 0;
    int arrivals = 0;
};

#endif //PHYSICS_ARRIVALSCHEDULE_H
