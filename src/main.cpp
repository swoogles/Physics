/*
 *  Created on: Jul 10, 2011
 *      Author: Bill Frasure
 */

#include "graphics/GraphicalOperations.h"

//GUI stuff
#include "Windows/control_center.h"
#include "Windows/main_display.h"

//Observers
#include "Observation/Observer.h"

//Physics
#include "Physics/Simulations.h"
#include "graphics/WorldSettings.h"
#include "Physics/PhysicsSandboxProperties.h"

// GLOBALS
SimulationPtr_t globalSimulation;

control_center globalControlCenter;
main_window_UI globalMainDisplay;

void idle() {
  auto dt = globalControlCenter.getDt();
  if (! globalControlCenter.isPaused() ) {
    globalSimulation->update(dt);
    globalMainDisplay.update(dt);
  }

  // Should just directly call Observer::getCurObserverInstance()
  auto observer = Observer::getCurObserver();
  observer->update(dt);

  if (WorldSettings::isAutoScaling()) {
    observer->calcMinPullback(globalSimulation->getXYMinsAndMaxes());
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

  globalControlCenter.init(properties.dt, windowDimensions.width);

  graphicalOperations.postSimulationGlInit();

  return 0;
}
