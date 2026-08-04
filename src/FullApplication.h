//
// Created by bfrasure on 1/8/19.
//

#ifndef PHYSICS_FULLAPPLICATION_H
#define PHYSICS_FULLAPPLICATION_H

#include "graphics/GraphicalOperations.h"
#include "graphics/Recorder.h"
#include "graphics/StreamingRecorder.h"

//GUI stuff
#include "Windows/ControlCenter.h"
#include "Windows/CenterStage.h"

#include "graphics/OpenGlSetup.h"

//Observers
#include "Observation/Observer.h"

//Physics
#include "Physics/Simulations.h"
#include "Physics/PhysicsSandboxProperties.h"
#include "Physics/ArrivalSchedule.h"

#include "Input/RunOptions.h"
#include "Sandbox/ApplicationResult.h"
#include "Sandbox/RunReport.h"

#include <lib/pstream.h>

#include <chrono>
#include <iomanip>
#include <random>
#include "TimedSceneAction.h"
#include <queue>
#include "TimedSceneAction.h"
using std::queue;

using std::chrono::time_point;
using std::chrono::duration;
using std::chrono::system_clock;

class FullApplication {
public:
    FullApplication(const RunOptions &options,
                    WindowDimensions windowDimensions,
                    PhysicsSandboxProperties properties,
                    OpenGlSetup openGlSetup,
                    Simulation simulation,
                    ScenarioSpec scenario,
                    RunReport *report);

    Simulation simulation;
    const ControlCenter controlCenter;
    CenterStage centerStage;
    Recorder recorder;
    unique_ptr<StreamingRecorder> streamingRecorder;
    const bool recording;
    const time_point<system_clock, duration<long int, std::ratio<1, 1000000000> >> start;
    const std::chrono::seconds maximumRuntime;
    void display() ;
    GraphicalOperations graphicalOperations;
    // TODO Instead of void, return a Result[SUCCESSFUL_STEP, COMPLETED, FAILED]
    ApplicationResult update();

    /*! \brief Asks the run to stop at the end of the current frame.
     *
     *  The interrupt handler calls this so Ctrl-C still finalizes the video and
     *  writes the sidecar instead of throwing the whole run away.
     */
    static void requestStop();

private:
    const RunOptions options;
    RunReport *report;
    int framesRendered;
    bool finished;

    queue<TimedSceneAction> timedSceneActions;

    /*! When extra groups arrive. Cadence and limit are defined once, in
     *  ArrivalSchedule.h - not here, and nowhere else.
     */
    ArrivalSchedule arrivals;

    //! What the run started as; arriving groups are drawn in its likeness.
    const ScenarioSpec scenario;

    //! Seeded from the scenario, so a seed reproduces the arrivals too.
    std::mt19937 arrivalRng;

    void finishRun();

    /*! \brief Prints one line each time the run passes another 10%.
     *
     *  A long render is otherwise completely silent about how far along it is.
     *  A run ends at whichever of its two limits it reaches first - the frame
     *  count or the wall-clock cap - so progress is measured against the nearer
     *  of them, and the estimate follows whichever that turns out to be.
     */
    void reportProgress();

    //! Last 10% mark announced, so each one is reported exactly once.
    int lastProgressDecile;
};


#endif //PHYSICS_FULLAPPLICATION_H
