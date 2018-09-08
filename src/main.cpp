/*
 * main.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#define PU_USE_GLUT 1

#include <GL/glut.h>
#include <GL/glu.h>

#include <boost/foreach.hpp>
#include <boost/ref.hpp>
#include <boost/thread/thread.hpp>
// #include <boost/thread/scoped_thread.hpp>

#include <float.h>
#include <string>
#include <sstream>

// #include <omp.h>

#include "BillProperties.h"
#include "inputFunctions.h"

//GUI stuff
#include "Windows/control_center.h"
#include "Windows/main_display.h"

//All PLIB includes (What a great library)
#include <plib/sg.h>
#include <plib/pu.h>

//Observers
#include "Observation/Observer.h"

//Physics
#include "Physics/Interactions.h"
#include "Physics/Simulations.h"
#include "Physics/WorldSettings.h"

//Output
#include "Observation/Timer.h"

//File interaction
#include "fileInteraction.h"

#define WW 5
#define WH 5
#define FPS 1

#define foreach_  BOOST_FOREACH                                                                                              

using namespace std;
using namespace boost::numeric::ublas;
using boost::numeric::ublas::compressed_vector;
using boost::shared_ptr;
using boost::make_shared;

typedef boost::shared_ptr<MyShape> shape_pointer;

int curStep = 0;
double totalTime = 0;

void sneeze()
{
  cout << "Achoo!" << endl;
}

void myTimer(int v) {
  glutPostRedisplay();
  glutTimerFunc(1000/FPS, myTimer, v);
}

static int main_window;

static int control_center_num;

// GLOBALS
boost::shared_ptr<Simulation> globalSimulation;
boost::shared_ptr<BillProperties> globalProperties;

control_center globalControlCenter;
main_window_UI globalMainDisplay;

// You want to avoid passing argument to this method, because it would slow down every single
// call.
// TODO Allow an arbitrary list of objects to be displayed
void display(void)
{
  glutSetWindow(main_window);

  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT);

  sgVec4 curPos;

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  Observer * curObserver = Observer::getCurObserver();
  curObserver->getView();
  curObserver->getPos(curPos);

  sgVec3 curColor = { 1.0, 0.0, 0.0 };
  glColor3fv(curColor);

  glMatrixMode(GL_MODELVIEW);
  MyShape::drawAllShapes();

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

double step;

void init(char simulation) {
   cout << "lookin for simulation: "  << simulation;
  glViewport(-WW,WW,-WH,WH);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  globalProperties = boost::make_shared<BillProperties>();
  globalProperties->readProperties();

  Observer::init();
  Observer * curObserver = Observer::getCurObserver();

  int numShapes = atoi(globalProperties->at( BillProperties::NUM_SHAPES ).c_str()); //value = 45

  // Determine and create simulation
  globalSimulation = Simulations::createSimulation( simulation, numShapes );
  MyShape::shapes = globalSimulation->getPhysicalObjects().getShapes() ;

  globalSimulation->setForceCalcMethodByString( globalProperties->at( BillProperties::FORCE_CALCULATION_METHOD ) );

  char saveFileName[150] = "/media/Media Hog/ProjectOutput/TheReturn/";
  strcat(saveFileName, "output.dat");

  float minX, minY, maxX, maxY;
  sgVec4 curPos;
  foreach_ ( shape_pointer curShape, globalSimulation->getPhysicalObjects().getShapes() )
  {
    curShape->getPos(curPos);
    globalSimulation->updateXYMinsAndMaxes(curPos);
  }
  globalSimulation->getXYMinsAndMaxes( minX, maxX, minY, maxY );
  curObserver->calcMinPullback( 45.0, minX, minY, maxX, maxY);

  string outFileName = "outFrame";
  string extension = "jpg";

  // Not using this for now because of all the corrupted list problems I was getting
  bool parallelize;
  if ( globalProperties->at( BillProperties::PARALLEL ).compare( "true" ) )
  {
    parallelize = true;
  }
  else
  {
    parallelize = false;
  }
  //openShapes(saveFileName);

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

int main(int argcp, char **argv) {
  int mainWinPosX = 100;
  int mainWinPosY = 50;
  int mainWinHeight = 720;
  int mainWinWidth = 1280;

  int controlWinPosX = mainWinPosX;
  int controlWinPosY = mainWinPosY + mainWinHeight + 30;
  int controlWinWidth = mainWinWidth;
  int controlWinHeight = 200;


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

  puInit();
  char simulation = argv[2][0];

  init( simulation );

  //Creates main menu bar
  globalMainDisplay.init( globalSimulation );

  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(controlWinPosX,controlWinPosY);
  glutInitWindowSize(controlWinWidth,controlWinHeight);
  control_center_num = glutCreateWindow("Control Center");

  glutDisplayFunc(controlDisplay);
  glutMouseFunc(myMouse);
  glutKeyboardFunc(myKey);

  globalControlCenter.init( globalSimulation );

  glutSetWindow(main_window);

  glutMainLoop();

  return 0;
}
