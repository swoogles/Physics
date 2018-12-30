#ifndef PHYSICS_DRAWING_H
#define PHYSICS_DRAWING_H

#include <GL/gl.h>
#include <GL/glut.h>

#include "Dimensions/Moveable.h"

class Drawing {
public:
    /*! \brief The key MyShape function called in the display function
     *
     *  Steps:
     *  \n -Push current matrix onto the stack
     *  \n -Translate using the pos vector
     *  \n -Rotate using orientationMat
     *  \n -Scale using drawScale()
     *  \n -Call glColor3fv(float *) using color vector
     *  \n -Call drawUnit() to draw a unit square, circle, etc.
     *  \n -Pop matrix from the stack
     */
    static void draw(shapePointer_t myShape);
    static void draw(Moveable &myShape);
    static void drawShapes(const vectorT &shapes);

};

#endif