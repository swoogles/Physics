/*
 *  Created on: Jul 10, 2011
 *      Author: Bill Frasure
 */

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

#include "FullApplication.h"

#include <lib/pstream.h>

#include <chrono>
#include <iomanip>

using std::chrono::time_point;

// GLOBALS
SimulationPtr_t globalSimulation;

unique_ptr<FullApplication> globalFullApplication;

void idle() {
    globalFullApplication->update();
}

int main(int argcp, char **argv) {
    char simulation = argv[2][0];
    PhysicsSandboxProperties properties("simulation.properties");
    globalSimulation = Simulations::createSimulation(simulation, properties);

    auto windowDimensions =
            WindowDimensions(
                    100,
                    50,
                    720,
                    1280
            );

    time_point start = std::chrono::system_clock::now();
    shared_ptr<Recorder> localRecorder;
    char recording = argv[1][0];
    if ( recording == 'r') {
        localRecorder = make_shared<Recorder>(std::chrono::system_clock::to_time_t(start));
    } else if (recording == 'x') {
        localRecorder = nullptr;
    } else {
        cout << "Bad recording value! Must be 'x' or 'r'" << endl;
        exit(1);
    }

    auto observer = Observer::init(windowDimensions);


    //Creates main menu bar

    CenterStage mainDisplay(windowDimensions.width, localRecorder);

    InputFunctions::init(observer);

    ControlCenter localControlCenter(hour_t(properties.dt), windowDimensions.width);

    GraphicalOperations graphicalOperations(
//            [](){globalFullApplication->update();},
idle,
            globalSimulation,
            localControlCenter,
            observer,
            windowDimensions
    );
    graphicalOperations.postSimulationGlInit();


    return 0;
}
