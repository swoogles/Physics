/*
 *  Created on: Jul 10, 2011
 *      Author: Bill Frasure
 */

#include <GL/glut.h>
#include <GL/glu.h>

#include <float.h>

#include "inputFunctions.h"

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

void myTimer(int v) {
  glutPostRedisplay();
  glutTimerFunc(1000/FPS, myTimer, v);
}

static int main_window;

static int control_center_num;

// GLOBALS
SimulationPtr_t globalSimulation;

control_center globalControlCenter;
main_window_UI globalMainDisplay;

// You want to avoid passing argument to this method, because it would slow down every single
// call.
// TODO Allow an arbitrary list of objects to be displayed
void display()
{
  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  Observer * curObserver = Observer::getCurObserver();
  curObserver->getView();

  glMatrixMode(GL_MODELVIEW);
  Drawing::drawShapes(globalSimulation->getPhysicalObjects().getShapes());

  function<void(Quadrant)> drawingFunc = [] (Quadrant quadrant) {
      Drawing::draw(quadrant.getBorders());
  };
  globalSimulation->getQuadrant()->applyToAllChildren(drawingFunc);


  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  puDisplay();
  glutSwapBuffers();
  glutPostRedisplay();
}

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

void init(char simulation) {
  BillProperties billProperties("simulation.properties");

  PhysicsSandboxProperties properties(billProperties);

  cout << "dt: " << properties.dt << endl;
  globalSimulation = Simulations::createSimulation(simulation, properties);
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

void mainGlut(int argcp, char **argv) {
  size_t mainWinPosX = 100;
  size_t mainWinPosY = 50;
  size_t mainWinHeight = 720;
  size_t mainWinWidth = 1280;

  glutInit(&argcp, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(mainWinPosX,mainWinPosY);
  glutInitWindowSize(mainWinWidth,mainWinHeight);
  main_window = glutCreateWindow("Center Stage");
  glutSetWindow(main_window);
  glutDisplayFunc(display);

  glutMouseFunc(myMouse);
  glutKeyboardFunc(myKey);

  glutIdleFunc(idle);
  glutTimerFunc(1000, myTimer, FPS);

  configureControlWindow(
      mainWinPosX,
      mainWinPosY,
      mainWinHeight,
      mainWinWidth
  );
}

void postSimulationGlInit() {
  glutDisplayFunc(controlDisplay);
  glutMouseFunc(myMouse);
  glutKeyboardFunc(myKey);
}

int main(int argcp, char **argv) {
  char simulation = argv[2][0];

  init( simulation );

  openGlInit();
  mainGlut(argcp, argv);
  puInit();

  Observer::init();
  Observer * curObserver = Observer::getCurObserver();

  determineViewBasedOnSimulation(*globalSimulation, curObserver);

  //Creates main menu bar
  globalMainDisplay.init(glutGet(GLUT_WINDOW_WIDTH));

  control_center_num = glutCreateWindow("Control Center");

  postSimulationGlInit();

  globalControlCenter.init(globalSimulation->getDT(), glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

  glutMainLoop();

  return 0;
}
