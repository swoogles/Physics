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
    FullApplication(Simulation &simulation, CenterStage centerStage,
                    shared_ptr<Recorder> recorder,
                    time_point<chrono::system_clock, chrono::duration<long, ratio<1, 1000000000>>> start,
                    chrono::seconds maximumRuntime, GraphicalOperations graphicalOperations);

    Simulation simulation;
    const ControlCenter controlCenter;
    CenterStage centerStage;
    const shared_ptr<Recorder> recorder;
    const time_point<std::chrono::_V2::system_clock, std::chrono::duration<long int, std::ratio<1, 1000000000> >> start;
    const std::chrono::seconds maximumRuntime;
    // TODO reinstate const-ness
    GraphicalOperations graphicalOperations;
    void update();
};


#endif //PHYSICS_FULLAPPLICATION_H
