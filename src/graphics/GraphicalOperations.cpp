//
// Created by bfrasure on 11/17/18.
//

#include "GraphicalOperations.h"

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

void GraphicalOperations::fullQuadrantDrawingFunction(ControlCenter controlCenter, Quadrant quadrant) {
    if (controlCenter.shouldRenderOctree()) {
        drawingFuncFixed(quadrant);
    }
    drawShapeInQuadrant(quadrant);
};


void GraphicalOperations::localDisplay() {
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    localObserver->applyView();

    glMatrixMode(GL_MODELVIEW);

    localSimulation->getQuadrant()->applyToAllChildren([this](Quadrant quadrant) {GraphicalOperations::fullQuadrantDrawingFunction(this->localControlCenter, quadrant);});

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    cout << "GraphicalOperations::localDisplay" << endl;
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
        ): localObserver(observer)
        , localSimulation(simulation)
        , localControlCenter(controlCenter)
,control_center_num(glutCreateWindow("Control Center"))
        {
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

    glutMouseFunc(InputFunctions::myMouse);
    glutKeyboardFunc(InputFunctions::myKey);

    glutIdleFunc(callback);
    glutTimerFunc(1000, myTimer, FPS);

    configureControlWindow(dimensions);


    cout << "GraphicalOperations::GraphicalOperations" << endl;
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

    cout << "GraphicalOperations::controlDisplay" << endl;
    puDisplay(control_center_num);

    glutSwapBuffers();
    glutPostRedisplay();
}

