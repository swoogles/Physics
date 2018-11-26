//
// Created by bfrasure on 8/10/18.
//

#ifndef PHYSICS_TESTUTILS_H
#define PHYSICS_TESTUTILS_H

#include "ShapeFiles/Particle.h"
#include <plib/sg.h>

class TestUtils {
public:
    static shared_ptr<Particle> testCircle();
    static shared_ptr<Particle> circleAt(float x, float y, float z);
    static Particle simpleCircleAt(float x, float y, float z);
    static unique_ptr<Particle> improvedTestCircle();

};


#endif //PHYSICS_TESTUTILS_H
