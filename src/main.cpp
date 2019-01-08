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

#include <lib/pstream.h>

#include <chrono>
#include <iomanip>

using std::chrono::time_point;

// GLOBALS
SimulationPtr_t globalSimulation;

ControlCenter globalControlCenter;
CenterStage globalMainDisplay;

shared_ptr<Recorder> globalRecorder;

time_point start = std::chrono::system_clock::now();

std::chrono::seconds maximumRuntime(60);

void idle() {
  if (! globalControlCenter.isPaused() ) {
    auto dt = globalControlCenter.getDt();
    globalSimulation->update(dt);
    globalMainDisplay.update(dt.value());

    if ( globalRecorder ) {
      globalRecorder->captureThisFrame(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    }
  }

  // Should just directly call Observer::getCurObserverInstance()
  auto observer = Observer::getCurObserver();
  observer->update();

  // TODO This would be more valuable if it only tried to include the largest N items.
  // It shouldn't pan out to catch every last tiny particle that gets thrown towards infinity.
  observer->calcMinPullback(globalSimulation->getXYMinsAndMaxes());

  time_point end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;
  if ( elapsed_seconds > (maximumRuntime)) {
    if ( globalRecorder ) {
        globalRecorder->createVideo();
    }
    exit(0);
  }
}

int main(int argcp, char **argv) {
    char simulation = argv[2][0];
    PhysicsSandboxProperties properties("simulation.properties");
    globalSimulation = Simulations::createSimulation(simulation, properties);
    maximumRuntime = properties.maximumRunTime;

    auto windowDimensions =
            WindowDimensions(
                    100,
                    50,
                    720,
                    1280
            );

    char recording = argv[1][0];
    if ( recording == 'r') {
        globalRecorder = make_shared<Recorder>(std::chrono::system_clock::to_time_t(start));
    } else if (recording == 'x') {
        globalRecorder = nullptr;
    } else {
        cout << "Bad recording value! Must be 'x' or 'r'" << endl;
        exit(1);
    }

    auto observer = Observer::init(windowDimensions);

    GraphicalOperations graphicalOperations(
            idle,
            globalSimulation,
            globalControlCenter,
            observer,
            windowDimensions
    );

    //Creates main menu bar
    globalMainDisplay.init(windowDimensions.width, globalRecorder);

    InputFunctions::init(observer);

    globalControlCenter.init(hour_t(properties.dt), windowDimensions.width);

    graphicalOperations.postSimulationGlInit();

    return 0;
}
