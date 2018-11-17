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

#define WW 5
#define WH 5
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

void openGlInit() {
  glViewport(-WW,WW,-WH,WH);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void determineViewBasedOnSimulation(Simulation & simulation, Observer * observer) {
  float minX, minY, maxX, maxY;
  simulation.getXYMinsAndMaxes( minX, maxX, minY, maxY );
  observer->calcMinPullback( 45.0, minX, minY, maxX, maxY);
}

PhysicsSandboxProperties init(char simulation) {
  BillProperties billProperties("simulation.properties");

  PhysicsSandboxProperties properties(billProperties);

  cout << "dt: " << properties.dt << endl;
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
  Observer * curObserver = Observer::getCurObserver();
  curObserver->update(dt);

  // Not sure if I can use Observer the way that I want to here, due to the constaints of the input methods
  if (WorldSettings::isAutoScaling()) {
    float minX, maxX, minY, maxY;
    globalSimulation->getXYMinsAndMaxes( minX, maxX, minY, maxY );
    curObserver->calcMinPullback( 45.0, minX, minY, maxX, maxY);
  }

}

void configureControlWindow(
    int mainWinPosX,
    int mainWinPosY,
    int mainWinHeight,
    int mainWinWidth

) {
  int controlWinPosX = mainWinPosX;
  int controlWinPosY = mainWinPosY + mainWinHeight + 30;
  int controlWinWidth = mainWinWidth;
  int controlWinHeight = 200;


  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(controlWinPosX,controlWinPosY);
  glutInitWindowSize(controlWinWidth,controlWinHeight);
}

void postSimulationGlInit() {
  glutDisplayFunc(controlDisplay);
  glutMouseFunc(myMouse);
  glutKeyboardFunc(myKey);
}

int main(int argcp, char **argv) {
  char simulation = argv[2][0];

  auto properties = init( simulation );

  openGlInit();
  GraphicalOperations graphicalOperations;
  main_window = graphicalOperations.mainGlut(argcp, argv, idle, globalSimulation, globalControlCenter);
//  mainGlut(argcp, argv);
  puInit();

  Observer::init();
  Observer * curObserver = Observer::getCurObserver();

  determineViewBasedOnSimulation(*globalSimulation, curObserver);

  //Creates main menu bar
  globalMainDisplay.init(glutGet(GLUT_WINDOW_WIDTH));

  control_center_num = glutCreateWindow("Control Center");

  postSimulationGlInit();

  globalControlCenter.init(properties.dt, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

  glutMainLoop();

  return 0;
}
