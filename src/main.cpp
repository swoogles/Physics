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

enum class ResolutionPreset {
    HD_720P,
    FHD_1080P,
    QHD_1440P,
    UHD_4K
};

WindowDimensions windowDimensionsFor(ResolutionPreset preset, int xPos, int yPos) {
    switch (preset) {
        case ResolutionPreset::HD_720P:
            return WindowDimensions(xPos, yPos, 720, 1280);
        case ResolutionPreset::FHD_1080P:
            return WindowDimensions(xPos, yPos, 1080, 1920);
        case ResolutionPreset::QHD_1440P:
            return WindowDimensions(xPos, yPos, 1440, 2560);
        case ResolutionPreset::UHD_4K:
            return WindowDimensions(xPos, yPos, 2160, 3840);
    }

    return WindowDimensions(xPos, yPos, 1080, 1920);
}

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

    // Change this single preset to switch rendering resolution.
    constexpr auto resolutionPreset = ResolutionPreset::FHD_1080P;
    auto windowDimensions = windowDimensionsFor(resolutionPreset, 400, 50);

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
