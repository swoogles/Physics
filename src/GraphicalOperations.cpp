//
// Created by bfrasure on 11/17/18.
//

#include "GraphicalOperations.h"

SimulationPtr_t GraphicalOperations::staticSimulation;
control_center GraphicalOperations::staticControlCenter;
shared_ptr<Observer> GraphicalOperations::observer;

#define FPS 1

using std::size_t;

void myTimer(int v) {
    glutPostRedisplay();
    glutTimerFunc(1000/FPS, myTimer, v);
}


void drawShapeInQuadrant(Quadrant quadrant) {
    auto shape = quadrant.getShapeInQuadrant();
    if (shape != nullptr) Drawing::draw(shape);
};

void drawingFuncFixed(Quadrant quadrant) {
    Drawing::draw(quadrant.getBorders());
}

void GraphicalOperations::fullQuadrantDrawingFunction(Quadrant quadrant) {
    if (staticControlCenter.shouldRenderOctree()) {
        drawingFuncFixed(quadrant);
    }
    drawShapeInQuadrant(quadrant);
};


void GraphicalOperations::display() {
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    GraphicalOperations::observer->applyView();

    glMatrixMode(GL_MODELVIEW);

    staticSimulation->getQuadrant()->applyToAllChildren(fullQuadrantDrawingFunction);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    puDisplay();
    glutSwapBuffers();
    glutPostRedisplay();

}

GraphicalOperations::GraphicalOperations() {
}

int GraphicalOperations::mainGlut(
        void (*callback)(void),
        SimulationPtr_t simulation,
        control_center controlCenter,
        shared_ptr<Observer> observer,
        WindowDimensions dimensions
) {
    this->openGlInit();
    size_t mainWinPosX = 100;
    size_t mainWinPosY = 50;
    size_t mainWinHeight = 720;
    size_t mainWinWidth = 1280;

    char fakeParam[] = "fake";
    char *fakeargv[] = { fakeParam, NULL };
    int fakeargc = 1;
    glutInit( &fakeargc, fakeargv );

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(dimensions.xPos,dimensions.yPos);
    glutInitWindowSize(dimensions.width,dimensions.height);
    int main_window = glutCreateWindow("Center Stage");
    glutSetWindow(main_window);
    GraphicalOperations::staticSimulation = simulation;
    GraphicalOperations::staticControlCenter = controlCenter;
    GraphicalOperations::observer = observer;
    glutDisplayFunc(GraphicalOperations::display);

    glutMouseFunc(InputFunctions::myMouse);
    glutKeyboardFunc(InputFunctions::myKey);

    glutIdleFunc(callback);
    glutTimerFunc(1000, myTimer, FPS);

    configureControlWindow(
            mainWinPosX,
            mainWinPosY,
            mainWinHeight,
            mainWinWidth
    );

    puInit();

    return main_window;
}

void GraphicalOperations::configureControlWindow(
        int mainWinPosX,
        int mainWinPosY,
        int mainWinHeight,
        int mainWinWidth

) {
    int controlWinPosX = mainWinPosX;
    int controlWinPosY = mainWinPosY + mainWinHeight + 30;
    int controlWinWidth = mainWinWidth;
    int controlWinHeight = 200;


    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(controlWinPosX,controlWinPosY);
    glutInitWindowSize(controlWinWidth,controlWinHeight);
}

void GraphicalOperations::openGlInit() {
    glViewport(-WW,WW,-WH,WH);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
