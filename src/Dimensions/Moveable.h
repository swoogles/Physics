#ifndef MOVEABLE_H_
#define MOVEABLE_H_

#include <plib/sg.h>
#include <memory>
#include <iostream>
#include "PhysicalVector.h"
#include "MatrixStruct.h"
#include <../lib/units.h>

using namespace units::mass;
using namespace units::density;

using namespace std;

using std::unique_ptr;

typedef std::unique_ptr<PhysicalVector> vecPtr;

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

	PhysicalVector pos; // TODO make this a PhysicalVector
	sgQuat orientationQuat;
	sgMat4 orientationMat;

    PhysicalVector _momentum; // TODO This should go into MyShape
	PhysicalVector prevMomentum;

	PhysicalVector angMomentum;
	PhysicalVector angVelocity;
	PhysicalVector prevAngVelocity;


	kilogram_t _mass; // TODO This should go into MyShape
	kilograms_per_cubic_meter_t _density;

	PhysicalVector _color;

	virtual double momentOfInertia();
public:
	Moveable(PhysicalVector momentum);

	void setPos(float inX, float inY, float inZ);
	void setPos(PhysicalVector newPos);

	PhysicalVector position() ; // Reinstate const-ness
	PhysicalVector vectorTo(Moveable &object2);

	 // Parameters are turned into a quaternion then applied to orientation
	void adjustAngle(SGfloat dAngle, const PhysicalVector rotAxis);
	unique_ptr<MatrixStruct> getOrientationMat() const;
	void adjustVelocity(PhysicalVector dVel);

	PhysicalVector velocity();
	void setAngularVelocity(PhysicalVector newAngVelocity);
	void adjustAngularVelocity(PhysicalVector dangVelocity);
	void update(float);
};
#endif
