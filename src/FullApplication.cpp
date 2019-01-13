//
// Created by bfrasure on 1/8/19.
//

#include "FullApplication.h"

FullApplication::FullApplication(const SimulationPtr_t &simulation, ControlCenter controlCenter,
                                 CenterStage mainDisplay, shared_ptr<Recorder> recorder,
                                 time_point<chrono::system_clock, chrono::duration<long, ratio<1, 1000000000>>> start,
                                 chrono::seconds maximumRuntime, GraphicalOperations graphicalOperations)
        : simulation(simulation), controlCenter(controlCenter),
          centerStage(mainDisplay), recorder(recorder), start(start),
          maximumRuntime(maximumRuntime),
          graphicalOperations(graphicalOperations){}

void FullApplication::update() {
        if (! controlCenter.isPaused() ) {
            auto dt = controlCenter.getDt();
            simulation->update(dt);
            centerStage.update(dt.value());

            if ( recorder ) {
                auto dimensions = graphicalOperations.currentDimensions();
                recorder->captureThisFrame(dimensions.width, dimensions.height);
            }
        }

        // Should just directly call Observer::getCurObserverInstance()
        auto observer = Observer::getCurObserver();
        observer->update();

        // TODO This would be more valuable if it only tried to include the largest N items.
        // It shouldn't pan out to catch every last tiny particle that gets thrown towards infinity.
        observer->calcMinPullback(simulation->getXYMinsAndMaxes());

        time_point end = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsed_seconds = end-start;
        if ( elapsed_seconds > (maximumRuntime)) {
            if ( recorder ) {
                recorder->createVideo();
            }
            exit(0);
        }
    }

