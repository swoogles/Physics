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

#include "octree.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

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

#define WW 5
#define WH 5
#define FPS 20

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


Quadrant * globalQuadrant;

void calcXYMinsAndMaxes(boost::numeric::ublas::vector<MyShape *> shapeList,
						float &minX, float &minY, float &maxX, float &maxY) {
	cout << "Ok " << endl;
	cout << "Size: " << shapeList.size() << endl;
	sgVec4 curPos;

	minX = FLT_MAX;
	minY = FLT_MAX;

	maxX = -(FLT_MAX-1);
	maxY = -(FLT_MAX-1);

	for (unsigned int i = 0; i < shapeList.size(); i++) {
		shapeList(i)->getPos(curPos);

		if (curPos[0] < minX)
			minX = curPos[0];
		if (curPos[0] > maxX)
			maxX = curPos[0];
		if (curPos[1] < minY)
			minY = curPos[1];
		if (curPos[1] > maxY)
			maxY = curPos[1];

	}
}

// You want to avoid passing argument to this method, because it would slow down every single
// call.
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
	for (unsigned int i = 0; i < MyShape::shapes.size(); i++) {
		MyShape::shapes(i)->draw();
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

	Box::init();

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
	  largeGridAlternating();
  }
  if ( simulation == '1' ) {
	  bodyFormation( 650 );
  }
  if ( simulation == '2' ) {
	  disruption();
  }
  if ( simulation == '3' ) {
	  simpleCollision();
  }
  if ( simulation == '4' ) {
	  billiards(4);
  }
  if ( simulation == '5' ) {
	  billiards2(10);
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
	  bodyFormationGeneric( 650, target, groupMomentum );
    cout << "xpos: " << target[0] << endl;
    cout << "NumShapes: " << MyShape::shapes.size() << endl;

    target[0]=-target[0];
    groupMomentum[0]=0;
    groupMomentum[1]=-2800;
    groupMomentum[2]=0;

    cout << "xpos: " << target[0] << endl;
	  bodyFormationGeneric( 650, target, groupMomentum );
    cout << "NumShapes: " << MyShape::shapes.size() << endl;
  }
  if ( simulation == '7' ) {
    globalQuadrant = octreeDemonstration(10);
  }

  if ( simulation == '8' ) {
    int numShapes = MyShape::shapes.size();
    MyShape::shapes.resize(numShapes + 1);
    MyShape::shapes(numShapes) = new Box();
    
  }
  
	//billiards3(7);

	char saveFileName[150] = "/media/Media\ Hog/ProjectOutput/TheReturn/";
	strcat(saveFileName, "output.dat");

	//cout << "fileName: " << saveFileName << endl;
	//saveShapes(saveFileName);


	float minX, minY, maxX, maxY;
	calcXYMinsAndMaxes(MyShape::shapes, minX, minY, maxX, maxY);

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

}

void idle() {
	sgVec4 curPos;

	if (! WorldSettings::isPaused() ) {
    numStep++;

		calcForcesAll(WorldSettings::getDT());
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

    MyShape * largest[10];
    for (unsigned int j = 10 -1 ; j > 0 ; j-- ) {
      largest[j] = NULL;
    }

    if ( MyShape::shapes.size() > 0 )
    {
      for (unsigned int i = 0; i < MyShape::shapes.size(); i++)
      {
        if ( numStep == 1 ) {
          totalMass += MyShape::shapes(i)->getMass();
        }

        //MyShape::shapes(i)->adjustMomentum(gravity);
        MyShape::shapes(i)->update(WorldSettings::getDT());
        MyShape::shapes(i)->getPos(curPos);

        if (WorldSettings::isAutoScaling())
          WorldSettings::updateXYMinsAndMaxes(curPos);

        //Largest Objects
        if ( numStep % 50 == 0 ) {
          cout << endl << endl;
          for (unsigned int j = 10 -1 ; j > 0 ; j-- ) {
            if ( largest[j] != NULL ) {
              // if ( largest(j) != NULL ) {

              if ( MyShape::shapes(i)->getMass() > largest[j]->getMass() ) {
                if ( j < 10 -1 ) {
                  largest[j+1] = largest[j];
                }
                largest[j] = MyShape::shapes(i);
              }
            }
              else {
                largest[j+1] = largest[j];
                largest[j] = MyShape::shapes(i);
              }
            }

          }

        }
        sgVec3 color;
        for (unsigned int j = 10 -1 ; j > 0 ; j-- ) {
          if ( largest[j] != NULL ) {
            largest[j]->getColor( color );
            cout << "largest[" << j << "]: " << largest[j]->getMass() 
              << "( "  <<largest[j]->getMass() / totalMass * 100 << "% of the total mass)" 
              << " color:[" << color[0] << "\t" << color[1] << "\t"<< color[2] << endl;
          }
        }
        if ( numStep % 50 == 0 ) {
          cout << endl << endl;
        }

        calcCollisionsAll();
      }
    }

    int curObserver = Observer::getCurObserver();

    Observer::observers(curObserver)->update(WorldSettings::getDT());

    /*
       float minX, minY, maxX, maxY;
       calcXYMinsAndMaxes(MyShape::shapes, minX, minY, maxX, maxY);
       */

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

/*
  int sub_menu = glutCreateMenu(size_menu);
  glutAddMenuEntry("Option 2", 2);
  glutAddMenuEntry("Move Shapes up", 3);
  glutCreateMenu(top_menu);
  glutAddMenuEntry("Quit", 1);
  glutAddSubMenu("Further Options", sub_menu);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  */


  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(controlWinPosX,controlWinPosY);
  glutInitWindowSize(controlWinWidth,controlWinHeight);
  control_center_num = glutCreateWindow("Control Center");

  glutDisplayFunc(controlDisplay);
  glutMouseFunc(myMouse);
  glutKeyboardFunc(myKey);
  //glutIdleFunc(controlDisplay);
  //glutIdleFunc(display);
  //glutTimerFunc(100, myTimer, FPS);
  //glutReshapeFunc(box, y);

  //Get the buttons laid out and what-not
  control_center::init();

  glutSetWindow(main_window);

	//int seconds = 1000000;
	//usleep(10*seconds);

  glutMainLoop();

  return 0;
}

void myTimer(int v) {
	glutPostRedisplay();
	glutTimerFunc(1000/FPS, myTimer, v);
}
