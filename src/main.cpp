/*
 *  Created on: Jul 10, 2011
 *      Author: Bill Frasure
 */

#include "graphics/GraphicalOperations.h"
#include "graphics/Recorder.h"

//GUI stuff
#include "Windows/control_center.h"
#include "Windows/main_display.h"

//Observers
#include "Observation/Observer.h"

//Physics
#include "Physics/Simulations.h"
#include "graphics/WorldSettings.h"
#include "Physics/PhysicsSandboxProperties.h"

#include <lib/pstream.h>

#include <chrono>
#include <iomanip>

using std::chrono::time_point;

// GLOBALS
SimulationPtr_t globalSimulation;

control_center globalControlCenter;
main_window_UI globalMainDisplay;

shared_ptr<Recorder> globalRecorder;

time_point start = std::chrono::system_clock::now();

void idle() {
  auto dt = globalControlCenter.getDt();
  if (! globalControlCenter.isPaused() ) {
    globalSimulation->update(dt);
    globalMainDisplay.update(dt.value());

    if ( globalRecorder ) {
      globalRecorder->captureThisFrame(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    }
  }

  // Should just directly call Observer::getCurObserverInstance()
  auto observer = Observer::getCurObserver();
  observer->update();

  if (WorldSettings::isAutoScaling()) {
    observer->calcMinPullback(globalSimulation->getXYMinsAndMaxes());
  }

  time_point end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;
  std::chrono::seconds sec(30);
  if ( elapsed_seconds > sec) {
    std::time_t now_c = std::chrono::system_clock::to_time_t(start);
    if ( globalRecorder ) {
      globalRecorder->createVideo(now_c);
    }
    exit(0);
  }
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

  char recording = argv[1][0];
  if ( recording == 'r') {
    globalRecorder = make_shared<Recorder>();
  } else if (recording == 'x') {

  } else {
    cout << "Bad recording value! Must be 'x' or 'r'" << endl;
    exit(1);
  }

  auto observer = Observer::init(windowDimensions);
  observer->calcMinPullback(globalSimulation->getXYMinsAndMaxes());

  GraphicalOperations graphicalOperations;
  graphicalOperations.mainGlut(
          idle,
          globalSimulation,
          globalControlCenter,
          observer,
          windowDimensions
  );

  //Creates main menu bar
  globalMainDisplay.init(windowDimensions.width);

  InputFunctions::init(observer);

  globalControlCenter.init(hour_t(properties.dt), windowDimensions.width);

  graphicalOperations.postSimulationGlInit();

  return 0;
}
