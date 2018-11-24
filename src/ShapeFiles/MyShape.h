#ifndef MYSHAPE_H_
#define MYSHAPE_H_

#include <cmath>
#include <vector>

#include "../Dimensions/Moveable.h"
#include "../Dimensions/VecStruct.h"
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
    VecStruct calcMergedAngMomentum(MyShape &otherShape);

  public:
    /*! \brief Sets default values of members common to all shapes
    */
    MyShape();

    virtual float getScale();

    //! Purely virtual method used by Circle
    virtual void setRadius(float );
    //! Purely virtual method used by Circle
    virtual meter_t getRadius();

    //! Alters momentum by <dMomentum>
    void adjustMomentum(VecStruct dMomentum);

    //! Returns momentum of object in retVec
    VecStruct getMomentum();

    /*! \brief Calculates the moment of inertia for the object
     *
     *  Calculation varies for different types of object
     *  \return Moment of Inertia value
     */
    virtual float getMomentOfInertia();

    //! Sets angular momentum of object to <newAngMomentum>
    void setAngMomentum(VecStruct newAngMomentum);

    //! Returns angular momentum of object in retVec
    VecStruct getAngMomentum();

    //! Sets mass of object to newMass
    void setMass(kilogram_t newMass);

    //! Returns mass of object
    kilogram_t getMass();

    //! Returns density of object
    kilograms_per_cubic_meter_t getDensity();

    VecStruct getColor() const;

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
    virtual ShapeType getType();

    /*!
      * Simple method for getting distance between 2 objects
      */
    float distanceTo(MyShape &object2);

    bool isTouching(MyShape &otherShape);

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
    void mergeWith(MyShape &otherShape);

    /*! \relates Circle
     *  \brief Finds radius after 2 circular objects are merged
     *
     */
    static float calcRadius(float massBoth, float density);

    VecStruct getWeightedPosition();

};

typedef shared_ptr<MyShape> shapePointer_t;
typedef typename std::vector<shapePointer_t> vectorT;

#endif
