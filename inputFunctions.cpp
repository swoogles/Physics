/*
 * inputFunctions.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#include "inputFunctions.h"

using namespace std;

void myMouse(int button, int state, int x, int y) {
	int z;
	if (state == GLUT_UP) {
		if (button == GLUT_WHEEL_UP) {
			int curObserver = Observer::getCurObserver();
			Observer::observers(curObserver)->zoomIn();
			control_center::uncheckAutoScaling();
			WorldSettings::setAutoScaling(false);
			//z = x;
		}
		if (button == GLUT_WHEEL_DOWN) {
			int curObserver = Observer::getCurObserver();
			Observer::observers(curObserver)->zoomOut();
			control_center::uncheckAutoScaling();
			WorldSettings::setAutoScaling(false);
			// V decrease
			//glTranslatef(0,0,-1);
		}

	}
	/*
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		cout << "rotating!" << endl;
		rotateShapes(PI/8);
	}
	*/
		if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		cout << "rotating!" << endl;
		//rotateShapes(PI/8);
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
		//cout << shapes(1)->getPts() << endl;
	}
	if (key == 'a') {}
		//moveShapes(-.5,0,0);
	if (key == 's') {
		//moveShapes(0,-.5,0);
		//cout << shapes(1)->getPts() << endl;
	}
	if (key == 'd') {}
		//moveShapes(.5,0,0);

	puKeyboard(key, PU_DOWN);
}
