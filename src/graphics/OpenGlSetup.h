//
// Created by bfrasure on 1/12/19.
//

#ifndef PHYSICS_OPENGLSETUP_H
#define PHYSICS_OPENGLSETUP_H


#include <WindowDimensions.h>
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

class OpenGlSetup {
public:
    void initialize(
            WindowDimensions dimensions
            );

    void configureControlWindow(WindowDimensions mainWindowDimensions);

    int mainDisplayNum;
    int controlCenterNum;
};


#endif //PHYSICS_OPENGLSETUP_H
