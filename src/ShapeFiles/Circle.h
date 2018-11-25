#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "MyShape.h"

#include <../lib/units.h>

using namespace units::mass;
using namespace units::length;

class Circle: public MyShape {
  private:
//    float radius;
    meter_t radius;

  public:

    Circle(
            sgVec4 pos,
            kilogram_t mass,
            sgVec4 momentum,
            kilograms_per_cubic_meter_t density,
            sgVec3 color
    );

    Circle(
            sgVec4 pos,
            double mass,
            float radius,
            sgVec4 momentum,
            sgVec3 color
    );

    virtual ~Circle();

    double getScale();

    meter_t getRadius();
    void setRadius(meter_t radius);

    double getMomentOfInertia();

    ShapeType getType();


    static meter_t calcRadius(kilogram_t mass, kilograms_per_cubic_meter_t density);

    static vector<VecStruct> pointsEvenlyDistributedOnSphere(int numPoints, float radius);
};

typedef shared_ptr<Circle> particle_t;
#endif
