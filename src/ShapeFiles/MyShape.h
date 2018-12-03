#ifndef MYSHAPE_H_
#define MYSHAPE_H_

#include <cmath>
#include <vector>

#include "../Dimensions/Moveable.h"
#include "Dimensions/PhysicalVector.h"
#include "ShapeType.h"

#include <../lib/units.h>

using namespace units::mass;
using namespace units::length;

using std::shared_ptr;

/*! \brief The class at the heart of all the physical calculations
 *
 *  This class holds all the attributes needed to model real world objects:
 *  \n -Mass
 *  \n -Density
 *  \n -Dimensions
 *  \n -Position
 *  \n -Orientation
 *  \n -Momentum (Normal and Angular)
 *  \n -Color
 *
 *  \n \n Currently Circles are the only objects that have been really developed, as they are the easiest starting point physically
 */
class MyShape : public Moveable {
private:
    const ShapeType shapeType;

  public:
    /*! \brief Sets default values of members common to all shapes
    */
    MyShape(ShapeType shapeType, PhysicalVector momentum);

    virtual double scale();

    void adjustMomentum(PhysicalVector dMomentum);
    PhysicalVector momentum();
    virtual double momentOfInertia();
    void setAngularMomentum(PhysicalVector newAngMomentum);
    PhysicalVector angularMomentum();
    void setMass(kilogram_t newMass);
    kilogram_t mass();
    kilograms_per_cubic_meter_t density();
    PhysicalVector getColor() const;

    /*!
     * TODO This should at *least* go to the circle class, but I think it really belongs in the Physics package
     * \brief Calculates color of object based on how a star of that mass/density would burn
     */
    void calcColor();
    ShapeType getType();

    /*!
      * Simple method for getting distance between 2 objects
      */
    double distanceTo(MyShape &object2);

    PhysicalVector weightedPosition();

};

typedef shared_ptr<MyShape> shapePointer_t;
typedef typename std::vector<shapePointer_t> vectorT;

#endif
