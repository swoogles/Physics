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
#include "ShapeFiles/MyShape.h"
#include "Observation/Observer.h"
#include "Physics/WorldSettings.h"
#include "Windows/control_center.h"
#include <boost/shared_ptr.hpp>

#include <plib/pu.h>
#include <plib/puAux.h>

#ifndef GLUT_WHEEL_UP
# define GLUT_WHEEL_UP 3
# define GLUT_WHEEL_DOWN 4
#endif

using namespace std;

class inputFunctions {
  private:
    boost::shared_ptr<Observer> observer;
  public:
    inline boost::shared_ptr<Observer> setObserver() { return observer ; };
    inline void setObserver( boost::shared_ptr<Observer> observer ) { this->observer = observer; };

static void myMouse(int button, int state, int x, int y );

static void myKey(unsigned char, int x, int y);
};

#endif /* INPUTFUNCTIONS_H_ */
