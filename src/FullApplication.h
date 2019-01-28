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

using std::chrono::time_point;
using std::chrono::duration;
using std::chrono::system_clock;

class FullApplication {
public:
    FullApplication(
        Simulation &simulation,
        CenterStage centerStage,
        bool shouldRecord,
        time_point<system_clock, duration<long, ratio<1, 1000000000>>> start,
        chrono::seconds maximumRuntime,
        GraphicalOperations graphicalOperations
    );

    Simulation simulation;
    const ControlCenter controlCenter;
    CenterStage centerStage;
    Recorder recorder;
    const bool recording;
    const time_point<system_clock, duration<long int, std::ratio<1, 1000000000> >> start;
    const std::chrono::seconds maximumRuntime;
    // TODO reinstate const-ness
    GraphicalOperations graphicalOperations;
    void update();
};


#endif //PHYSICS_FULLAPPLICATION_H
