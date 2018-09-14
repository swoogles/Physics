#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "MyShape.h"

#define PI 3.14159265

class Circle: public MyShape {
  private:
    GLdouble radius;
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

    matrix<double> getPts();

    float getMarkerSize();
    void scaleMembers(float);

    void drawScale();
    void drawUnit();

    float getRadius();
    void setRadius(float);

    float getMomentOfInertia();

    int getType();
};
#endif
