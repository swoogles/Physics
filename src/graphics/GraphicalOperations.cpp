//
// Created by bfrasure on 11/17/18.
//

#include "GraphicalOperations.h"

SimulationPtr_t GraphicalOperations::staticSimulation;
ControlCenter GraphicalOperations::staticControlCenter;
shared_ptr<Observer> GraphicalOperations::observer;

int GraphicalOperations::control_center_num;

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
    Drawing::draw(quadrant);
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

GraphicalOperations::GraphicalOperations(
        void (*callback)(void),
        SimulationPtr_t simulation,
        ControlCenter controlCenter,
        shared_ptr<Observer> observer,
        WindowDimensions dimensions
        ) {
    this->openGlInit();

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

    configureControlWindow(dimensions);

    control_center_num = glutCreateWindow("Control Center");

    puInit();
}

void GraphicalOperations::configureControlWindow(WindowDimensions mainWindowDimensions) {
    int controlWinPosX = mainWindowDimensions.xPos;
    int controlWinPosY = mainWindowDimensions.yPos + mainWindowDimensions.height + 30;
    int controlWinWidth = mainWindowDimensions.width;
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

void GraphicalOperations::controlDisplay() {
    glutSetWindow(control_center_num);
    glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    puDisplay(control_center_num);

    glutSwapBuffers();
    glutPostRedisplay();
}

void GraphicalOperations::postSimulationGlInit() {
    glutDisplayFunc(GraphicalOperations::controlDisplay);
    glutMouseFunc(InputFunctions::myMouse);
    glutKeyboardFunc(InputFunctions::myKey);

    glutMainLoop();
}
