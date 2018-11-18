//
// Created by bfrasure on 11/17/18.
//

#ifndef PHYSICS_GRAPHICALOPERATIONS_H
#define PHYSICS_GRAPHICALOPERATIONS_H

#include <GL/glut.h>
#include <GL/glu.h>

#include "Observation/Observer.h"
#include "ShapeFiles/Drawing.h"
#include "Windows/control_center.h"

#include <plib/pu.h>
#include <plib/puAux.h>

#include "inputFunctions.h"
#include "WindowDimensions.h"

#include <Physics/Simulation.h>

#define WW 5
#define WH 5

class GraphicalOperations {
public:
    static void display();
    GraphicalOperations();

    int mainGlut(int argcp, char **argv, void (*callback)(void), SimulationPtr_t simulation,
                 control_center controlCenter,
                 shared_ptr<Observer> observer, WindowDimensions dimensions);
    void configureControlWindow(
            int mainWinPosX,
            int mainWinPosY,
            int mainWinHeight,
            int mainWinWidth
    );

    // Make private, at the very least
    static SimulationPtr_t staticSimulation;
    static control_center staticControlCenter;
private:
    SimulationPtr_t globalSimulation;
    control_center globalControlCenter;

    static shared_ptr<Observer> observer;

    static void fullQuadrantDrawingFunction(Quadrant quadrant);

    void openGlInit();
};


#endif //PHYSICS_GRAPHICALOPERATIONS_H
