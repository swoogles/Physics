//
// Created by bfrasure on 1/12/19.
//

#include "OpenGlSetup.h"

#include <iostream>
using namespace std;

void myTimer(int v) {
    glutPostRedisplay();
    glutTimerFunc(FRAME_DELAY_MS, myTimer, v);
}


void OpenGlSetup::initialize(WindowDimensions dimensions, void (idleFunction)(void)) {
    // glutInit must be called before any OpenGL calls
    char fakeParam[] = "fake";
    char *fakeargv[] = { fakeParam, NULL };
    int fakeargc = 1;
    glutInit( &fakeargc, fakeargv );

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(dimensions.xPos,dimensions.yPos);
    glutInitWindowSize(dimensions.width,dimensions.height);

    int main_window = glutCreateWindow("Center Stage");
    cout << "main window: " << main_window << endl;
    glutSetWindow(main_window);
    mainDisplayNum = main_window;

    // OpenGL initialization (must happen after window creation)
    glViewport(-WW,WW,-WH,WH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutMouseFunc(InputFunctions::myMouse);
    glutKeyboardFunc(InputFunctions::myKey);
    glutMotionFunc(InputFunctions::myMotion);
    glutPassiveMotionFunc(InputFunctions::myMotion);

    configureControlWindow(dimensions);
    controlCenterNum = glutCreateWindow("Control Center");
    cout << "control center: " << controlCenterNum << endl;

    glutMouseFunc(InputFunctions::myMouse);
    glutKeyboardFunc(InputFunctions::myKey);
    glutMotionFunc(InputFunctions::myMotion);
    glutPassiveMotionFunc(InputFunctions::myMotion);

    glutIdleFunc(idleFunction);
    glutTimerFunc(FRAME_DELAY_MS, myTimer, 0);

    puInit();
}
void OpenGlSetup::configureControlWindow(WindowDimensions mainWindowDimensions) {
    int controlWinPosX = mainWindowDimensions.xPos;
    int controlWinPosY = mainWindowDimensions.yPos + mainWindowDimensions.height + 30;
    int controlWinWidth = mainWindowDimensions.width;
    int controlWinHeight = 200;

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(controlWinPosX,controlWinPosY);
    glutInitWindowSize(controlWinWidth,controlWinHeight);
}
