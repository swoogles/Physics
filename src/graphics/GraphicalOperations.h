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
    GraphicalOperations(int CenterStageWindow, int controlCenterWindow,
                        WindowDimensions windowDimensions);

    void localDisplay(Simulation & simulation) const;
    void controlDisplay() const;
    void fullDisplay(Simulation & simulation) const;
    WindowDimensions currentDimensions() const;

    void updateObserver(
            MaximumValues maximumValues
            );
private:
    Observer localObserver;

    int control_center_num;
    int mainDisplayNum;
};


#endif //PHYSICS_GRAPHICALOPERATIONS_H
