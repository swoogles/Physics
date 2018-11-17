//
// Created by bfrasure on 11/17/18.
//

#include "GraphicalOperations.h"

SimulationPtr_t GraphicalOperations::staticSimulation;
control_center GraphicalOperations::staticControlCenter;

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

GraphicalOperations::GraphicalOperations(SimulationPtr_t simulation, control_center controlCenter) {
    this->quadrantDrawingFunction = [&controlCenter](Quadrant quadrant) {
        if (controlCenter.shouldRenderOctree()) {
            drawingFuncFixed(quadrant);
        }
        drawShapeInQuadrant(quadrant);
    };

}
