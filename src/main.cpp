/*
 *  Created on: Jul 10, 2011
 *      Author: Bill Frasure
 */

#include "graphics/OpenGlSetup.h"

//GUI stuff
#include "Windows/CenterStage.h"

//Physics
#include "Physics/PhysicsSandboxProperties.h"

#include "FullApplication.h"
#include "Input/ParameterArguments.h"

#include <iomanip>

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
                    400,
                    50,
                    720,
                    1280
            );

    auto idleFunction = []() {
    };
    OpenGlSetup openGlSetup{};
    openGlSetup.initialize(
        windowDimensions,
        idleFunction
    );

    globalFullApplication = make_unique<FullApplication>(
            parameterArguments.isRecording(),
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
