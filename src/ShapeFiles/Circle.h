#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "MyShape.h"

class Circle: public MyShape {
  private:
    GLdouble radius;
    GLint numPts;
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

    void drawScale();
    void drawUnit();

    float getRadius();
    void setRadius(float);

    float getMomentOfInertia();

    int getType();
};
#endif
