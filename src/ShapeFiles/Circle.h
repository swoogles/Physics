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
            float radius,
            sgVec4 momentum,
            float density,
            sgVec3 color
    );
    // Circle( Circle& );
    // Circle( const Circle& );
    virtual ~Circle();

    float getScale();

    float getRadius();
    void setRadius(float);

    float getMomentOfInertia();

    ShapeType getType();
};
#endif
