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
#include "../ShapeFiles/Moveable.h"
#include "../Physics/WorldSettings.h"

#include <iostream>
#include <string>

#include <plib/sg.h>
#define foreach_  BOOST_FOREACH                                                                                              

using namespace std;
using namespace boost::numeric::ublas;
using boost::numeric::ublas::compressed_vector;
using boost::shared_ptr;

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
    bool ptsHighlighted;
    float kineticEnergy;

  public:
    /*! \brief Sets default values of members common to all shapes
    */
    MyShape();

    virtual ~MyShape();

    /*! \brief Returns points used for outline markers
     *
     *  This was useful when originally getting the linear algebra figured out for openGL, but has been put on the shelf for the time being
     */
    virtual matrix<double> getPts();

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
    void draw() const;


    /*! \brief Use shape dimensions to scale points before drawing
     *
     *  Implementation varies depending on shape: will use side for squares, radius for circles, etc
     */
    virtual void drawScale() const;

    //! Draw unit shape (should be called last in draw() )
    virtual void drawUnit() const;

    //! Purely virtual method used by Circle
    virtual void setRadius(float );
    //! Purely virtual method used by Circle
    virtual float getRadius() const;

    //! Calculates orientationMat based on orientationQuat
    void calcRotMat();

    //! Sets momentum of object to <inX, inY, inZ>
    void setMomentum(float inX, float inY, float inZ);
    //! Sets momentum of object to <newMomentum>
    void setMomentum(const sgVec4 newMomentum);

    //! Alters momentum by <dx, dy, dz>
    void adjustMomentum(float dx, float dy, float dz);
    //! Alters momentum by <dMomentum>
    void adjustMomentum(const sgVec4 dMomentum);

    //! Returns momentum of object in retVec
    void getMomentum(sgVec4 retVec) const;

    //! Print momentum of object. Format: <##,##,##>
    std::string getMomentumString() const;

    /*! \brief Calculates the moment of inertia for the object
     *
     *  Calculation varies for different types of object
     *  \return Moment of Inertia value
     */
    virtual float getMomentOfInertia() const;

    //! Sets angular momentum of object to <newAngMomentum>
    void setAngMomentum(const sgVec4 newAngMomentum);
    //! Alters angular momentum of object by <dAngMomentum>
    void adjustAngMomentum(const sgVec4 dAngMomentum);
    //! Returns angular momentum of object in retVec
    void getAngMomentum(sgVec4 retVec) const;

    //! Sets mass of object to newMass
    void setMass(float newMass);
    //! Alters mass of object by dMass
    void adjustMass(float dMass);
    //! Returns mass of object
    float getMass() const;

    //! Sets density of object to newDensity
    void setDensity(float newDensity);
    //! Returns density of object
    float getDensity() const;

    //! Sets kineticEnergy of object to newDensity
    bool setKineticEnergy(float newKineticEnergy);
    //! Returns kineticEnergy of object
    float getKineticEnergy() const;

    //! Sets color of object to <newColor>
    void setColor(const sgVec3 newColor);
    //! Returns color of object in retVec
    void getColor(sgVec3 retVec) const;

    /*! \brief Calculates color of object based on how a star of that mass/density would burn
     *
     *  Right now this function only handles stars near the size of the Sun
     */
    void calcColor();

    //! Another relic from figuring out the linear algebra, keeping around just in case
    virtual float getMarkerSize();

    /*! \brief Returns an integer indicating the type of shape
     *
     *  Possible values:
     *  \n 1: Circle
     *  \n 2: Box
     *  \n Rest to be determined
     */
    virtual int getType() const;

    /*! \brief Vector that holds all currently active shapes
     *
     *  One of the biggest decisions still to be made is how/if to alter this to make it less scary
     */
    // static vector<MyShape *> shapes;
    //PHYS-7
    typedef shared_ptr<MyShape> shape_pointer;
    static compressed_vector<shape_pointer> shapes;

    static void clearShapes();

    /*! \brief Returns a moveable from the main moveables list
     *
     *  /param moveableIndex The index of the moveable you want to retrieve from the main moveables list
     *  \return Pointer to desired moveable
     */
    static int addShapeToList( shape_pointer insertShape );

    static void removeShapeFromList( shape_pointer shapeToRemove );
};
#endif /* MYSHAPE_H_ */
