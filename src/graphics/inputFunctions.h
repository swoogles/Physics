/*
 * inputFunctions.h
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#ifndef INPUTFUNCTIONS_H_
#define INPUTFUNCTIONS_H_

#include <iostream>
#include <GL/glut.h>
#include "Dimensions/Moveable.h"
#include "Observation/Observer.h"
#include "Windows/control_center.h"

#include <plib/pu.h>
#include <plib/puAux.h>

#ifndef GLUT_WHEEL_UP
# define GLUT_WHEEL_UP 3
# define GLUT_WHEEL_DOWN 4
#endif

class InputFunctions {
public:
    static void init(shared_ptr<Observer> observer);
    static void myMouse(int button, int state, int x, int y);
    static void myKey(unsigned char, int x, int y);
private:
    static shared_ptr<Observer> observer;
    static void stopAutoScaling();
};

#endif /* INPUTFUNCTIONS_H_ */
