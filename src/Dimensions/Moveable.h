#ifndef MOVEABLE_H_
#define MOVEABLE_H_

#include <plib/sg.h>
#include <memory>
#include <iostream>
#include "VecStruct.h"
#include "MatrixStruct.h"

using namespace std;

using std::unique_ptr;

typedef std::unique_ptr<VecStruct> vecPtr;

/*! \brief The class at the heart of all the physical calculations
 *
 *  This class holds all the attributes needed to non-physical, abstract objects:
 *
 *  \n -Mass TODO this should move out of here.
 *  \n -Density TODO this should move out of here.
 *  \n -Position
 *  \n -Orientation
 *  \n -Momentum (Normal and Angular)
 *  \n -Color TODO this should move out of here.
 */
class Moveable {
protected:

	VecStruct pos; // TODO make this a VecStruct
	sgQuat orientationQuat;
	sgMat4 orientationMat;

    VecStruct momentum; // TODO This gots'ta go.
	VecStruct prevMomentum;

	sgVec4 angMomentum;
	sgVec4 angVelocity;
	sgVec4 prevAngVelocity;

	float mass; // TODO This gots'ta go.
	float density;

	sgVec3 color;

	/*! \brief Calculates the moment of inertia for the object
	 *
	 *  Calculation varies for different types of object
	 *  \return Moment of Inertia value
	 */
	virtual float getMomentOfInertia();

public:
	/*! \brief Sets default values of members common to all shapes
	 */
	Moveable();

	//! Set position of object to <inX, inY, inZ>
	void setPos(float inX, float inY, float inZ);
	//! Set position of object to <newPos>
	void setPos(const sgVec4 newPos);

	VecStruct getPos() ; // Reinstate const-ness

	VecStruct getVectorToObject(Moveable &object2);

	/*! \brief Rotate object around a specified axis
	 *
	 *  Parameters are turned into a quaternion then applied to orientationQuat using sgPostMultQuat()
	 *  \param dAngle Amount to be rotated
	 *  \param rotAxis Axis to rotate around
	 */
	void adjustAngle(SGfloat dAngle, const sgVec3 rotAxis);

	unique_ptr<MatrixStruct> getOrientationMat() const;


	//! Alters momentum of object by <dVel> * mass
	void adjustVelocity(VecStruct dVel);

	VecStruct getVelocity();

	//! Sets angular velocity of object to <newAngVelocity>
	void setAngVelocity(const sgVec4 newAngVelocity);
	//! Alters angular velocity of object by <dAngVelocity>
	void adjustAngVelocity(const sgVec4 dangVelocity);

	//! Moves object based on current normal and angular momentum
	void update(float);
};
#endif
