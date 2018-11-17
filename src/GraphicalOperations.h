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

#include <Physics/Simulation.h>

class GraphicalOperations {
public:
    static void display();
    GraphicalOperations(SimulationPtr_t simulation, control_center controlCenter);

    // Make private, at the very least
    static SimulationPtr_t staticSimulation;
    static control_center staticControlCenter;
private:
    SimulationPtr_t globalSimulation;
    control_center globalControlCenter;

    function<void(Quadrant)> quadrantDrawingFunction;

    static void fullQuadrantDrawingFunction(Quadrant quadrant);

};


#endif //PHYSICS_GRAPHICALOPERATIONS_H
