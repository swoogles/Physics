//
// Created by bfrasure on 11/17/18.
//

#include "GraphicalOperations.h"

SimulationPtr_t GraphicalOperations::staticSimulation;
control_center GraphicalOperations::staticControlCenter;

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

    Observer * curObserver = Observer::getCurObserver();
    curObserver->getView();

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

int GraphicalOperations::mainGlut(int argcp, char **argv, void (*callback)(void), SimulationPtr_t simulation,
                                  control_center controlCenter) {
    size_t mainWinPosX = 100;
    size_t mainWinPosY = 50;
    size_t mainWinHeight = 720;
    size_t mainWinWidth = 1280;

    glutInit(&argcp, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(mainWinPosX,mainWinPosY);
    glutInitWindowSize(mainWinWidth,mainWinHeight);
    int main_window = glutCreateWindow("Center Stage");
    glutSetWindow(main_window);
    GraphicalOperations::staticSimulation = simulation;
    GraphicalOperations::staticControlCenter = controlCenter;
    glutDisplayFunc(GraphicalOperations::display);

    glutMouseFunc(myMouse);
    glutKeyboardFunc(myKey);

    glutIdleFunc(callback);
    glutTimerFunc(1000, myTimer, FPS);

    configureControlWindow(
            mainWinPosX,
            mainWinPosY,
            mainWinHeight,
            mainWinWidth
    );
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
