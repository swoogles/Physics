/*
 *  Created on: Jul 10, 2011
 *      Author: Bill Frasure
 */

#include <GL/glut.h>
#include <GL/glu.h>

#include <float.h>

#include "inputFunctions.h"
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

#include "ShapeFiles/Drawing.h"
#include "BillProperties.h"

#include <functional>

#define FPS 1

using std::size_t;

static int main_window;

static int control_center_num;

// GLOBALS
SimulationPtr_t globalSimulation;

control_center globalControlCenter;
main_window_UI globalMainDisplay;

void controlDisplay() {
  glutSetWindow(control_center_num);
  glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  puDisplay(control_center_num);

  glutSwapBuffers();
  glutPostRedisplay();
}

PhysicsSandboxProperties init(char simulation) {
  BillProperties billProperties("simulation.properties");

  PhysicsSandboxProperties properties(billProperties);

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
    Observer::getCurObserverRef().calcMinPullback( 45.0, globalSimulation->getXYMinsAndMaxes());
  }

}

void postSimulationGlInit() {
  glutDisplayFunc(controlDisplay);
  glutMouseFunc(InputFunctions::myMouse);
  glutKeyboardFunc(InputFunctions::myKey);
}

int main(int argcp, char **argv) {
  char simulation = argv[2][0];

  auto properties = init( simulation );

  auto windowDimensions = WindowDimensions(
    100,
    50,
    720,
    1280
  );

  Observer::init(windowDimensions);
  Observer::getCurObserverRef().calcMinPullback( 45.0, globalSimulation->getXYMinsAndMaxes());

  GraphicalOperations graphicalOperations;
  main_window = graphicalOperations.mainGlut(argcp, argv, idle, globalSimulation, globalControlCenter,
                                             Observer::getCurObserver(), windowDimensions);

  //Creates main menu bar
  globalMainDisplay.init(glutGet(GLUT_WINDOW_WIDTH));

  control_center_num = glutCreateWindow("Control Center");

  // TODO InputFunction->init(Observer)
  InputFunctions::init(Observer::getCurObserver());
  postSimulationGlInit();

  globalControlCenter.init(properties.dt, glutGet(GLUT_WINDOW_WIDTH));

  glutMainLoop();

  return 0;
}
