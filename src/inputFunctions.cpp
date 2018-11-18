/*
 * inputFunctions.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#include "inputFunctions.h"

using namespace std;

void stopAutoScaling() {
	WorldSettings::setAutoScaling(false);
}

void myMouse(int button, int state, int x, int y) {
	if (state == GLUT_UP) {
		Observer * curObserver = Observer::getCurObserver();
		if (button == GLUT_WHEEL_UP) {
			curObserver->zoomIn();
			stopAutoScaling();
		}
		if (button == GLUT_WHEEL_DOWN) {
			curObserver->zoomOut();
			stopAutoScaling();
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
