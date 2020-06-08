#ifndef INPUTFUNCTIONS_H_
#define INPUTFUNCTIONS_H_

#include "MouseAction.h"

#include <queue>
#include <optional>

#ifndef GLUT_WHEEL_UP
# define GLUT_WHEEL_UP 3
# define GLUT_WHEEL_DOWN 4
#endif

using std::queue;
using std::optional;

class InputFunctions {
public:
    static void myMouse(int button, int state, int x, int y);
    static void myKey(unsigned char, int x, int y);
    //  TODO create user input fields that can be queried by FullApplication
    static optional<MouseAction> currentMouseAction();
private:
    static queue<MouseAction> mouseActions;
};

#endif /* INPUTFUNCTIONS_H_ */
