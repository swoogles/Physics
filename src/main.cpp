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
    globalFullApplication->graphicalOperations.fullDisplay();
}

int main(int argcp, char **argv) {
    PhysicsSandboxProperties properties("simulation.properties");

    ParameterArguments parameterArguments(argv);
    Simulation simulation =
            Simulations::createSimulation(parameterArguments.getCraftedSimulation(), properties);

    auto windowDimensions =
            WindowDimensions(
                    100,
                    50,
                    720,
                    1280
            );

    time_point start = system_clock::now();

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
            parameterArguments.isRecording(),
            start,
            properties.maximumRunTime,
            graphicalOperations
    );

    glutSetWindow(1);
    glutDisplayFunc([]() {
        globalFullApplication->graphicalOperations.fullDisplay();
    });
    glutSetWindow(2);
    glutDisplayFunc([]() {
        displayFunc();
    });

    glutMainLoop();

    return 0;
}
