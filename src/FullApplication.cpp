//
// Created by bfrasure on 1/8/19.
//

#include "FullApplication.h"
#include "Physics/Simulations.h"
#include <chrono>
using namespace std;
using std::chrono::time_point;


FullApplication::FullApplication(bool shouldRecord,
                                 WindowDimensions windowDimensions, PhysicsSandboxProperties properties,
                                 OpenGlSetup openGlSetup)
        : simulation(
        // The way this is invoked is a smell. Should probably just be a free-floating function.
        Simulations().bodyFormationCollision(properties)
        ),
        start(system_clock::now()),
        controlCenter(hour_t(properties.dt), windowDimensions.width, start),
          centerStage (windowDimensions.width, system_clock::to_time_t(start)),
          recorder(Recorder()),
          recording(shouldRecord),
          maximumRuntime(properties.maximumRunTime),
          graphicalOperations(
                  openGlSetup.mainDisplayNum,
                  openGlSetup.controlCenterNum,
                  windowDimensions)
            {}

ApplicationResult FullApplication::update() {
    if (! controlCenter.isPaused() ) {
        auto dt = controlCenter.getDt();
        simulation.update(dt);
        centerStage.update(dt.value());

        if ( recording ) {
            glutSetWindow(1); // TODO Find a more reliable way to get this int.
            auto dimensions = graphicalOperations.currentDimensions();
            recorder.captureThisFrame(dimensions.width, dimensions.height);
        }
    }
    graphicalOperations.updateObserver(simulation.getXYMinsAndMaxes());

    time_point end = system_clock::now();

    duration<double> elapsed_seconds = end-start;
    if ( elapsed_seconds > (maximumRuntime)) {
        if ( recording ) {
            FfmpegClient client;
            client.createVideo(system_clock::to_time_t(start));
            client.cleanupFrames();
        }

        return ApplicationResult::COMPLETED;
    }
    return ApplicationResult::SUCESSFUL_STEP;
}

void FullApplication::display() {
    this->graphicalOperations.fullDisplay(simulation);
}

