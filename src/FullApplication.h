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

#include "graphics/OpenGlSetup.h"

//Observers
#include "Observation/Observer.h"

//Physics
#include "Physics/Simulations.h"
#include "Physics/PhysicsSandboxProperties.h"

#include "Sandbox/ApplicationResult.h"

#include <lib/pstream.h>

#include <chrono>
#include <iomanip>

using std::chrono::time_point;
using std::chrono::duration;
using std::chrono::system_clock;

class FullApplication {
public:
    FullApplication(bool shouldRecord,
                    WindowDimensions windowDimensions, PhysicsSandboxProperties properties,
                    OpenGlSetup openGlSetup);

    Simulation simulation;
    const ControlCenter controlCenter;
    CenterStage centerStage;
    Recorder recorder;
    const bool recording;
    const time_point<system_clock, duration<long int, std::ratio<1, 1000000000> >> start;
    const std::chrono::seconds maximumRuntime;
    void display() ;
    GraphicalOperations graphicalOperations;
    // TODO Instead of void, return a Result[SUCCESSFUL_STEP, COMPLETED, FAILED]
    ApplicationResult update();
};


#endif //PHYSICS_FULLAPPLICATION_H
