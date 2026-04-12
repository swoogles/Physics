//
// Created by bfrasure on 1/12/19.
//

#ifndef PHYSICS_OPENGLSETUP_H
#define PHYSICS_OPENGLSETUP_H

#include <WindowDimensions.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include <plib/pu.h>

#include "inputFunctions.h"

#define FRAME_DELAY_MS 1000

class OpenGlSetup {
public:
    void initialize(WindowDimensions dimensions, void (*idleFunction)(void));

    void configureControlWindow(WindowDimensions mainWindowDimensions);

    int mainDisplayNum;
    int controlCenterNum;
};


#endif //PHYSICS_OPENGLSETUP_H
