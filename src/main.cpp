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
    globalFullApplication->display();
}

int main(int argcp, char **argv) {
    PhysicsSandboxProperties properties("simulation.properties");

    ParameterArguments parameterArguments(argv);

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

    ControlCenter controlCenter(hour_t(properties.dt), windowDimensions.width, start);

    auto outputTime = system_clock::to_time_t(start);
    CenterStage centerStage(windowDimensions.width, outputTime);

    Simulations simulations;
    globalFullApplication = make_unique<FullApplication>(
//            simulations.bodyFormation(properties.numShapes, properties),
            simulations.bodyFormationCollision(properties),
//            Simulations::QuadrantTesting_simplest(),
            centerStage,
            parameterArguments.isRecording(),
            start,
            properties.maximumRunTime,
            windowDimensions,
            properties,
            openGlSetup
    );

    glutSetWindow(1);
    glutDisplayFunc([]() {
//        globalFullApplication->display();
    });
    glutSetWindow(2);
    glutDisplayFunc([]() {
        displayFunc();
    });

    glutMainLoop();

    return 0;
}
