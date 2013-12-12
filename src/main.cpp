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
#include <boost/thread/scoped_thread.hpp>

#include <float.h>
#include <string>

#include <omp.h>

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
#include "Observation/Recorder.h"
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

typedef shared_ptr<MyShape> shape_pointer;

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
shared_ptr<Simulation> globalSimulation;
shared_ptr<Recorder> globalRecorder;
shared_ptr<BillProperties> globalProperties;

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

  int curObserverIdx = Observer::getCurObserver();
  Observer * curObserver =  Observer::observers(curObserverIdx);
  curObserver->getView();
  curObserver->getPos(curPos);

  sgVec3 curColor = { 1.0, 0.0, 0.0 };
  glColor3fv(curColor);

  glMatrixMode(GL_MODELVIEW);
  if ( MyShape::shapes.size() > 0 )
  {
    // #pragma omp parallel for
    // for (unsigned int i = 0; i < MyShape::shapes.size(); i++) {
    //   MyShape::shapes(i)->draw();
    // }

    Timer drawingTimer = Timer();
    if ( curStep < 100 )
    {
      drawingTimer.startTiming();
    }

    omp_set_num_threads(1);
    int numShapes = 0;
    shape_pointer curShape;
    // for (unsigned int i = 0; i < numShapes; i++) 
    // #pragma omp parallel for reduction(+:numShapes)
    for (unsigned int i = 0; i < MyShape::shapes.size(); i++) 
    {
      curShape = MyShape::shapes(i);
      curShape->draw();
      if ( curStep == 1 )
      {
        cout << "Shape(" << i << ").pos: " << curShape->getPosString() << endl;
      }
      numShapes++;
    }

    // cout << "numShapes (reduced): " << numShapes  << endl;

    // foreach_ ( shape_pointer curShape, MyShape::shapes )
    // {
    //   curShape->draw();
    // }

    if ( curStep < 100 )
    {
      // cout << "Drawing ";
      // drawingTimer.stopTiming();
      // totalTime +=drawingTimer.getDuration().count();
    }
    else if ( curStep == 100 )
    {
      cout << "Avg time: " << totalTime/100  << endl;
    }

    curStep++;

    // #pragma omp parallel for reduction(+:threadSum)
    // for (localI = 0; localI < num_steps; localI+=1)
    // {
    //   double localX = ( (localI)+0.5)*step;
    //   threadSum += 4.0/(1.0+localX*localX);
    // }
  }

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  //Recording section
  // TODO A different version of this function should be called if I want to record, rather 
  // than a branch here.
  if ( globalRecorder->getRecording() && globalRecorder->shouldCaptureThisFrame() && ! globalSimulation->isPaused() ) {

    globalRecorder->captureThisFrame(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
  }
  globalRecorder->incCurFrame();

  puDisplay();
  glutSwapBuffers();
  glutPostRedisplay();
}

void controlDisplay(void) {
  glutSetWindow(control_center_num);
  glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  puDisplay(control_center_num);

  //puDisplay();
  glutSwapBuffers();
  glutPostRedisplay();

  //glutSetWindow(main_window);
}

static long num_steps = 200000000;
double step;

void init(char simulation) {
  cout.flush();
  glViewport(-WW,WW,-WH,WH);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  globalProperties = make_shared<BillProperties>();
  globalProperties->readProperties();

  globalRecorder = make_shared<Recorder>();
  globalRecorder->imageMagickMucking();

  boost::strict_scoped_thread<> t((boost::thread( sneeze )));

  Observer::init();

  Observer::observers.resize(Observer::observers.size()+1);
  Observer::observers(0) = new Observer;
  Observer * curObserver =  Observer::observers(0);
  Observer::setCurObserver(0);

  // Determine and create simulation
  globalSimulation = Simulations::createSimulation( simulation );
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

  char pathName[]="/media/bfrasure/Media Hog/VideoOutput/outFrame";
  globalRecorder->setPath(pathName);
  globalRecorder->setSkipFrames(1);

  //openShapes(saveFileName);

  globalSimulation->refreshQuadrant();
}

void idle() {
  sgVec4 curPos;

  if (! globalSimulation->isPaused()  ) {
    if (WorldSettings::isAutoScaling())
    {
      globalSimulation->resetXYMinsAndMaxes();
    }
    // cout << "Function:" << BOOST_CURRENT_FUNCTION << endl;
    string forceCalculations = globalProperties->at( BillProperties::FORCE_CALCULATION_METHOD );


    // Timer collisionsTimer = Timer();
    // collisionsTimer.startTiming();
    calcCollisionsAll( globalSimulation );
    // collisionsTimer.stopTiming();

    calcForcesAll( globalSimulation );
    globalSimulation->getPhysicalObjects().update( globalSimulation->getDT() );

    globalSimulation->updateTimeElapsed();
    globalMainDisplay.update();

    globalSimulation->incCurStep();

    foreach_ ( shape_pointer curShape, globalSimulation->getPhysicalObjects().getShapes() )
    {
      curShape->getPos(curPos);
      globalSimulation->updateXYMinsAndMaxes(curPos);
    }

    // if ( globalSimulation->getCurStep() == 0 )
    // {
      globalSimulation->refreshQuadrant();
    // }
  }

  int curObserverIdx = Observer::getCurObserver();
  Observer * curObserver =  Observer::observers(curObserverIdx);
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
  char record = argv[1][0];
  char simulation = argv[2][0];

  init( simulation );

  if ( record == 'r' ){
    globalRecorder->setRecording(true);
  }

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
