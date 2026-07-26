#ifndef PHYSICS_RUNREPORT_H
#define PHYSICS_RUNREPORT_H

#include <string>
#include <vector>

#include <BillProperties.h>
#include <Input/RunOptions.h>
#include <Physics/ScenarioBuilder.h>
#include <Physics/ScenarioSpec.h>
#include <Physics/Simulation.h>

/*! \brief One row of the run's history, sampled every second of video. */
struct StatSample {
    int frame = 0;
    int particleCount = 0;
    int merges = 0;
    double largestMassFraction = 0;
    double rmsRadius = 0;
};

/*! \brief The .json written next to every recorded video.
 *
 *  Holds the seed, the resolved scenario, the starting diagnostics and a
 *  coarse history, which together are enough to (a) reproduce the run exactly
 *  and (b) decide whether it was worth watching.
 */
class RunReport {
public:
    RunReport(const RunOptions &options,
              const BillProperties &config,
              const ScenarioSpec &scenario,
              const SetupDiagnostics &diagnostics);

    void sample(int frame, const SimulationStats &stats);
    void finish(int framesRendered, double wallClockSeconds, const SimulationStats &finalStats);

    std::string toJson() const;

    //! Returns false if the file couldn't be opened.
    bool writeTo(const std::string &path) const;

    static std::string currentGitRevision();

private:
    const RunOptions &options;
    const BillProperties &config;
    const ScenarioSpec &scenario;
    const SetupDiagnostics &diagnostics;

    std::string gitRevision;
    std::vector<StatSample> history;

    int framesRendered = 0;
    double wallClockSeconds = 0;
    SimulationStats finalStats;
    bool finished = false;
};

#endif //PHYSICS_RUNREPORT_H
