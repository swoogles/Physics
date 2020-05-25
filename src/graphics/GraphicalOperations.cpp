#include <utility>

//
// Created by bfrasure on 11/17/18.
//

#include "GraphicalOperations.h"

using std::size_t;;;

void processMouseAction(
        Observer & observer,
        MouseAction  mouseAction
) {
    switch(mouseAction) {
        case MouseAction::SCROLL_UP:
            observer.zoomIn();
            break;
        case MouseAction::SCROLL_DOWN:
            observer.zoomOut();
            break;
    }
    observer.setAutoScaling(false);
}

void processCameraAction(
        Observer & observer,
        CameraAction cameraAction
) {
    switch(cameraAction) {
        case CameraAction::ROTATE_LEFT: {
            PhysicalVector leftAngVelocity(0, -.5f, 0);
            observer.adjustAngularVelocity(leftAngVelocity);
            break;
        }
        case CameraAction::ROTATE_RIGHT: {
            PhysicalVector rightAngVelocity(0, .5f, 0);
            observer.adjustAngularVelocity(rightAngVelocity);
            break;
        }
        case CameraAction::ROTATE_UP: {
            PhysicalVector upAngVelocity(+0.5f, 0, 0);
            observer.adjustAngularVelocity(upAngVelocity);
            break;
        }
        case CameraAction::ROTATE_DOWN: {
            PhysicalVector downAngVelocity(-0.5f, 0, 0);
            observer.adjustAngularVelocity(downAngVelocity);
        }
        case TOGGLE_AUTOSCALING: {
            break;
        }
        case STOP_ROTATION: {
            PhysicalVector stoppedAngVelocity(0,0,0);
            observer.adjustAngularVelocity(stoppedAngVelocity);
            break;
        }
    }
}

void GraphicalOperations::localDisplay(Simulation & simulation) const {
    glutSetWindow(mainDisplayNum);
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    localObserver.applyView();

    glMatrixMode(GL_MODELVIEW);

    Drawing drawing;

    // TODO You know this is bad. Keep looking at it.
    //    if (controlCenter.shouldRenderOctree()) {
    simulation.applySideEffectingFunctionsToInnards(
            [this, drawing](const Quadrant & quadrant) {
//                drawing.draw(quadrant);
            },
            [this, drawing](const Particle & particle) {
                if (particle.mass() != kilogram_t(0)) drawing.draw(particle);
            }
    );

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    puDisplay(mainDisplayNum);
    glutSwapBuffers();
    glutPostRedisplay();

}

GraphicalOperations::GraphicalOperations(ControlCenter controlCenter,
                                         int CenterStageWindow, int controlCenterWindow,
                                         WindowDimensions windowDimensions)
        : localControlCenter(controlCenter)
        , mainDisplayNum(CenterStageWindow)
        , control_center_num(controlCenterWindow)
        , localObserver(windowDimensions)
{
}

void GraphicalOperations::controlDisplay() const {
    glutSetWindow(control_center_num);
    glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    puDisplay(control_center_num);

    glutSwapBuffers();
    glutPostRedisplay();
}

void GraphicalOperations::fullDisplay(Simulation & simulation) const {
    localDisplay(simulation);
    controlDisplay();
}

WindowDimensions GraphicalOperations::currentDimensions() const {
    return WindowDimensions(
            glutGet(GLUT_WINDOW_X),
            glutGet(GLUT_WINDOW_Y),
            glutGet(GLUT_WINDOW_HEIGHT),
            glutGet(GLUT_WINDOW_WIDTH)
            );
}

void GraphicalOperations::updateObserver(
        MaximumValues maximumValues
        ) {
    auto mouseAction = InputFunctions::currentMouseAction();
    if (mouseAction.has_value()) {
        processMouseAction(localObserver, mouseAction.value());
    }

    auto cameraAction = ControlCenter::currentCameraAction();
    if (cameraAction.has_value()) {
        processCameraAction(localObserver, cameraAction.value());
    }

    localObserver.update();

    // TODO This would be more valuable if it only tried to include the largest N items.
    // It shouldn't pan out to catch every last tiny particle that gets thrown towards infinity.
    localObserver.calcMinPullback(maximumValues);
    localObserver.setAutoScaling(false); // TODO Where to put this? I Only need it executed one time.
}

