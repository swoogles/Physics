/*
 * inputFunctions.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#include "inputFunctions.h"

using namespace std;

void myMouse(int button, int state, int x, int y) {
	if (state == GLUT_UP) {
    int curObserverIdx = Observer::getCurObserver();
    Observer * curObserver =  Observer::observers(curObserverIdx);
		if (button == GLUT_WHEEL_UP) {
      curObserver->zoomIn();
			control_center::uncheckAutoScaling();
			WorldSettings::setAutoScaling(false);
		}
		if (button == GLUT_WHEEL_DOWN) {
			curObserver->zoomOut();
			control_center::uncheckAutoScaling();
			WorldSettings::setAutoScaling(false);
		}

	}
	puMouse ( button, state, x, y ) ;
	glutPostRedisplay ();
}

void myKey(unsigned char key, int x, int y) {
	if (key == 'h') {
		cout << "Hi!" << endl;
	}
	if (key == 'w') {
		//MoveShapes(0,.5,0);
  }
	if (key == 'a') {}
		//moveShapes(-.5,0,0);
	if (key == 's') {
		//moveShapes(0,-.5,0);
	}
	if (key == 'd') {}
		//moveShapes(.5,0,0);

	puKeyboard(key, PU_DOWN);
}
