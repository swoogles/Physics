/*
 * Moveable.h
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#ifndef MOVEABLE_H_
#define MOVEABLE_H_

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
class Moveable {
protected:
	//vector<double> pos;
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

	sgVec3 color;


public:
	/*! \brief Sets default values of members common to all shapes
	 */
	Moveable();

	virtual ~Moveable();

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


	//! Moves object based on current normal and angular momentum
	void update(float);

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

	//! Sets color of object to <newColor>
	void setColor(sgVec3 newColor);
	//! Returns color of object in retVec
	void getColor(sgVec3 retVec);

  // TODO make generic versions of these variables and methods.
  // Eg. Observers::getObserverFromList, Quadrant
	/*! \brief Vector that holds all currently active moveables
	 *
	 *  One of the biggest decisions still to be made is how/if to alter this to make it less scary
	 */
	static vector<Moveable *> moveables;

	/*! \brief Returns a moveable from the main moveables list
	 *
	 *  /param moveableIndex The index of the moveable you want to retrieve from the main moveables list
	 *  \return Pointer to desired moveable
	 */
  static Moveable * getMoveableFromList( int moveableIndex );

	/*! \brief Returns a moveable from the main moveables list
	 *
	 *  /param moveableIndex The index of the moveable you want to retrieve from the main moveables list
	 *  \return Pointer to desired moveable
	 */
  static int addMoveableToList( Moveable * insertMoveable );

	/*! \brief Removes a moveable from the main moveables list
	 *
	 *  /param moveableIndex The index of the moveable you want to retrieve from the main moveables list
	 *  \return Pointer to desired moveable
	 */
  static void removeMoveableFromList( int moveableIndex );

  static void clearMoveables();

};


#endif /* MOVEABLE_H_ */


