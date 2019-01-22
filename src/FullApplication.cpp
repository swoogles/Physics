//
// Created by bfrasure on 1/8/19.
//

#include "FullApplication.h"

FullApplication::FullApplication(
        Simulation &simulation,
        CenterStage mainDisplay,
        bool shouldRecord,
        time_point<chrono::system_clock, chrono::duration<long, ratio<1, 1000000000>>> start,
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

void FullApplication::update() {
    // TODO ah! I should look at scrolling status of ControlCenter here, and *then* take action on Observer.
        if (! controlCenter.isPaused() ) {
            auto dt = controlCenter.getDt();
            simulation.update(dt);
            centerStage.update(dt.value());

            if ( recording ) {
                auto dimensions = graphicalOperations.currentDimensions();
                recorder.captureThisFrame(dimensions.width, dimensions.height);
            }
        }
        auto mouseAction = InputFunctions::currentMouseAction();
        if (mouseAction.has_value()) {
            cout << "MouseAction value: " << mouseAction.value() << endl;
            auto observer = graphicalOperations.localObserver;
            switch(mouseAction.value()) {
                case MouseAction::SCROLL_UP:
                    observer->zoomIn();
                    break;
                case MouseAction::SCROLL_DOWN:
                    observer->zoomOut();
                    break;
            }
            observer->setAutoScaling(false);
        }

    auto cameraAction = ControlCenter::currentCameraAction();
    if (cameraAction.has_value()) {
        cout << "CameraAction value: " << cameraAction.value() << endl;
        auto observer = graphicalOperations.localObserver;
        switch(cameraAction.value()) {
            case CameraAction::ROTATE_LEFT: {
                PhysicalVector leftAngVelocity(0, -.5f, 0);
                observer->adjustAngularVelocity(leftAngVelocity);
                break;
            }
            case CameraAction::ROTATE_RIGHT: {
                PhysicalVector rightAngVelocity(0, .5f, 0);
                observer->adjustAngularVelocity(rightAngVelocity);
                break;
            }
            case CameraAction::ROTATE_UP: {
                PhysicalVector upAngVelocity(+0.5f, 0, 0);
                observer->adjustAngularVelocity(upAngVelocity);
                break;
            }
            case CameraAction::ROTATE_DOWN: {
                PhysicalVector downAngVelocity(-0.5f, 0, 0);
                observer->adjustAngularVelocity(downAngVelocity);
            }
            case TOGGLE_AUTOSCALING: {
                break;
            }
            case STOP_ROTATION: {
                PhysicalVector stoppedAngVelocity(0,0,0);
                observer->adjustAngularVelocity(stoppedAngVelocity);
                break;
            }
        }
        }

        // Should just directly call Observer::getCurObserverInstance()
        auto observer = graphicalOperations.localObserver;
        observer->update();

        // TODO This would be more valuable if it only tried to include the largest N items.
        // It shouldn't pan out to catch every last tiny particle that gets thrown towards infinity.
        observer->calcMinPullback(simulation.getXYMinsAndMaxes());

        time_point end = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsed_seconds = end-start;
        if ( elapsed_seconds > (maximumRuntime)) {
            if ( recording ) {
                FfmpegClient client;
                client.createVideo(std::chrono::system_clock::to_time_t(start));
                exit(0);
            }
        }
    }

