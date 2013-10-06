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

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/foreach.hpp>
#include <boost/ref.hpp>

#include "Parallelization/Quadrant.h"

#include "ShapeFiles/Box.h"
#include "ShapeFiles/Circle.h"
#include "ShapeFiles/MyEllipse.h"

#include "inputFunctions.h"
#include "menus.h"
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

#include <float.h>

#include <unistd.h>

#define WW 5
#define WH 5
#define FPS 20

#define foreach_  BOOST_FOREACH                                                                                              

//#define DT .04
//#define DT 4000  //Formation DT
//#define DT 1.81e5  //Formation DT
//#define DT 4e10 //Simple orbit DT
//#define DT 4

using namespace std;
using namespace boost::numeric::ublas;
using boost::numeric::ublas::vector;
//using namespace mathglpp;

float globalPullback;

void myTimer(int v);

static int main_window;
static puMenuBar *main_menu_bar;
static int observer_one;

static int observer_two;


static int control_center_num;

bool writtenImage = false;

//void vecFileRead(ifstream & curFile, sgVec4 retVec);

int numStep = 0;
float totalMass = 0;


boost::shared_ptr<Quadrant> globalQuadrant;
boost::numeric::ublas::vector<shape_pointer> physicalObjects; 

void calcXYMinsAndMaxes(boost::numeric::ublas::vector< boost::shared_ptr<MyShape> > shapeList,
						float &minX, float &minY, float &maxX, float &maxY) {
	cout << "Ok " << endl;
	cout << "Size: " << shapeList.size() << endl;
	sgVec4 curPos;

	minX = FLT_MAX;
	minY = FLT_MAX;

	maxX = -(FLT_MAX-1);
	maxY = -(FLT_MAX-1);

  typedef boost::shared_ptr<MyShape> shape_pointer;
  foreach_ ( shape_pointer curShape, shapeList )
  {
    curShape->getPos(curPos);

		if (curPos[0] < minX)
			minX = curPos[0];
		if (curPos[0] > maxX)
			maxX = curPos[0];
		if (curPos[1] < minY)
			minY = curPos[1];
		if (curPos[1] > maxY)
			maxY = curPos[1];

	}
  cout << "Calced maxes" << endl;
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
	int curObserver = Observer::getCurObserver();
	Observer::observers(curObserver)->getView();
	Observer::observers(curObserver)->getPos(curPos);
	//cout << "Observer pos: " << curPos[0] << ", " << curPos[1] << ", " << curPos[2] << endl;

	sgVec3 curColor;
	curColor[0] = 1.0;
	curColor[1] = 0.0;
	curColor[2] = 0.0;
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
  //globalQuadrant->thisShape->draw();


	glMatrixMode(GL_PROJECTION);
	glPopMatrix();


	//Recording section

  // TODO A different version of this function should be called if I want to record, rather 
  // than a branch here.
  if ( Recorder::getRecording() && Recorder::shouldCaptureThisFrame() && ! WorldSettings::isPaused() ) {
    Recorder::captureThisFrame(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
  }
  Recorder::incCurFrame();


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
	cout << "Initializing!" << endl;
  cout << "simulation: " << simulation << endl;
	cout.flush();
	glViewport(-WW,WW,-WH,WH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	WorldSettings::Pause();

	Observer::init();

	cout << "Making observer" << endl;
	Observer::observers.resize(Observer::observers.size()+1);
	Observer::observers(0) = new Observer;
	Observer::observers(0)->setPos(0,0,0);
	cout << "Position set" << endl;
	Observer::observers(0)->setAngle(0, 0, 0);
	cout << "Angle set" << endl;
	Observer::setCurObserver(0);
	//Observer::observers(0)->setAngVel(0, 0.2, 0);

	//******CURRENT SIMULATION*****
  if ( simulation == '0' ) {
  //if ( strcmp(simulation, "simulation") ) {
    Simulations::largeGridAlternating();
  }
  if ( simulation == '1' ) {
	  physicalObjects = Simulations::bodyFormation_ArbitraryList( 400 ).getShapes();
    MyShape::shapes = physicalObjects; 
    cout << "Done setting up sim." << endl;
  }
  if ( simulation == '2' ) {
	  // Simulations::disruption();
	  physicalObjects = Simulations::disruption_ArbitraryList().getShapes();
    MyShape::shapes = physicalObjects; 
  }
  if ( simulation == '3' ) {
	  Simulations::simpleCollision();
  }
  if ( simulation == '4' ) {
    physicalObjects = Simulations::billiardsReturningList(5).getShapes() ;
    MyShape::shapes = physicalObjects; 
  }
  if ( simulation == '5' ) {
	  Simulations::billiards2(10);
    // MyShape::removeShapeFromList(  MyShape::shapes(15) );
  }

  if ( simulation == '6' ) {
    sgVec4 groupMomentum;
    groupMomentum[0]=0;
    groupMomentum[1]=2800;
    groupMomentum[2]=0;
    groupMomentum[3]=1;
    sgVec4 target;
    target[0]=-2000;
    target[1]=0;
    target[2]=0;
    target[3]=1;
	  Simulations::bodyFormationGeneric( 650, target, groupMomentum );
    cout << "xpos: " << target[0] << endl;
    cout << "NumShapes: " << MyShape::shapes.size() << endl;

    target[0]=-target[0];
    groupMomentum[0]=0;
    groupMomentum[1]=-2800;
    groupMomentum[2]=0;

    cout << "xpos: " << target[0] << endl;
	  Simulations::bodyFormationGeneric( 650, target, groupMomentum );
    cout << "NumShapes: " << MyShape::shapes.size() << endl;
  }
  if ( simulation == '7' ) {
    cout << "Sim setup starting..." << endl;
    globalQuadrant = Simulations::octreeDemonstration(10);
    cout << "Sim setup complete!" << endl;
    // while ( MyShape::shapes.size() > 0 )
    // {
    //   MyShape::removeShapeFromList(MyShape::shapes(0));
    // }
    // MyShape::removeShapeFromList(MyShape::shapes(1));
    // int numShapes = MyShapes
    // for ( int i = 0; i < 
    cout << "Reset quadrant" << endl;
    // sleep(3);
    // globalQuadrant.reset();
  }

  if ( simulation == '8' ) {
    int numShapes = MyShape::shapes.size();
    MyShape::shapes.resize(numShapes + 1);
    MyShape::shapes(numShapes) = make_shared<Box>(); ;
    
  }

  if ( simulation == '9' ) {
    physicalObjects = Simulations::billiards3_ArbitraryList( 5 ).getShapes();
    MyShape::shapes = physicalObjects; 
  }
  

	char saveFileName[150] = "/media/Media\ Hog/ProjectOutput/TheReturn/";
	strcat(saveFileName, "output.dat");

	//cout << "fileName: " << saveFileName << endl;
	//saveShapes(saveFileName);


	float minX, minY, maxX, maxY;
	calcXYMinsAndMaxes(physicalObjects, minX, minY, maxX, maxY);

	cout << "minX: " << minX << endl;
	cout << "minY: " << minY << endl;
	cout << "maxX: " << maxX << endl;
	cout << "maxX: " << maxY << endl;


	float pullBack = calcMinPullback(45.0, minX, minY, maxX, maxY);

	Observer::observers(0)->setPos(0, 0, -pullBack*2);

	Recorder::init();
	Recorder::setPath("/media/bfrasure/Media\ Hog/VideoOutput/outFrame");
	Recorder::setSkipFrames(1);

	//clearShapes();
	//openShapes(saveFileName);

	//glhPerspectivef2(MyShape::perspectiveMat, 45, 1, 1, 5);

    int numShapes = MyShape::shapes.size();
    cout << "NumShapes in simulation: " << numShapes << endl;

}

void idle() {
	sgVec4 curPos;

  // cout << "Function:" << BOOST_CURRENT_FUNCTION << endl;
	if (! WorldSettings::isPaused() ) {
    numStep++;

		calcForcesAll_ArbitraryList(physicalObjects, WorldSettings::getDT());
		WorldSettings::updateTimeElapsed();
		main_window_UI::update();
		//calcDrag(WorldSettings::getDT());

		if (WorldSettings::isAutoScaling())
    {
			WorldSettings::resetXYMinsAndMaxes();
    }

    //Largest Objects
    // boost::numeric::ublas::vector<MyShape *> largest(10);
    // for (unsigned int j = largest.size() -1 ; j > 0 ; j-- ) {
      // largest(j) = NULL;
    // }

    typedef boost::shared_ptr<MyShape> shape_pointer;
    shape_pointer largest[10];
    // for (unsigned int j = 10 -1 ; j > 0 ; j-- ) {
    //   largest[j] = NULL;
    // }

    if ( physicalObjects.size() > 0 )
    {
      foreach_ ( shape_pointer curShape, physicalObjects )
      // for (unsigned int i = 0; i < MyShape::shapes.size(); i++)
      {
        if ( numStep == 1 ) {
          totalMass += curShape->getMass();
        }

        //MyShape::shapes(i)->adjustMomentum(gravity);
        curShape->update(WorldSettings::getDT());
        curShape->getPos(curPos);

        if (WorldSettings::isAutoScaling())
          WorldSettings::updateXYMinsAndMaxes(curPos);

        }

        calcCollisionsAll_ArbitraryList(physicalObjects);
      }
    }

    int curObserver = Observer::getCurObserver();

    Observer::observers(curObserver)->update(WorldSettings::getDT());

    sgVec4 pos;
    pos[0] = 0;
    pos[1] = 0;
    pos[2] = 0;
    pos[3] = 1;

    float side = 1e4;
    float width = side;
    float height = side;
    float depth = side;
    sgVec3 dimensions;
    dimensions[0] = width;
    dimensions[1] = height;
    dimensions[2] = depth;

    // globalQuadrant.reset();

    globalQuadrant= boost::make_shared<Quadrant>( 4, 1, boost::ref(pos), boost::ref(dimensions) ) ;
    // sleep(2);
    // cout << "Idling!" << endl;
    // cout << "Shapes size: " << MyShape::shapes.size() << endl;
    typedef boost::shared_ptr<MyShape> shape_pointer;
    boost::numeric::ublas::vector<shape_pointer> localShapeList = physicalObjects;
    foreach_ ( shape_pointer curShape, localShapeList )
    {
      // cout << "processing shape" << endl;
      globalQuadrant->insertShape( curShape );
    }

    if (WorldSettings::isAutoScaling()) {

      float minX = WorldSettings::getMinX();
      float minY = WorldSettings::getMinY();
      float maxX = WorldSettings::getMaxX();
      float maxY = WorldSettings::getMaxY();

      float pullBack = calcMinPullback(45.0, minX, minY, maxX, maxY);

      Observer::observers(curObserver)->setPos(0, 0, -pullBack * 2);
    }

  }

void dummyCallback(puObject * thisGuy) {
	cout << "I do something! Really!";
}

int main(int argcp, char **argv) {

  int mainWinPosX = 100;
  int mainWinPosY = 50;
  int mainWinHeight = 1100;
  int mainWinWidth = mainWinHeight * 1.3;

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
  glutTimerFunc(100, myTimer, FPS);

  puInit();
  //std::string record = argv[1][0];
  char record = argv[1][0];
  char simulation = argv[2][0];

  init( simulation );
  cout << "argv[1]: " << simulation << endl;

  if ( record == 'r' ){
    Recorder::setRecording(true);
  }


  //Creates main menu bar
  main_window_UI::init();

  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(controlWinPosX,controlWinPosY);
  glutInitWindowSize(controlWinWidth,controlWinHeight);
  control_center_num = glutCreateWindow("Control Center");

  glutDisplayFunc(controlDisplay);
  glutMouseFunc(myMouse);
  glutKeyboardFunc(myKey);

  //Get the buttons laid out and what-not
  control_center::init();

  glutSetWindow(main_window);

	//int seconds = 1000000;

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
