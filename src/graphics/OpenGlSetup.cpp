//
// Created by bfrasure on 1/12/19.
//

#include "OpenGlSetup.h"

#include <iostream>
using namespace std;

void myTimer(int v) {
    glutPostRedisplay();
    glutTimerFunc(FPS, myTimer, v);
}


void OpenGlSetup::initialize(WindowDimensions dimensions, void (idleFunction)(void)) {
    glViewport(-WW,WW,-WH,WH);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

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

    glutMouseFunc(InputFunctions::myMouse);
    glutKeyboardFunc(InputFunctions::myKey);

    configureControlWindow(dimensions);
    controlCenterNum = glutCreateWindow("Control Center");
    cout << "control center: " << controlCenterNum << endl;

    glutMouseFunc(InputFunctions::myMouse);
    glutKeyboardFunc(InputFunctions::myKey);

    glutIdleFunc(idleFunction);
    glutTimerFunc(1000, myTimer, FPS);

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

