#ifndef PHYSICS_DRAWING_H
#define PHYSICS_DRAWING_H

#include <GL/gl.h>
#include <GL/glut.h>

#include "Dimensions/Moveable.h"

class Drawing {
public:
    static void draw(Moveable &myShape);
};

#endif