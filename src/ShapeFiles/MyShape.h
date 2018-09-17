/*
 * MyShape.h
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#ifndef MYSHAPE_H_
#define MYSHAPE_H_

#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/foreach.hpp>
#include "../Dimensions/Moveable.h"
#include "../Dimensions/VecStruct.h"
#include "../Physics/WorldSettings.h"

#include <iostream>
#include <string>

#include <plib/sg.h>
#define foreach_  BOOST_FOREACH                                                                                              

using namespace std;
using namespace boost::numeric::ublas;
using boost::numeric::ublas::compressed_vector;
using boost::shared_ptr;

typedef boost::scoped_ptr<VecStruct> vecPtr;

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
  protected:
    GLint numPts;
    matrix<double> pts;

  public:
    /*! \brief Sets default values of members common to all shapes
    */
    MyShape();
    // MyShape( const MyShape& );

    virtual ~MyShape();

    /*! \brief The key MyShape function called in the display function
     *
     *  Steps:
     *  \n -Push current matrix onto the stack
     *  \n -Translate using the pos vector
     *  \n -Rotate using orientationMat
     *  \n -Scale using drawScale()
     *  \n -Call glColor3fv(float *) using color vector
     *  \n -Call drawUnit() to draw a unit square, circle, etc.
     *  \n -Pop matrix from the stack
     */
    void draw();

    /*
     * Refer to draw() docs.
     */
    static void drawAllShapes();


    /*! \brief Use shape dimensions to scale points before drawing
     *
     *  Implementation varies depending on shape: will use side for squares, radius for circles, etc
     */
    virtual void drawScale();

    //! Draw unit shape (should be called last in draw() )
    virtual void drawUnit();

    //! Purely virtual method used by Circle
    virtual void setRadius(float );
    //! Purely virtual method used by Circle
    virtual float getRadius();

    //! Alters momentum by <dMomentum>
    void adjustMomentum(const sgVec4 dMomentum);

    //! Returns momentum of object in retVec
    void getMomentum(sgVec4 retVec);

    //! Returns momentum of object in retVec
    VecStruct * getMomentum();

    /*! \brief Calculates the moment of inertia for the object
     *
     *  Calculation varies for different types of object
     *  \return Moment of Inertia value
     */
    virtual float getMomentOfInertia();

    //! Sets angular momentum of object to <newAngMomentum>
    void setAngMomentum(sgVec4 newAngMomentum);
    //! Alters angular momentum of object by <dAngMomentum>
    void adjustAngMomentum(const sgVec4 dAngMomentum);
    //! Returns angular momentum of object in retVec
    void getAngMomentum(sgVec4 retVec);

    //! Sets mass of object to newMass
    void setMass(float newMass);
    //! Alters mass of object by dMass
    void adjustMass(float dMass);
    //! Returns mass of object
    float getMass();

    //! Returns density of object
    float getDensity();

    //! Returns color of object in retVec
    void getColor(sgVec3 retVec) const;

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
    virtual int getType();

    /*! \brief Vector that holds all currently active shapes
     *
     *  One of the biggest decisions still to be made is how/if to alter this to make it less scary
     */
    // static vector<MyShape *> shapes;
    //PHYS-7
    typedef boost::shared_ptr<MyShape> shapePointer_t;
    static compressed_vector<shapePointer_t> shapes;

    static void drawShapes(compressed_vector<shapePointer_t> shapes);

    static void clearShapes();

    /*! \brief Returns a moveable from the main moveables list
     *
     *  /param moveableIndex The index of the moveable you want to retrieve from the main moveables list
     *  \return Pointer to desired moveable
     */
    static int addShapeToList( shapePointer_t insertShape, compressed_vector< shapePointer_t > shapes );

    static void removeShapeFromList( shapePointer_t shapeToRemove, compressed_vector< shapePointer_t > shapes );

    /*! \relates MyShape
     *  \brief Finds a vector pointing from object1 to object2
     *
     *  \param object1 Start Object
     *  \param object2 End Object
     *  \param sepVector Calculated separation vector
     */
    void getVectorToObject( shapePointer_t object2, sgVec4 sepVector);

    /*! 
      * Simple method for getting distance between 2 objects
      */
    float getDistanceToObject( shapePointer_t object2 );

    bool isTouching( shapePointer_t otherShape );

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
    void mergeWith( shapePointer_t otherShape );

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
    void calcMergedAngMomentum( shapePointer_t otherShape, sgVec4 totalAngMom );

    /*! \relates Circle
     *  \brief Finds radius after 2 circular objects are merged
     *
     */
    float calcMergedRadius(float massBoth, float density);

    VecStruct * getCenterOfMass();
};

typedef boost::shared_ptr<MyShape> shapePointer_t;
#endif /* MYSHAPE_H_ */
