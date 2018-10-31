#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "MyShape.h"

class Circle: public MyShape {
  private:
    float radius;
  public:
    /* TODO Constructors should allow for 2 out of 3 these fields
     * mass
     * radius
     * density
     * Whichever one is missing can be derived from other 2
     * This would allow me to ditch one of these fields, making objects smaller
     */

    Circle(
            sgVec4 pos,
            float mass,
            float radius,
            sgVec4 momentum,
            // TODO Density should be derived from mass/radius
            float density,
            sgVec3 color
    );

    Circle(
            sgVec4 pos,
            float mass,
            sgVec4 momentum,
            float density,
            sgVec3 color
    );

    Circle(
            sgVec4 pos,
            float mass,
            float radius,
            sgVec4 momentum,
            sgVec3 color
    );

    virtual ~Circle();

    float getScale();

    float getRadius();
    void setRadius(float);

    float getMomentOfInertia();

    ShapeType getType();


    static float calcRadius(float mass, float density);
};
#endif
