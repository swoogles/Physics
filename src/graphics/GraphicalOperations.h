//
// Created by bfrasure on 11/17/18.
//

#ifndef PHYSICS_GRAPHICALOPERATIONS_H
#define PHYSICS_GRAPHICALOPERATIONS_H

#include <GL/glut.h>
#include <GL/glu.h>

#include "Observation/Observer.h"
#include "Drawing.h"
#include "Windows/ControlCenter.h"

#include <plib/pu.h>
#include <plib/puAux.h>

#include "inputFunctions.h"
#include "WindowDimensions.h"

#include <Physics/Simulation.h>

#define WW 5
#define WH 5

class GraphicalOperations {
public:
    GraphicalOperations(Simulation & simulation, ControlCenter controlCenter,
                        int CenterStageWindow, int controlCenterWindow,
                        WindowDimensions windowDimensions);

    shared_ptr<Observer> localObserver;
    Simulation & localSimulation;
    ControlCenter localControlCenter;
    void localDisplay();
    void controlDisplay();
    void fullDisplay();
    WindowDimensions currentDimensions();
private:
    // TODO replace ControlCenter param with a simple "shouldRenderOctree" bool
    static void fullQuadrantDrawingFunction(ControlCenter controlCenter, Quadrant quadrant);

    void configureControlWindow(WindowDimensions mainWindowDimensions);


    int control_center_num;
    int mainDisplayNum;
};


#endif //PHYSICS_GRAPHICALOPERATIONS_H
