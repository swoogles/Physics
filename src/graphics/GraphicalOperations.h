//
// Created by bfrasure on 11/17/18.
//

#ifndef PHYSICS_GRAPHICALOPERATIONS_H
#define PHYSICS_GRAPHICALOPERATIONS_H

#include <GL/glut.h>
#include <GL/glu.h>

#include "Observation/Observer.h"
#include "Drawing.h"
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
    GraphicalOperations();

    int mainGlut(void (*callback)(void), SimulationPtr_t simulation, control_center controlCenter,
                     shared_ptr<Observer> observer, WindowDimensions dimensions);
    void postSimulationGlInit();

private:
    static shared_ptr<Observer> observer;
    static SimulationPtr_t staticSimulation;
    static control_center staticControlCenter;

    static void display();

    static void fullQuadrantDrawingFunction(Quadrant quadrant);

    void openGlInit();

    void configureControlWindow(
            int mainWinPosX,
            int mainWinPosY,
            int mainWinHeight,
            int mainWinWidth
    );


    static int control_center_num;
    static void controlDisplay();
};


#endif //PHYSICS_GRAPHICALOPERATIONS_H
