/*
 * inputFunctions.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#include "inputFunctions.h"

using namespace std;

shared_ptr<Observer> InputFunctions::observer;
queue<MouseAction> InputFunctions::mouseActions;

void InputFunctions::stopAutoScaling() {
	InputFunctions::observer->setAutoScaling(false);
}

void InputFunctions::myMouse(int button, int state, int x, int y) {
    if (state == GLUT_UP) {
        if (button == GLUT_WHEEL_UP) {
            mouseActions.push(MouseAction::SCROLL_UP);
            InputFunctions::observer->zoomIn();
            stopAutoScaling();
        }
        if (button == GLUT_WHEEL_DOWN) {
            mouseActions.push(MouseAction::SCROLL_DOWN);
            InputFunctions::observer->zoomOut();
            stopAutoScaling();
        }

    }
    puMouse ( button, state, x, y ) ;
    cout << "Number of Mouse Actions:" <<  (InputFunctions::mouseActions.size()) << endl;
}

void InputFunctions::myKey(unsigned char key, int x, int y) {
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

void InputFunctions::init(shared_ptr<Observer> observer) {
    InputFunctions::observer = observer;
}
