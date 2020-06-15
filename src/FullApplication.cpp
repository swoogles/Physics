//
// Created by bfrasure on 1/8/19.
//

#include "FullApplication.h"
#include "Physics/Simulations.h"


FullApplication::FullApplication(CenterStage mainDisplay, bool shouldRecord,
                                 time_point<system_clock, duration<long, ratio<1, 1000000000>>> start,
                                 WindowDimensions windowDimensions, PhysicsSandboxProperties properties,
                                 OpenGlSetup openGlSetup)
        : simulation(
        // The way this is invoked is a smell. Should probably just be a free-floating function.
        Simulations().bodyFormationCollision(properties)
        ),
          controlCenter(hour_t(properties.dt), windowDimensions.width, start),
          centerStage(mainDisplay),
          recorder(Recorder()),
          recording(shouldRecord),
          start(start),
          maximumRuntime(properties.maximumRunTime),
          graphicalOperations(
                  this->controlCenter,
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

