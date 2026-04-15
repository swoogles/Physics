//
// Created by bfrasure on 1/8/19.
//

#include "FullApplication.h"
#include "Physics/Simulations.h"
#include <chrono>
#include <iomanip>
using namespace std;
using std::chrono::time_point;
using std::chrono::time_point_cast;


FullApplication::FullApplication(bool shouldRecord,
                                 WindowDimensions windowDimensions, PhysicsSandboxProperties properties,
                                 OpenGlSetup openGlSetup)
        : simulation(
        Simulations().bodyFormationCollision(properties)
        ),
        start(system_clock::now()),
        controlCenter(hour_t(properties.dt), windowDimensions.width, start),
          centerStage (windowDimensions.width, system_clock::to_time_t(time_point_cast<system_clock::duration>(start))),
          recorder(Recorder()),
          recording(shouldRecord),
          maximumRuntime(properties.maximumRunTime * 5),
          graphicalOperations(
                  openGlSetup.mainDisplayNum,
                  openGlSetup.controlCenterNum,
                  windowDimensions)
{
    // Intentionally no default scripted camera actions.
    // Keep timedSceneActions in place so scripted camera paths can be re-enabled later.

    // Note: StreamingRecorder will be lazily initialized on first frame capture
    // to ensure OpenGL context is ready
}

ApplicationResult FullApplication::update() {
    if (! controlCenter.isPaused() ) {
        auto dt = controlCenter.getDt();
        simulation.update(dt);
        centerStage.update(dt.value());
    }
    graphicalOperations.updateObserver(simulation.getXYMinsAndMaxes());

    time_point end = system_clock::now();

    duration<double> elapsed_seconds = end-start;
    if ( elapsed_seconds > (maximumRuntime)) {
        if ( recording && streamingRecorder ) {
            cout << "Maximum runtime reached - finalizing video..." << endl;
            streamingRecorder->finalize();
        }

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
            time_t startTime = system_clock::to_time_t(time_point_cast<system_clock::duration>(start));
            ostringstream outputPath;
            outputPath << "./WorthyVideos/" << std::put_time(std::localtime(&startTime), "%F %T") << ".mp4";

            auto dimensions = graphicalOperations.currentDimensions();
            streamingRecorder = make_unique<StreamingRecorder>(
                dimensions.width,
                dimensions.height,
                outputPath.str()
            );
            cout << "Streaming recorder initialized for: " << outputPath.str() << endl;
        }

        streamingRecorder->captureFrame();

        // Restore the original window context
        glutSetWindow(currentWindow);
    }
}
