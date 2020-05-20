//
// Created by bfrasure on 1/8/19.
//

#include "FullApplication.h"

void processMouseAction(
        Observer & observer,
        MouseAction  mouseAction
) {
    switch(mouseAction) {
        case MouseAction::SCROLL_UP:
            observer.zoomIn();
            break;
        case MouseAction::SCROLL_DOWN:
            observer.zoomOut();
            break;
    }
    observer.setAutoScaling(false);
}

void processCameraAction(
        Observer & observer,
        CameraAction cameraAction
) {
    switch(cameraAction) {
        case CameraAction::ROTATE_LEFT: {
            PhysicalVector leftAngVelocity(0, -.5f, 0);
            observer.adjustAngularVelocity(leftAngVelocity);
            break;
        }
        case CameraAction::ROTATE_RIGHT: {
            PhysicalVector rightAngVelocity(0, .5f, 0);
            observer.adjustAngularVelocity(rightAngVelocity);
            break;
        }
        case CameraAction::ROTATE_UP: {
            PhysicalVector upAngVelocity(+0.5f, 0, 0);
            observer.adjustAngularVelocity(upAngVelocity);
            break;
        }
        case CameraAction::ROTATE_DOWN: {
            PhysicalVector downAngVelocity(-0.5f, 0, 0);
            observer.adjustAngularVelocity(downAngVelocity);
        }
        case TOGGLE_AUTOSCALING: {
            break;
        }
        case STOP_ROTATION: {
            PhysicalVector stoppedAngVelocity(0,0,0);
            observer.adjustAngularVelocity(stoppedAngVelocity);
            break;
        }
    }
}

void updateObserver(
        Observer & observer,
        MaximumValues maximumValues
) {
    auto mouseAction = InputFunctions::currentMouseAction();
    if (mouseAction.has_value()) {
        processMouseAction(observer, mouseAction.value());
    }

    auto cameraAction = ControlCenter::currentCameraAction();
    if (cameraAction.has_value()) {
        processCameraAction(observer, cameraAction.value());
    }

    observer.update();

    // TODO This would be more valuable if it only tried to include the largest N items.
    // It shouldn't pan out to catch every last tiny particle that gets thrown towards infinity.
    observer.calcMinPullback(maximumValues);
}


FullApplication::FullApplication(
        Simulation &simulation,
        CenterStage mainDisplay,
        bool shouldRecord,
        time_point<system_clock, duration<long, ratio<1, 1000000000>>> start,
        chrono::seconds maximumRuntime,
        GraphicalOperations graphicalOperations
) : simulation(simulation),
    controlCenter(graphicalOperations.localControlCenter),
    centerStage(mainDisplay),
    recorder(Recorder()),
    recording(shouldRecord),
    start(start),
    maximumRuntime(maximumRuntime),
    graphicalOperations(graphicalOperations){}

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
    updateObserver(graphicalOperations.getObserver(), simulation.getXYMinsAndMaxes());
    graphicalOperations.getObserver().setAutoScaling(false);


    time_point end = system_clock::now();

    duration<double> elapsed_seconds = end-start;
    if ( elapsed_seconds > (maximumRuntime)) {
        if ( recording ) {
            FfmpegClient client;
            client.createVideo(system_clock::to_time_t(start));
        }

        return ApplicationResult::COMPLETED;
    }
    return ApplicationResult::SUCESSFUL_STEP;
}

