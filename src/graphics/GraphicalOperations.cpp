#include <utility>

//
// Created by bfrasure on 11/17/18.
//

#include "GraphicalOperations.h"

using std::size_t;;;


void GraphicalOperations::localDisplay() {
    glutSetWindow(mainDisplayNum);
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    localObserver.applyView();

    glMatrixMode(GL_MODELVIEW);

    // TODO You know this is bad. Keep looking at it.
//    if (controlCenter.shouldRenderOctree()) {
    localSimulation
        .getQuadrant()->applyToAllChildren(
                [this](Quadrant & quadrant) {
                    Drawing::draw(quadrant);
                });
//    }
    localSimulation.getPhysicalObjects().applyToAllParticles(
            [this](Particle & particle) {
                if (particle.mass() != kilogram_t(0)) Drawing::draw(particle);
            }
            );

//    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    puDisplay(mainDisplayNum);
    glutSwapBuffers();
    glutPostRedisplay();

}

GraphicalOperations::GraphicalOperations(Simulation & simulation, ControlCenter controlCenter,
                                         int CenterStageWindow, int controlCenterWindow,
                                         WindowDimensions windowDimensions)
        : localSimulation(simulation)
        , localControlCenter(controlCenter)
        , mainDisplayNum(CenterStageWindow)
        , control_center_num(controlCenterWindow)
        , localObserver(windowDimensions)
{
}

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

WindowDimensions GraphicalOperations::currentDimensions() {
    return WindowDimensions(
            glutGet(GLUT_WINDOW_X),
            glutGet(GLUT_WINDOW_Y),
            glutGet(GLUT_WINDOW_HEIGHT),
            glutGet(GLUT_WINDOW_WIDTH)
            );
}

Observer & GraphicalOperations::getObserver() {
    return this->localObserver;
}

