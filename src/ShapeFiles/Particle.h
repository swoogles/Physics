#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "MyShape.h"

#include <../lib/units.h>

using namespace units::mass;
using namespace units::length;

class Particle: public MyShape {
  private:
//    float radius;
    meter_t radius;

    /*! \relates MyShape
     *  \brief Determines the final angular momentum after 2 objects collide in a completely inelastic collision
     *
     *  The final angular momentum is determined with the following formula:
     *  \n v1i = the initial velocity of object 1
     *  \n v2i = the initial velocity of object 2
     *	\n m1 = the mass of object 1
     *	\n m2 = the mass of object 2
     *	\n e = the coefficient of restitution (e = 1 for elastic collision)
     *	\n n = normal unit vector drawn from object 1 to object 2
     *	\n c = n . (v1i - v2i)
     *	\n v1f = v1i - ((m2c)/(m1 + m2))(1 + e)n
     *	\n v2f = v2i + ((m1c)/(m1 + m2))(1 + e)n
     *
     *	\return The angular momentum to be assigned to the merged object
     */
    VecStruct calcMergedAngMomentum(Particle &otherShape);

  public:

    Particle(
            sgVec4 pos,
            kilogram_t mass,
            VecStruct momentum,
            kilograms_per_cubic_meter_t density,
            sgVec3 color
    );

    Particle(
            sgVec4 pos,
            double mass,
            float radius,
            VecStruct momentum,
            sgVec3 color
    );

    double getScale() override;

    meter_t getRadius();
    void setRadius(meter_t radius);

    double getMomentOfInertia() override;

    static meter_t calcRadius(kilogram_t mass, kilograms_per_cubic_meter_t density);

    static vector<VecStruct> pointsEvenlyDistributedOnSphere(int numPoints, float radius);

    /*! \relates MyShape
     *  \brief Combines 2 distinct objects in an inelastic collision into the first object and eliminates the second
     *
     *  Combines 2 objects by:
     *  \n -Calculating their combined radius (calcMergedRadius)
     *  \n -Finding their center of mass and centering the new object there
     *  \n -Adding their masses and momentums
     *  \n -Calculating their combined angular momentum about the COM (calcMergedAngMomentum)
     *
     *  Note: This function does NOT delete the second object, currently that must be handled outside
     */
    void mergeWith(Particle &otherShape);

    bool isTouching(Particle &otherShape);
};

typedef shared_ptr<Particle> particle_t;
#endif
