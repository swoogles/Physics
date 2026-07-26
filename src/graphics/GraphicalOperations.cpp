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

//    PhysicalVector rotVec;
//    rotVec = PhysicalVector(1,  0, 0);
//    adjustAngle(angVelocity.x(), rotVec);
//    rotVec = PhysicalVector(0,  1, 0);
//    adjustAngle(angVelocity.y(), rotVec);
//    rotVec = PhysicalVector(0,  0, 1);
//    adjustAngle(angVelocity.z(), rotVec);

    switch(cameraAction) {
        case CameraAction::ROTATE_LEFT: {
            PhysicalVector leftAngVelocity(0, -.5f, 0);
            observer.adjustAngle(leftAngVelocity);
            break;
        }
        case CameraAction::ROTATE_RIGHT: {
            PhysicalVector rightAngVelocity(0, .5f, 0);
            observer.adjustAngle(rightAngVelocity);
            break;
        }
        case CameraAction::ROTATE_UP: {
            PhysicalVector upAngVelocity(+0.5f, 0, 0);
            observer.adjustAngle(upAngVelocity);
            break;
        }
        case CameraAction::ROTATE_DOWN: {
            PhysicalVector downAngVelocity(-0.5f, 0, 0);
            observer.adjustAngle(downAngVelocity);
        }
        case TOGGLE_AUTOSCALING: {
            observer.toggleAutoScaling();
            break;
        }
        case STOP_ROTATION: {
            PhysicalVector stoppedAngVelocity(0,0,0);
            observer.adjustAngularVelocity(stoppedAngVelocity);
            break;
        }
        case CameraAction::ZOOM_IN: {
            observer.zoomIn();
            break;
        }
        case CameraAction::ZOOM_OUT: {
            observer.zoomOut();
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

    /* Particles are drawn as GL_POINTS, which default to aliased squares - very
     * obvious once a merged body is ten pixels wide. Smoothing renders them as
     * round dots, and needs blending on for the antialiased edge. */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    //    if (controlCenter.shouldRenderOctree()) {
    simulation.applySideEffectingFunctionsToInnards(
            [this, drawing](const Quadrant & quadrant) {
//                drawing.draw(quadrant);
            },
            [this, drawing](const Particle & particle) {
                if (particle.mass() != kilogram_t(0)) drawing.draw(particle);
            }
    );

    // Back to plain state so the plib UI below draws as it always has.
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_BLEND);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    puDisplay(mainDisplayNum);
    glutSwapBuffers();

}

GraphicalOperations::GraphicalOperations(int CenterStageWindow, int controlCenterWindow,
                                         WindowDimensions windowDimensions)
        : mainDisplayNum(CenterStageWindow)
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
    // Drain all pending mouse inputs so zoom responds immediately.
    while (true) {
        auto mouseAction = InputFunctions::currentMouseAction();
        if (!mouseAction.has_value()) {
            break;
        }
        processMouseAction(localObserver, mouseAction.value());
    }

    // Keep camera pans smooth by applying only a tiny number of
    // queued camera actions each frame.
    constexpr size_t maxCameraActionsPerFrame = 1;
    size_t cameraActionsProcessed = 0;
    while (cameraActionsProcessed < maxCameraActionsPerFrame) {
        auto cameraAction = ControlCenter::currentCameraAction();
        if (!cameraAction.has_value()) {
            break;
        }
        processCameraAction(localObserver, cameraAction.value());
        cameraActionsProcessed++;
    }

    localObserver.update();

    localObserver.calcMinPullback(maximumValues);
}
