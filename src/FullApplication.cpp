//
// Created by bfrasure on 1/8/19.
//

#include "FullApplication.h"

FullApplication::FullApplication(const SimulationPtr_t &simulation, const ControlCenter controlCenter,
                                 const CenterStage mainDisplay, const shared_ptr<Recorder> recorder,
                                 const time_point<chrono::system_clock, chrono::duration<long, ratio<1, 1000000000>>> start,
                                 const chrono::seconds maximumRuntime, GraphicalOperations graphicalOperations)
        : globalSimulation(simulation), globalControlCenter(controlCenter),
          globalMainDisplay(mainDisplay), globalRecorder(recorder), start(start),
          maximumRuntime(maximumRuntime),
          graphicalOperations(graphicalOperations){}

void FullApplication::update() {
        if (! globalControlCenter.isPaused() ) {
            auto dt = globalControlCenter.getDt();
            globalSimulation->update(dt);
            globalMainDisplay.update(dt.value());

            if ( globalRecorder ) {
                globalRecorder->captureThisFrame(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
            }
        }

        // Should just directly call Observer::getCurObserverInstance()
        auto observer = Observer::getCurObserver();
        observer->update();

        // TODO This would be more valuable if it only tried to include the largest N items.
        // It shouldn't pan out to catch every last tiny particle that gets thrown towards infinity.
        observer->calcMinPullback(globalSimulation->getXYMinsAndMaxes());

        time_point end = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsed_seconds = end-start;
        if ( elapsed_seconds > (maximumRuntime)) {
            if ( globalRecorder ) {
                globalRecorder->createVideo();
            }
            exit(0);
        }
    }

