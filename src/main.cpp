/*
 *  Created on: Jul 10, 2011
 *      Author: Bill Frasure
 */

#include "GraphicalOperations.h"

//GUI stuff
#include "Windows/control_center.h"
#include "Windows/main_display.h"

//Observers
#include "Observation/Observer.h"

//Physics
#include "Physics/Simulations.h"
#include "Physics/WorldSettings.h"
#include "Physics/PhysicsSandboxProperties.h"

// GLOBALS
SimulationPtr_t globalSimulation;

control_center globalControlCenter;
main_window_UI globalMainDisplay;

PhysicsSandboxProperties init(char simulation) {
  PhysicsSandboxProperties properties("simulation.properties");

  globalSimulation = Simulations::createSimulation(simulation, properties);
  return properties;
}

void idle() {
  auto dt = globalControlCenter.getDt();
  if (! globalControlCenter.isPaused() ) {
    globalSimulation->update(dt);
    globalMainDisplay.update(dt);
  }

  // Should just directly call Observer::getCurObserverInstance()
  Observer::getCurObserverRef().update(dt);

  // Not sure if I can use Observer the way that I want to here, due to the constaints of the input methods
  if (WorldSettings::isAutoScaling()) {
    Observer::getCurObserverRef().calcMinPullback(globalSimulation->getXYMinsAndMaxes());
  }

}

int main(int argcp, char **argv) {
  char simulation = argv[2][0];

  auto properties = init( simulation );

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
