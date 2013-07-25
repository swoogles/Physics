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

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <iostream>

#include <plib/sg.h>

//#include "../Physics/Simulations.h"


#define PI 3.14159265

using namespace boost::numeric::ublas;
using boost::numeric::ublas::vector;

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
class MyShape {
protected:
	//vector<double> pos;
	GLint numPts;
	matrix<double> pts;
	//vector<double> color;
	bool ptsHighlighted;

	//matrix<double> scaleMat;
	//matrix<double> rotMat;
	//vector<double> toolVec;

	sgVec4 pos;
	sgQuat orientationQuat;
	sgMat4 orientationMat;

	sgVec4 angMomentum;
	sgVec4 angVelocity;
	sgVec4 prevAngVelocity;

	sgVec4 momentum;
	sgVec4 prevMomentum;
	float mass;
	float density;
  float kineticEnergy;

	sgVec3 color;


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


	//! Print outline coordinates
	void printPts();

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


	/*! \brief Use shape dimensions to scale points before drawing
	 *
	 *  Implementation varies depending on shape: will use side for squares, radius for circles, etc
	 */
	virtual void drawScale();


	//! Draw unit shape (should be called last in draw() )
	virtual void drawUnit();


	/*
	void scale(float);
	void scale(float, float);
	virtual void scaleMembers(float);
	virtual void scaleMembers(float, float);
	*/


	//! Set position of object to <inX, inY, inZ>
	void setPos(float inX, float inY, float inZ);

	//! Set position of object to <newPos>
	void setPos(sgVec4 newPos);

	//! Alter position of object by <dx, dy, dz>
	void adjustPos(float dx, float dy, float dz);

	//! Alter position of object by <dPos>
	void adjustPos(sgVec4 dPos);

	//! Return address of new sgVec4 holding position of object
	sgVec4 * getPos();

	//! Return position of object in retVec
	void getPos(sgVec4 retVec);


	/*! \brief Set orientation of object
	 *
	 *  /param xAngle Degrees from X-axis
	 *  /param yAngle Degrees from Y-axis
	 *  /param zAngle Degrees from Z-axis
	 */
	void setAngle(float xAngle, float yAngle, float zAngle);


	/*! \brief Rotate object around a specified axis
	 *
	 *  Parameters are turned into a quaternion then applied to orientationQuat using sgPostMultQuat()
	 *  \param dAngle Amount to be rotated
	 *  \param rotAxis Axis to rotate around
	 */
	void adjustAngle(const SGfloat dAngle, const sgVec3 rotAxis);


	//! Calculates orientationMat based on orientationQuat
	void calcRotMat();

	//! Sets momentum of object to <inX, inY, inZ>
	void setMomentum(float inX, float inY, float inZ);
	//! Sets momentum of object to <newMomentum>
	void setMomentum(sgVec4 newMomentum);
	//! Alters momentum by <dx, dy, dz>
	void adjustMomentum(float dx, float dy, float dz);
	//! Alters momentum by <dMomentum>
	void adjustMomentum(sgVec4 dMomentum);
	//! Return address of new sgVec4 holding momentum of object
	sgVec4 * getMomentum();
	//! Returns momentum of object in retVec
	void getMomentum(sgVec4 retVec);


	//! Sets momentum of object to <inX, inY, inZ> * mass
	void setVelocity(float inX, float inY, float inZ);
	//! Sets momentum of object to <newVel> * mass
	void setVelocity(sgVec4 newVel);
	//! Alters momentum of object by <dx, dy, dy> * mass
	void adjustVelocity(float dx, float dy, float dz);
	//! Alters momentum of object by <dVel> * mass
	void adjustVelocity(sgVec4 dVel);
	//! Return address of new sgVec4 holding velocity of object
	sgVec4 * getVelocity();
	//! Returns velocity of object in retVec
	void getVelocity(sgVec4 retVec);

	/*! \brief Calculates the moment of inertia for the object
	 *
	 *  Calculation varies for different types of object
	 *  \return Moment of Inertia value
	 */
	virtual float getMomentOfInertia();

	//! Sets angular momentum of object to <newAngMomentum>
	void setAngMomentum(sgVec4 newAngMomentum);
	//! Alters angular momentum of object by <dAngMomentum>
	void adjustAngMomentum(sgVec4 dAngMomentum);
	//! Returns angular momentum of object in retVec
	void getAngMomentum(sgVec4 retVec);

	//! Sets angular velocity of object to <newAngVelocity>
	void setAngVelocity(sgVec4 newAngVelocity);
	//! Alters angular velocity of object by <dAngVelocity>
	void adjustAngVelocity(sgVec4 dangVelocity);
	//! Returns angular velocity of object in retVec
	void getAngVelocity(sgVec4 retVec);

	//! Sets mass of object to newMass
	void setMass(float newMass);
	//! Alters mass of object by dMass
	void adjustMass(float dMass);
	//! Returns mass of object
	float getMass();

	//! Sets density of object to newDensity
	void setDensity(float newDensity);
	//! Returns density of object
	float getDensity();

	//! Sets kineticEnergy of object to newDensity
	bool setKineticEnergy(float newKineticEnergy);
	//! Returns kineticEnergy of object
	float getKineticEnergy();

	//! Sets color of object to <newColor>
	void setColor(sgVec3 newColor);
	//! Returns color of object in retVec
	void getColor(sgVec3 retVec);

	/*! \brief Calculates color of object based on how a star of that mass/density would burn
	 *
	 *  Right now this function only handles stars near the size of the Sun
	 */
	void calcColor();

	//! Moves object based on current normal and angular momentum
	void update(float);

	/*
	void hFlip();
	void vFlip();
	*/

	//! Another relic from figuring out the linear algebra, keeping around just in case
	virtual float getMarkerSize();

	//! Purely virtual method used by Circle
	virtual void setRadius(float );
	//! Purely virtual method used by Circle
	virtual float getRadius();

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
	static vector<MyShape *> shapes;

	/*! \brief Returns a shape from the main shapes list
	 *
	 *  /param shapeIndex The index of the shape you want to retrieve from the main shapes list
	 *  \return Pointer to desired shape
	 */
  static MyShape * getShapeFromList( int shapeIndex );

	/*! \brief Returns a shape from the main shapes list
	 *
	 *  /param shapeIndex The index of the shape you want to retrieve from the main shapes list
	 *  \return Pointer to desired shape
	 */
  static int addShapeToList( MyShape * insertShape );



	//Get rid of this and store in WorldSettings instead
	static float G;

   static void clearShapes();

};


#endif /* MYSHAPE_H_ */

//******Non-Class Functions*****

