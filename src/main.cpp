/*
 *  Created on: Jul 10, 2011
 *      Author: Bill Frasure
 */

#include "graphics/GraphicalOperations.h"
#include "graphics/OpenGlSetup.h"

//GUI stuff
#include "Windows/ControlCenter.h"
#include "Windows/CenterStage.h"

//Physics
#include "Physics/Simulations.h"
#include "Physics/PhysicsSandboxProperties.h"

#include "FullApplication.h"
#include "Input/ParameterArguments.h"

#include <chrono>
#include <iomanip>

using std::chrono::time_point;
using std::chrono::system_clock;

unique_ptr<FullApplication> globalFullApplication;

void displayFunc() {
    auto result = globalFullApplication->update();
    switch (result) {
        case SUCESSFUL_STEP:
            break;
        case COMPLETED:
            cout << "Should make new Simulation now." << endl;
            exit(0);
    }
    // TODO Should update ControlCenter every time, but only re-render simulation in the idle function
    globalFullApplication->graphicalOperations.fullDisplay();
}

int main(int argcp, char **argv) {
    char simulationParameter = argv[2][0]; // TODO I should convert to an Enum immediately, so I can safely pass that to the Simulations class
    PhysicsSandboxProperties properties("simulation.properties");
    Simulation simulation = Simulations::createSimulation(simulationParameter, properties);

    auto windowDimensions =
            WindowDimensions(
                    100,
                    50,
                    720,
                    1280
            );

    time_point start = system_clock::now();
    ParameterArguments parameterArguments;
    bool shouldRecord = parameterArguments.parseRecordingParameters(argv);

    auto idleFunction = []() {
    };
    OpenGlSetup openGlSetup{};
    openGlSetup.initialize(
        windowDimensions,
        idleFunction
    );

    ControlCenter controlCenter(hour_t(properties.dt), windowDimensions.width);

    auto outputTime = system_clock::to_time_t(start);
    CenterStage centerStage(windowDimensions.width, outputTime);

    GraphicalOperations graphicalOperations(
            simulation,
            controlCenter,
            openGlSetup.mainDisplayNum,
            openGlSetup.controlCenterNum,
            windowDimensions);

    globalFullApplication = make_unique<FullApplication>(
            simulation,
            centerStage,
            shouldRecord,
            start,
            properties.maximumRunTime,
            graphicalOperations
    );

    glutDisplayFunc([]() {
        displayFunc();
    });

    glutMainLoop();

    return 0;
}
