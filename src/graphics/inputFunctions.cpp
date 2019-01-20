#include "inputFunctions.h"

using namespace std;

queue<MouseAction> InputFunctions::mouseActions;

void InputFunctions::myMouse(int button, int state, int x, int y) {
    if (state == GLUT_UP) {
        if (button == GLUT_WHEEL_UP) {
            mouseActions.push(MouseAction::SCROLL_UP);
        }
        if (button == GLUT_WHEEL_DOWN) {
            mouseActions.push(MouseAction::SCROLL_DOWN);
        }

    }
    puMouse ( button, state, x, y ) ;
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

optional<MouseAction> InputFunctions::currentMouseAction() {
    if (!InputFunctions::mouseActions.empty()) {
        auto currentAction = InputFunctions::mouseActions.front();
        InputFunctions::mouseActions.pop();
        return currentAction;
    } else {
        return {};
    }

}
