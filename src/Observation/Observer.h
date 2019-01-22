#ifndef OBSERVER_H_
#define OBSERVER_H_

#include <GL/glut.h>
#include <plib/sg.h>

#include "../Dimensions/PhysicalVector.h"
#include "../MaximumValues.h"

#include "../WindowDimensions.h"

/*! \brief Moveable "camera" entities that can give views from different frames of reference
 *
 *  This class allows normal simulations to be observed from different positions and orientations, but was really created for eventually helping to
 *  demonstrate different aspects of relativity.
 *
 */
class Observer {
public:

	//! Creates an Observer at the origin looking in the -Z direction
	Observer(WindowDimensions windowDimensions);
	/*! \brief Does matrix operations needed to get the observers point of view
	 *
	 * Steps:
	 * \n -Loads the Observers perspective matrix
	 * \n -Translates using pos
	 * \n -Rotates by multiplying by orientationMat
	 */
	void applyView();

	//! Move observer closer to target
	void zoomIn();
	//! Move observer further from target
	void zoomOut();

	//! Set to true to autoscale with simulation
	void setAutoScaling(bool shouldScale);

	void calcMinPullback(MaximumValues maximumValues);

	//! Similar as MyShape::update(float), except only uses angular velocity right now
	void update();

	void adjustAngularVelocity(PhysicalVector dangVelocity);
	void adjustAngle(SGfloat dAngle, PhysicalVector rotAxis);
	void setPos(float inX, float inY, float inZ);
private:
	PhysicalVector pos;
//	Moveable * target; // This has never actually been doing anyhting.
	float * perspectiveMat;

	bool autoScale;

	const float fov;

	void BuildPerspProjMat(float *m, float aspect, float znear, float zfar);

    PhysicalVector angVelocity;
	sgQuat orientationQuat;
	sgMat4 orientationMat;
};
#endif /* OBSERVER_H_ */
