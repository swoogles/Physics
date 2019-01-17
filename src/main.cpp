/*
 *  Created on: Jul 10, 2011
 *      Author: Bill Frasure
 */

#include "graphics/GraphicalOperations.h"
#include "graphics/OpenGlSetup.h"
#include "graphics/Recorder.h"

//GUI stuff
#include "Windows/ControlCenter.h"
#include "Windows/CenterStage.h"

//Physics
#include "Physics/Simulations.h"
#include "Physics/PhysicsSandboxProperties.h"

#include "FullApplication.h"

#include <chrono>
#include <iomanip>

using std::chrono::time_point;

unique_ptr<FullApplication> globalFullApplication;

int main(int argcp, char **argv) {
    char simulationParameter = argv[2][0];
    PhysicsSandboxProperties properties("simulation.properties");
    SimulationPtr_t simulation = Simulations::createSimulation(simulationParameter, properties);

    auto windowDimensions =
            WindowDimensions(
                    100,
                    50,
                    720,
                    1280
            );

    time_point start = std::chrono::system_clock::now();
    shared_ptr<Recorder> recorder;
    char recording = argv[1][0];
    if ( recording == 'r') {
        recorder = make_shared<Recorder>(std::chrono::system_clock::to_time_t(start));
    } else if (recording == 'x') {
        recorder = nullptr;
    } else {
        cout << "Bad recording value! Must be 'x' or 'r'" << endl;
        exit(1);
    }

    auto idleFunction = []() { globalFullApplication->update(); };
    OpenGlSetup openGlSetup{};
    openGlSetup.initialize(
        windowDimensions,
        idleFunction
    );

    ControlCenter controlCenter(hour_t(properties.dt), windowDimensions.width);

    CenterStage centerStage(windowDimensions.width, recorder);

    GraphicalOperations graphicalOperations(
            simulation,
            controlCenter,
            openGlSetup.mainDisplayNum,
            openGlSetup.controlCenterNum,
            windowDimensions);

    globalFullApplication = make_unique<FullApplication>(simulation,
                                                         centerStage,
                                                         recorder,
                                                         start,
                                                         properties.maximumRunTime,
                                                         graphicalOperations
    );

    glutDisplayFunc([]() {
        globalFullApplication->graphicalOperations.fullDisplay();
    });

    glutMainLoop();

    return 0;
}
