#ifndef MOVEABLE_H_
#define MOVEABLE_H_

#include <plib/sg.h>
#include <memory>
#include <iostream>
#include <vector>
#include "PhysicalVector.h"
#include "MatrixStruct.h"
#include <../lib/units.h>
#include <ShapeFiles/ShapeType.h>

using namespace units::mass;
using namespace units::density;
using namespace units::time;

using namespace std;

using std::unique_ptr;

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
public:
	Moveable(ShapeType shapeType, PhysicalVector momentum);
	Moveable(PhysicalVector momentum);

	void setPos(PhysicalVector newPos);

	PhysicalVector position() const; // Reinstate const-ness
	PhysicalVector vectorTo(Moveable &object2) const;

	// Parameters are turned into a quaternion then applied to orientation
	void adjustAngle(SGfloat dAngle, const PhysicalVector rotAxis);
	unique_ptr<MatrixStruct> getOrientationMat() const;
	void adjustVelocity(PhysicalVector dVel);

	PhysicalVector velocity() const;
	void update(hour_t);
	PhysicalVector weightedPosition() const;

	double distanceTo(Moveable &object2) const;

	virtual double scale() const;
	void adjustMomentum(PhysicalVector dMomentum);
	PhysicalVector momentum() const;
	void setAngularMomentum(PhysicalVector newAngMomentum);
	PhysicalVector angularMomentum() const;

	void setMass(kilogram_t newMass);
	kilogram_t mass() const;
	kilograms_per_cubic_meter_t density() const;
	PhysicalVector getColor() const;

	/*!
		 * TODO This should at *least* go to the circle class, but I think it really belongs in the Physics package
		 * \brief Calculates color of object based on how a star of that mass/density would burn
		 */
	void calcColor();

	ShapeType getType() const;
protected:
	PhysicalVector pos;
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

	virtual double momentOfInertia() const;
private:
	const ShapeType shapeType;
};
#endif
