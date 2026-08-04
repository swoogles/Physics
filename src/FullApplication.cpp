//
// Created by bfrasure on 1/8/19.
//

#include "FullApplication.h"
#include "Physics/Simulations.h"
#include <atomic>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <vector>
#include <random>
using namespace std;
using std::chrono::time_point;
using std::chrono::time_point_cast;

namespace {
    //! Set by the interrupt handler; checked once per frame.
    atomic<bool> stopRequested(false);

    //! One video second, at the recorder's 24 fps input rate.
    const int SAMPLE_INTERVAL_FRAMES = 24;

    //! "45s", "3m20s", "1h04m" - short enough to sit inside a one-line report.
    string formatDuration(double seconds) {
        if (!std::isfinite(seconds) || seconds < 0) {
            return "?";
        }

        const long total = (long) (seconds + 0.5);
        ostringstream out;
        out << setfill('0');

        if (total >= 3600) {
            out << (total / 3600) << "h" << setw(2) << ((total % 3600) / 60) << "m";
        } else if (total >= 60) {
            out << (total / 60) << "m" << setw(2) << (total % 60) << "s";
        } else {
            out << total << "s";
        }
        return out.str();
    }
}

void FullApplication::requestStop() {
    stopRequested = true;
}

FullApplication::FullApplication(const RunOptions &options,
                                 WindowDimensions windowDimensions,
                                 PhysicsSandboxProperties properties,
                                 OpenGlSetup openGlSetup,
                                 Simulation simulation,
                                 ScenarioSpec scenario,
                                 RunReport *report)
        : simulation(std::move(simulation)),
        start(system_clock::now()),
        controlCenter(hour_t(properties.dt), windowDimensions.width, start),
          centerStage (windowDimensions.width, system_clock::to_time_t(time_point_cast<system_clock::duration>(start))),
          recorder(Recorder()),
          recording(options.recording),
          maximumRuntime(
                  options.maxSeconds > 0
                  ? std::chrono::seconds(options.maxSeconds)
                  : properties.maximumRunTime),
          graphicalOperations(
                  openGlSetup.mainDisplayNum,
                  openGlSetup.controlCenterNum,
                  windowDimensions,
                  properties.autoScaleTuning()),
          options(options),
          report(report),
          framesRendered(0),
          finished(false),
          scenario(std::move(scenario)),
          // Offset so arrivals don't replay the sequence the setup already drew.
          arrivalRng(this->scenario.seed + 0x9E3779B9u),
          lastProgressDecile(-1)
{
    /* Whatever the run started with is what arrivals top it back up to, so the
     * particle count - the thing that costs time - has a ceiling. */
    arrivals.setTargetPopulation(this->simulation.particleCount());

    // Intentionally no default scripted camera actions.
    // Keep timedSceneActions in place so scripted camera paths can be re-enabled later.

    // Note: StreamingRecorder will be lazily initialized on first frame capture
    // to ensure OpenGL context is ready
}

void FullApplication::reportProgress() {
    const duration<double> sinceStart = system_clock::now() - start;
    const double elapsed = sinceStart.count();

    /* Whichever limit the run reaches first is the one that ends it, so the
     * honest answer is the nearer of the two. */
    double fraction = 0;
    if (options.maxFrames > 0) {
        fraction = (double) framesRendered / (double) options.maxFrames;
    }
    if (maximumRuntime.count() > 0) {
        fraction = std::max(fraction, elapsed / (double) maximumRuntime.count());
    }

    if (fraction <= 0) {
        return;  // Nothing bounded to measure against - an open-ended run.
    }

    const int decile = std::min(10, (int) (fraction * 10));
    if (decile <= lastProgressDecile) {
        return;
    }
    lastProgressDecile = decile;

    /* Built on its own stream: `fixed` and `setprecision` are sticky, and
     * cout is shared with everything else that reports during a run. */
    ostringstream line;
    line << "[" << setw(3) << (decile * 10) << "%] frame " << framesRendered;
    if (options.maxFrames > 0) {
        line << "/" << options.maxFrames;
    }

    line << " | " << fixed << setprecision(1)
         << (elapsed > 0 ? framesRendered / elapsed : 0) << " fps"
         << " | elapsed " << formatDuration(elapsed)
         << " | eta ";

    // One frame in, the rate is not yet worth extrapolating from.
    if (decile > 0) {
        line << formatDuration(elapsed * (1.0 - fraction) / fraction);
    } else {
        line << "estimating";
    }

    line << " | " << simulation.particleCount() << " particles";

    cout << line.str() << endl;
}

void FullApplication::finishRun() {
    if (finished) {
        return;
    }
    finished = true;

    if (recording && streamingRecorder) {
        cout << "Finalizing video after " << framesRendered << " frames..." << endl;
        streamingRecorder->finalize();
    }

    if (report) {
        const duration<double> elapsed = system_clock::now() - start;
        report->finish(framesRendered, elapsed.count(), simulation.getStats());

        const string sidecar = options.sidecarPath();
        if (options.writeSidecar && !sidecar.empty()) {
            if (report->writeTo(sidecar)) {
                cout << "Wrote run report: " << sidecar << endl;
            } else {
                cerr << "Failed to write run report: " << sidecar << endl;
            }
        }
    }
}

ApplicationResult FullApplication::update() {
    // Checked before stepping: the previous frame has been drawn and captured
    // by now, so the video comes out exactly maxFrames long.
    if (options.maxFrames > 0 && framesRendered >= options.maxFrames) {
        cout << "Frame limit reached (" << options.maxFrames << ")." << endl;
        finishRun();
        return ApplicationResult::COMPLETED;
    }

    if (! controlCenter.isPaused() ) {
        auto dt = controlCenter.getDt();
        simulation.update(dt);
        centerStage.update(dt.value());
        framesRendered++;
        reportProgress();

        if (report && framesRendered % SAMPLE_INTERVAL_FRAMES == 0) {
            report->sample(framesRendered, simulation.getStats());
        }
        
        /* Merging has to have made room first, so the run refills at the rate
         * it is actually thinning out rather than on a guessed interval. */
        const int room = arrivals.roomFor(simulation.particleCount());

        if (room > 0) {
            // Aimed at where the action currently is, not where it started.
            const SimulationStats stats = simulation.getStats();

            const GroupSpec arriving = ScenarioBuilder::arrivalGroup(
                    scenario,
                    arrivals.arrivalsSoFar(),
                    stats.centerOfMass,
                    stats.totalMass,
                    room,
                    arrivalRng);

            /* Built on its own, with no system-wide virial rescale: the group
             * keeps the approach velocity it was just given. */
            ScenarioSpec arrivingScenario;
            arrivingScenario.virialRatio = -1.0;
            arrivingScenario.groups.push_back(arriving);

            SetupDiagnostics diagnostics;
            ParticleList newGroup = ScenarioBuilder::build(
                    arrivingScenario, 1.0, diagnostics, arrivalRng);

            cout << "Introducing " << arriving.label
                 << " at " << simulation.getOutputViewingTime().value() << "s"
                 << " | " << arriving.count << " particles"
                 << " | " << simulation.particleCount() << " -> "
                 << (simulation.particleCount() + arriving.count)
                 << " of " << arrivals.targetPopulation() << endl;

            simulation.addGroup(std::move(newGroup));
            arrivals.noteArrival();
        }
    }
    graphicalOperations.updateObserver(simulation.getXYMinsAndMaxes());

    time_point end = system_clock::now();

    duration<double> elapsed_seconds = end-start;

    if ( elapsed_seconds > maximumRuntime || stopRequested ) {
        cout << (stopRequested ? "Stop requested - wrapping up." : "Maximum runtime reached.") << endl;

        finishRun();
        return ApplicationResult::COMPLETED;
    }

    if (!timedSceneActions.empty()) {
        auto currentAction = timedSceneActions.front();
        if (simulation.getOutputViewingTime() > currentAction.triggerTime) {
            for (int i = 0; i < 90; i ++) {
                ControlCenter::submitCameraAction(currentAction.cameraAction);
            }
            timedSceneActions.pop();
        }
    }
    return ApplicationResult::SUCESSFUL_STEP;
}

void FullApplication::display() {
    this->graphicalOperations.fullDisplay(simulation);

    // Capture frame AFTER rendering is complete (only when not paused)
    if ( recording && !controlCenter.isPaused() ) {
        int currentWindow = glutGetWindow();
        glutSetWindow(1); // Capture from the main display window

        // Lazy initialization - create recorder on first frame capture
        if (!streamingRecorder) {
            auto dimensions = graphicalOperations.currentDimensions();

            /* A window can't be larger than the display, so asking for 1440p on
             * a smaller screen silently records at whatever the window manager
             * allowed. Say so rather than letting the run finish and surprise
             * someone with the wrong resolution. */
            if (dimensions.width < options.width || dimensions.height < options.height) {
                cout << "WARNING: asked for " << options.width << "x" << options.height
                     << " but the window is only " << dimensions.width << "x" << dimensions.height
                     << ". Recording at the smaller size - a window cannot exceed the display."
                     << endl;
            }

            streamingRecorder = make_unique<StreamingRecorder>(
                dimensions.width,
                dimensions.height,
                options.outputPath
            );
            cout << "Streaming recorder initialized for: " << options.outputPath << endl;
        }

        streamingRecorder->captureFrame();

        // Restore the original window context
        glutSetWindow(currentWindow);
    }
}
