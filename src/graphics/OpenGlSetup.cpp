//
// Created by bfrasure on 1/12/19.
//

#include "OpenGlSetup.h"

void OpenGlSetup::initialize(WindowDimensions dimensions) {
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
}
