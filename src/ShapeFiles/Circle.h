#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "MyShape.h"

class Circle: public MyShape {
  private:
    float radius;
  public:

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
