/*
 *  Created on: Jul 10, 2011
 *      Author: Bill Frasure
 */

#define PU_USE_GLUT 1

#include <GL/glut.h>
#include <GL/glu.h>

#include <float.h>

#include "inputFunctions.h"

//GUI stuff
#include "Windows/control_center.h"
#include "Windows/main_display.h"

//All PLIB includes (What a great library)
#include <plib/sg.h> // A better main class wouldn't be reference sgVecs.
#include <plib/pu.h>

//Observers
#include "Observation/Observer.h"

//Physics
#include "Physics/Interactions.h"
#include "Physics/Simulations.h"
#include "Physics/WorldSettings.h"
#include "Physics/PhysicsSandboxProperties.h"

#include "ShapeFiles/Drawing.h"
#include "BillProperties.h"

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

ShapeList shapes;

// You want to avoid passing argument to this method, because it would slow down every single
// call.
// TODO Allow an arbitrary list of objects to be displayed
void display(void)
{
  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  Observer * curObserver = Observer::getCurObserver();
  curObserver->getView();

  sgVec3 curColor = { 1.0, 0.0, 0.0 };
  glColor3fv(curColor);

  glMatrixMode(GL_MODELVIEW);
  Drawing::drawShapes(shapes.getShapes());

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  puDisplay();
  glutSwapBuffers();
  glutPostRedisplay();
}

void controlDisplay(void) {
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

void determineViewBasedOnSimulation(SimulationPtr_t simulation, Observer * observer) {
  float minX, minY, maxX, maxY;
  simulation->getXYMinsAndMaxes( minX, maxX, minY, maxY );
  observer->calcMinPullback( 45.0, minX, minY, maxX, maxY);
}

void init(char simulation) {
  openGlInit();

  BillProperties billProperties("simulation.properties");

  Observer::init();
  Observer * curObserver = Observer::getCurObserver();

  PhysicsSandboxProperties properties(billProperties);

  globalSimulation = Simulations::createSimulation( simulation, properties.numShapes, properties.forceCalculationMethod);
  // TODO How about just use simulationPtr here?
  shapes = globalSimulation->getPhysicalObjects();

  determineViewBasedOnSimulation(globalSimulation, curObserver);
}

void idle() {

  if (! globalSimulation->isPaused()  ) {
    if (WorldSettings::isAutoScaling())
    {
      globalSimulation->resetXYMinsAndMaxes();
    }

    globalSimulation->update();
    shapes = globalSimulation->getPhysicalObjects();
    globalMainDisplay.update();
  }

  // Should just directly call Observer::getCurObserverInstance()
  Observer * curObserver = Observer::getCurObserver();
  curObserver->update( globalSimulation->getDT() );

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
  mainGlut(argcp, argv);
  puInit();
  char simulation = argv[2][0];

  init( simulation );

  //Creates main menu bar
  globalMainDisplay.init( globalSimulation );

  control_center_num = glutCreateWindow("Control Center");

  postSimulationGlInit();

  globalControlCenter.init( globalSimulation );

  glutMainLoop();

  return 0;
}
