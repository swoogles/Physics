/*
 * Moveable.h
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#ifndef MOVEABLE_H_
#define MOVEABLE_H_

#include <boost/lexical_cast.hpp> 
#include <boost/numeric/ublas/matrix.hpp>
#include <cmath>
#include <iostream>
#include <plib/sg.h>

using namespace boost::numeric::ublas;
using boost::numeric::ublas::compressed_vector;
using boost::lexical_cast;

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

	sgVec4 pos;
	sgQuat orientationQuat;
	sgMat4 orientationMat;

	sgVec4 angMomentum;
	sgVec4 angVelocity;
	sgVec4 prevAngVelocity;

	sgVec3 angVel;

	sgVec4 momentum;
	sgVec4 prevMomentum;
	float mass;
	float density;

	sgVec3 color;

	bool ptsHighlighted;

public:
	/*! \brief Sets default values of members common to all shapes
	 */
	Moveable();
	virtual ~Moveable();

	//! Set position of object to <inX, inY, inZ>
	void setPos(float inX, float inY, float inZ);
	//! Set position of object to <newPos>
	void setPos(sgVec4 newPos);

	//! Alter position of object by <dx, dy, dz>
	void adjustPos(float dx, float dy, float dz);
	//! Alter position of object by <dPos>
	void adjustPos(sgVec4 dPos);

	//! Return position of object in retVec
	void getPos(sgVec4 retVec);

	//! Print position of object. Format: <##,##,##>
  std::string getPosString();


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

	//! Sets momentum of object to <inX, inY, inZ> * mass
	void setVelocity(float inX, float inY, float inZ);
	//! Sets momentum of object to <newVel> * mass
	void setVelocity(sgVec4 newVel);

	//! Alters momentum of object by <dx, dy, dy> * mass
	void adjustVelocity(float dx, float dy, float dz);
	//! Alters momentum of object by <dVel> * mass
	void adjustVelocity(sgVec4 dVel);

	//! Returns velocity of object in retVec
	void getVelocity(sgVec4 retVec);

	/*! \brief Calculates the moment of inertia for the object
	 *
	 *  Calculation varies for different types of object
	 *  \return Moment of Inertia value
	 */
	virtual float getMomentOfInertia();

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

};
#endif /* MOVEABLE_H_ */
