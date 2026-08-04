/*
 *  Created on: Jul 10, 2011
 *      Author: Bill Frasure
 */

#include "graphics/OpenGlSetup.h"

//GUI stuff
#include "Windows/CenterStage.h"

//Physics
#include "Physics/PhysicsSandboxProperties.h"
#include "Physics/ScenarioBuilder.h"
#include "Physics/ScenarioSpec.h"
#include "Physics/Simulations.h"

#include "FullApplication.h"
#include "GlobalApplication.h"
#include "Input/RunOptions.h"
#include "Sandbox/RunReport.h"

#include <csignal>
#include <ctime>
#include <iomanip>
#include <random>
#include <sstream>
#include <sys/stat.h>

void displayFunc() {
    // Empty - work is done in idleFunc
}

void idleFunc() {
    auto result = globalFullApplication->update();

    switch (result) {
        case SUCESSFUL_STEP:
            break;
        case COMPLETED:
            exit(0);
    }

    globalFullApplication->display();
}

//! Ctrl-C should still produce a playable video and a run report.
void handleInterrupt(int) {
    FullApplication::requestStop();
}

string defaultOutputPath() {
    mkdir("./WorthyVideos", 0755);

    const time_t now = time(nullptr);
    ostringstream path;
    path << "./WorthyVideos/" << put_time(localtime(&now), "%F %T") << ".mp4";
    return path.str();
}

int main(int argcp, char **argv) {
    RunOptions options(argcp, argv);

    PhysicsSandboxProperties properties(options.configPath);

    // A run is reproducible from its seed, so record whichever one we used.
    if (options.seed == 0) {
        std::random_device entropy;
        options.seed = entropy();
    }
    std::mt19937 rng(options.seed);

    ScenarioSpec scenario = ScenarioParser::parse(properties.raw(), properties, options.seed);

    SetupDiagnostics diagnostics;
    Simulation simulation = Simulations::fromScenario(scenario, properties, diagnostics, rng);

    cout << "scenario=" << scenario.name
         << " type=" << scenario.type
         << " seed=" << options.seed
         << " groups=" << scenario.groups.size() << endl;
    cout << "setup " << diagnostics.toJson() << endl;

    if (options.printSetupOnly) {
        return 0;
    }

    if (options.outputPath.empty()) {
        options.outputPath = defaultOutputPath();
    }

    RunReport report(options, properties.raw(), scenario, diagnostics);

    auto windowDimensions = WindowDimensions(400, 50, options.height, options.width);

    auto idleFunction = []() {
    };
    OpenGlSetup openGlSetup{};
    openGlSetup.initialize(
        windowDimensions,
        idleFunction
    );

    globalFullApplication = make_unique<FullApplication>(
            options,
            windowDimensions,
            properties,
            openGlSetup,
            std::move(simulation),
            scenario,
            &report
    );

    signal(SIGINT, handleInterrupt);
    signal(SIGTERM, handleInterrupt);

    glutSetWindow(1);
    glutDisplayFunc([]() {
//        globalFullApplication->display();
    });
    glutSetWindow(2);
    glutDisplayFunc([]() {
        displayFunc();
    });

    // Use idle function to continuously trigger redraws
    glutIdleFunc(idleFunc);

    glutMainLoop();

    return 0;
}
