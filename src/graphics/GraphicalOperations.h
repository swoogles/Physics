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
    GraphicalOperations(void (*callback)(void), SimulationPtr_t simulation, ControlCenter controlCenter,
                     shared_ptr<Observer> observer, WindowDimensions dimensions);

    shared_ptr<Observer> localObserver;
    SimulationPtr_t localSimulation;
    ControlCenter localControlCenter;
    void localDisplay();
    void controlDisplay();
private:
    static void fullQuadrantDrawingFunction(ControlCenter controlCenter, Quadrant quadrant);

    void openGlInit();

    void configureControlWindow(WindowDimensions mainWindowDimensions);


    const int control_center_num;
};


#endif //PHYSICS_GRAPHICALOPERATIONS_H
