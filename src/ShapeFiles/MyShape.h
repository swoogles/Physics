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

    virtual double getScale();

    //! Alters momentum by <dMomentum>
    void adjustMomentum(PhysicalVector dMomentum);

    //! Returns momentum of object in retVec
    PhysicalVector getMomentum();

    /*! \brief Calculates the moment of inertia for the object
     *
     *  Calculation varies for different types of object
     *  \return Moment of Inertia value
     */
    virtual double getMomentOfInertia();

    //! Sets angular momentum of object to <newAngMomentum>
    void setAngMomentum(PhysicalVector newAngMomentum);

    //! Returns angular momentum of object in retVec
    PhysicalVector getAngMomentum();

    //! Sets mass of object to newMass
    void setMass(kilogram_t newMass);

    //! Returns mass of object
    kilogram_t getMass();

    //! Returns density of object
    kilograms_per_cubic_meter_t getDensity();

    PhysicalVector getColor() const;

    /*!
     * TODO This should at *least* go to the circle class, but I think it really belongs in the Physics package
     *
     * \brief Calculates color of object based on how a star of that mass/density would burn
     *
     *  Right now this function only handles stars near the size of the Sun
     */
    void calcColor();

    /*! \brief Returns an integer indicating the type of shape
     *
     *  Possible values:
     *  \n 1: Circle
     *  \n 2: Box
     *  \n Rest to be determined
     */
    ShapeType getType();

    /*!
      * Simple method for getting distance between 2 objects
      */
    double distanceTo(MyShape &object2);

    PhysicalVector getWeightedPosition();

};

typedef shared_ptr<MyShape> shapePointer_t;
typedef typename std::vector<shapePointer_t> vectorT;

#endif
