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

#include <chrono>
#include <iomanip>

using std::chrono::time_point;

unique_ptr<FullApplication> globalFullApplication;

bool parseRecordingParameters(char **argv, std::time_t  start) {
    char recording = argv[1][0];
    if ( recording == 'r') {
        return true;
    } else if (recording == 'x') {
        return false;
    } else {
        cout << "Bad recording value! Must be 'x' or 'r'" << endl;
        exit(1);
    }
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

    time_point start = std::chrono::system_clock::now();
    bool shouldRecord = parseRecordingParameters(argv, std::chrono::system_clock::to_time_t(start));

    auto idleFunction = []() { globalFullApplication->update(); };
    OpenGlSetup openGlSetup{};
    openGlSetup.initialize(
        windowDimensions,
        idleFunction
    );

    ControlCenter controlCenter(hour_t(properties.dt), windowDimensions.width);

    auto outputTime = std::chrono::system_clock::to_time_t(start);
    CenterStage centerStage(windowDimensions.width, outputTime);

    GraphicalOperations graphicalOperations(
            simulation,
            controlCenter,
            openGlSetup.mainDisplayNum,
            openGlSetup.controlCenterNum,
            windowDimensions);

    globalFullApplication = make_unique<FullApplication>(simulation,
                                                         centerStage,
                                                         shouldRecord,
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
