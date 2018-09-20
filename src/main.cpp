/*
 *  Created on: Jul 10, 2011
 *      Author: Bill Frasure
 */

#define PU_USE_GLUT 1

#include <GL/glut.h>
#include <GL/glu.h>

#include <boost/foreach.hpp>

#include <float.h>
#include <string>

#include "BillProperties.h"
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

#include "ShapeFiles/Drawing.h"

#define WW 5
#define WH 5
#define FPS 1

#define foreach_  BOOST_FOREACH                                                                                              

using boost::shared_ptr;
using boost::make_shared; // TODO Target this for removal

void myTimer(int v) {
  glutPostRedisplay();
  glutTimerFunc(1000/FPS, myTimer, v);
}

static int main_window;

static int control_center_num;

using boost::numeric::ublas::compressed_vector;

// GLOBALS
SimulationPtr_t globalSimulation;
boost::shared_ptr<BillProperties> globalProperties;

control_center globalControlCenter;
main_window_UI globalMainDisplay;

compressed_vector<shapePointer_t> shapes;

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
  Drawing::drawShapes(shapes);

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
  globalSimulation->getXYMinsAndMaxes( minX, maxX, minY, maxY );
  observer->calcMinPullback( 45.0, minX, minY, maxX, maxY);
}

void init(char simulation) {
  openGlInit();

  globalProperties = boost::make_shared<BillProperties>();
  globalProperties->readProperties();

  Observer::init();
  Observer * curObserver = Observer::getCurObserver();

  int numShapes = atoi(globalProperties->at( BillProperties::NUM_SHAPES ).c_str()); //value = 45

  // Determine and create simulation
  globalSimulation = Simulations::createSimulation( simulation, numShapes );
  shapes = globalSimulation->getPhysicalObjects().getShapes() ;

  globalSimulation->setForceCalcMethodByString( globalProperties->at( BillProperties::FORCE_CALCULATION_METHOD ) );
  globalSimulation->updateMinsAndMaxes();
  determineViewBasedOnSimulation(globalSimulation, curObserver);

  // This is the first "n" part in "n log(n)"
  globalSimulation->refreshQuadrant();
}

void idle() {

  if (! globalSimulation->isPaused()  ) {
    if (WorldSettings::isAutoScaling())
    {
      globalSimulation->resetXYMinsAndMaxes();
    }
    string forceCalculations = globalProperties->at( BillProperties::FORCE_CALCULATION_METHOD );

    calcForcesAll( globalSimulation );

    globalSimulation->update();
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
  int mainWinPosX = 100;
  int mainWinPosY = 50;
  int mainWinHeight = 720;
  int mainWinWidth = 1280;

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
