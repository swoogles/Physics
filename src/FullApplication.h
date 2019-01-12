//
// Created by bfrasure on 1/8/19.
//

#ifndef PHYSICS_FULLAPPLICATION_H
#define PHYSICS_FULLAPPLICATION_H

#include "graphics/GraphicalOperations.h"
#include "graphics/Recorder.h"

//GUI stuff
#include "Windows/ControlCenter.h"
#include "Windows/CenterStage.h"

//Observers
#include "Observation/Observer.h"

//Physics
#include "Physics/Simulations.h"
#include "Physics/PhysicsSandboxProperties.h"

#include <lib/pstream.h>

#include <chrono>
#include <iomanip>


class FullApplication {
public:
    FullApplication(const SimulationPtr_t &globalSimulation, const ControlCenter globalControlCenter,
                        const CenterStage globalMainDisplay, const shared_ptr<Recorder> globalRecorder,
                        const time_point<chrono::system_clock, chrono::duration<long, ratio<1, 1000000000>>> start,
                        const chrono::seconds maximumRuntime, shared_ptr<GraphicalOperations> graphicalOperations);

    const SimulationPtr_t globalSimulation;
    const ControlCenter globalControlCenter;
    CenterStage globalMainDisplay;
    const shared_ptr<Recorder> globalRecorder;
    const time_point<std::chrono::_V2::system_clock, std::chrono::duration<long int, std::ratio<1, 1000000000> >> start;
    const std::chrono::seconds maximumRuntime;
    const shared_ptr<GraphicalOperations> graphicalOperations;
    void update();
};


#endif //PHYSICS_FULLAPPLICATION_H
