/*
 * main.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#define PU_USE_GLUT 1

#include <GL/glut.h>
#include <GL/glu.h>
#include <cmath>

#include <boost/foreach.hpp>
#include <boost/ref.hpp>

#include <float.h>
#include <unistd.h>
#include <fstream>
#include <map>
#include <string>

#include "inputFunctions.h"
#include "CameraFunctions.h"

//GUI stuff
#include "Windows/control_center.h"
#include "Windows/main_display.h"

//All PLIB includes (What a great library)
#include <plib/sg.h>
#include <plib/pu.h>
#include <plib/puAux.h>

//Observers
#include "Observation/Observer.h"

//Physics
#include "Physics/Interactions.h"
#include "Physics/Simulations.h"
#include "Physics/WorldSettings.h"

//Output
//#include "ImageOutput.h"
#include <jpeglib.h>
#include "Observation/Recording.h"

//File interaction
#include "fileInteraction.h"

#define WW 5
#define WH 5
#define FPS 1

#define foreach_  BOOST_FOREACH                                                                                              

//#define DT .04
//#define DT 4000  //Formation DT
//#define DT 1.81e5  //Formation DT
//#define DT 4e10 //Simple orbit DT
//#define DT 4

using namespace std;
using namespace boost::numeric::ublas;
using boost::numeric::ublas::compressed_vector;
//using namespace mathglpp;

float globalPullback;

void myTimer(int v);

static int main_window;

static int control_center_num;

bool writtenImage = false;

int numStep = 0;
int numFrame = 0;
float totalMass = 0;


boost::shared_ptr<Quadrant> globalQuadrant;
std::map<std::string, std::string> globalProperties;
boost::shared_ptr<Simulation> globalSimulation;
boost::shared_ptr<Recorder> globalRecorder;

control_center globalControlCenter;
main_window_UI globalMainDisplay;

class BillProperties
{
  public:
    // Using char * instead of string gets rid of heap allocation and dynamic initialization
    static const char FORCE_CALCULATION_METHOD[];
    static const char  SIMULATION_DT[];

    bool static isValidProperty( string line );
};
const char BillProperties::FORCE_CALCULATION_METHOD[] = "forceCalculationMethod";
const char BillProperties::SIMULATION_DT[] = "dt";

bool BillProperties::isValidProperty( string line )
{
  bool valid = true;
  if ( line[0] == '#' )
  {
    valid = false;
  }

  if ( line.find('=') == -1 )
  {
    valid = false;
  }

  return valid;
}

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
  typedef boost::shared_ptr<MyShape> shape_pointer;
  if ( MyShape::shapes.size() > 0 )
  {
    foreach_ ( shape_pointer curShape, MyShape::shapes )
    {
      curShape->draw();
    }
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


void init(char simulation) {
  // Simulations::setCurStep(0);
  // globalSimulation->setCurStep(0);

  cout.flush();
  glViewport(-WW,WW,-WH,WH);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  string line;
  string propName, propValue;
  ifstream propertiesFile;
  propertiesFile.open(".properties", ios::in);
  int equalsPosition;
  while( getline(propertiesFile, line) )
  {
    if ( BillProperties::isValidProperty( line ) )
    {
      equalsPosition = line.find('=');
      propName=line.substr(0,equalsPosition);
      propValue=line.substr(equalsPosition+1);
      globalProperties.insert( make_pair( propName, propValue ) );
    }
  }
  propertiesFile.close();

  globalRecorder = boost::make_shared<Recorder>();

  Observer::init();

  Observer::observers.resize(Observer::observers.size()+1);
  Observer::observers(0) = new Observer;
  Observer * curObserver =  Observer::observers(0);
  Observer::setCurObserver(0);

  // Determine and create simulation
  globalSimulation = Simulations::createSimulation( simulation );
  MyShape::shapes = globalSimulation->getPhysicalObjects().getShapes() ;

  globalSimulation->setForceCalcMethodByString( globalProperties.at( BillProperties::FORCE_CALCULATION_METHOD ) );

  char saveFileName[150] = "/media/Media Hog/ProjectOutput/TheReturn/";
  strcat(saveFileName, "output.dat");

  float minX, minY, maxX, maxY;
  sgVec4 curPos;
  foreach_ ( shape_pointer curShape, globalSimulation->getPhysicalObjects().getShapes() )
  {
    if (WorldSettings::isAutoScaling())
    {
      curShape->getPos(curPos);
      globalSimulation->updateXYMinsAndMaxes(curPos);
    }
  }
  minX = globalSimulation->getMinX();
  maxX = globalSimulation->getMaxX();
  minY = globalSimulation->getMinY();
  maxY = globalSimulation->getMaxY();
  curObserver->calcMinPullback( 45.0, minX, minY, maxX, maxY);

  char pathName[]="/media/bfrasure/Media Hog/VideoOutput/outFrame";
  globalRecorder->setPath(pathName);
  globalRecorder->setSkipFrames(1);

  //openShapes(saveFileName);

  globalSimulation->refreshQuadrant();
}

void idle() {
  typedef boost::shared_ptr<MyShape> shape_pointer;
  sgVec4 curPos;

  if (! globalSimulation->isPaused()  ) {
    if (WorldSettings::isAutoScaling())
    {
      globalSimulation->resetXYMinsAndMaxes();
    }
    // cout << "Function:" << BOOST_CURRENT_FUNCTION << endl;
    string forceCalculations = globalProperties.at( BillProperties::FORCE_CALCULATION_METHOD );

    calcCollisionsAll( globalSimulation );
    calcForcesAll( globalSimulation, globalQuadrant );

    globalSimulation->updateTimeElapsed();
    globalMainDisplay.update();

    globalSimulation->incCurStep();

    foreach_ ( shape_pointer curShape, globalSimulation->getPhysicalObjects().getShapes() )
    {
      if (WorldSettings::isAutoScaling())
      {
        curShape->getPos(curPos);
        globalSimulation->updateXYMinsAndMaxes(curPos);
      }
      globalQuadrant->insertShape( curShape );
    }
  }

  int curObserverIdx = Observer::getCurObserver();
  Observer * curObserver =  Observer::observers(curObserverIdx);
  curObserver->update( globalSimulation->getDT() );

  // Not sure if I can use Observer the way that I want to here, due to the constaints of the input methods
  if (WorldSettings::isAutoScaling()) {

    float minX = globalSimulation->getMinX();
    float minY = globalSimulation->getMinY();
    float maxX = globalSimulation->getMaxX();
    float maxY = globalSimulation->getMaxY();

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
  cout << "argv[1]: " << simulation << endl;

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

void myTimer(int v) {
  glutPostRedisplay();
  glutTimerFunc(1000/FPS, myTimer, v);
}

void calcLargest()
{

  //Largest Objects
  // if ( numStep % 50 == 0 ) {
  //   cout << endl << endl;
  //   for (unsigned int j = 10 -1 ; j > 0 ; j-- ) {
  //     cout << "Shape.2" << endl;
  //     if ( largest[j] != NULL ) {
  //       cout << "Shape.3" << endl;

  //       if ( curShape->getMass() > largest[j]->getMass() ) {
  //         cout << "Shape.4" << endl;
  //         if ( j < 10 -1 ) {
  //           cout << "Shape.5" << endl;
  //           largest[j+1] = largest[j];
  //         }
  //           cout << "Shape.6" << endl;
  //         largest[j] = curShape;
  //           cout << "Shape.7" << endl;
  //       }
  //     }
  //       else {

  //           cout << "index: " << j << endl;

  //           shape_pointer x = largest[j];

  //           cout << "index: " << j+1 << endl;
  //           x = largest[j+1];


  //         largest[j+1] = largest[j];
  //           cout << "Shape.9" << endl;
  //         largest[j] = curShape;
  //           cout << "Shape.10" << endl;
  //       }
  //       cout << "Um..." << endl;
  //     }

  //   }

  // }
  // cout << "Going to output largest" << endl;
  // sgVec3 color;
  // for (unsigned int j = 10 -1 ; j > 0 ; j-- ) {
  //   if ( largest[j] != NULL ) {
  //     largest[j]->getColor( color );
  //     cout << "largest[" << j << "]: " << largest[j]->getMass() 
  //       << "( "  <<largest[j]->getMass() / totalMass * 100 << "% of the total mass)" 
  //       << " color:[" << color[0] << "\t" << color[1] << "\t"<< color[2] << endl;
  //   }
  // }
  // if ( numStep % 50 == 0 ) {
  //   cout << endl << endl;
}
