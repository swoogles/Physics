#include <utility>

//
// Created by bfrasure on 11/17/18.
//

#include "GraphicalOperations.h"

using std::size_t;

void drawShapeInQuadrant(Quadrant quadrant) {
    auto shape = quadrant.getShapeInQuadrant();
    if (shape != nullptr) Drawing::draw(shape);
};

void GraphicalOperations::fullQuadrantDrawingFunction(ControlCenter controlCenter, Quadrant quadrant) {
    if (controlCenter.shouldRenderOctree()) {
        Drawing::draw(quadrant);
    }
    drawShapeInQuadrant(quadrant);
};


void GraphicalOperations::localDisplay() {
    glutSetWindow(mainDisplayNum);
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    localObserver->applyView();

    glMatrixMode(GL_MODELVIEW);

    localSimulation->getQuadrant()->applyToAllChildren([this](Quadrant quadrant) {GraphicalOperations::fullQuadrantDrawingFunction(this->localControlCenter, quadrant);});

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    puDisplay();
    glutSwapBuffers();
    glutPostRedisplay();

}

GraphicalOperations::GraphicalOperations(SimulationPtr_t simulation, ControlCenter controlCenter,
                                         shared_ptr<Observer> observer, int CenterStageWindow,
                                         int controlCenterWindow) : localObserver(std::move(observer))
        , localSimulation(simulation)
        , localControlCenter(controlCenter)
        , mainDisplayNum(CenterStageWindow)
        , control_center_num(controlCenterWindow)
{ }

void GraphicalOperations::controlDisplay() {
    glutSetWindow(control_center_num);
    glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    puDisplay(control_center_num);

    glutSwapBuffers();
    glutPostRedisplay();
}

void GraphicalOperations::fullDisplay() {
    localDisplay();
    controlDisplay();
}

