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
/*! \brief How eagerly the auto-scaling camera chases the simulation's bounds.
 *
 *  The bounds are recomputed from scratch every frame from the extrema of a
 *  set whose membership keeps changing, so following them directly makes the
 *  camera stutter. These four numbers turn that raw target into occasional,
 *  smooth moves.
 */
struct AutoScaleTuning {
	//! Fraction of the remaining distance covered per frame when pulling back.
	float zoomOutRate = 0.06f;

	//! The same when moving in. Deliberately slower: losing the action off the
	//! edges of frame is far worse than being a little too far away, and a
	//! collapse shouldn't drag the camera in behind it.
	float zoomInRate = 0.012f;

	//! Start adjusting only once the target differs by more than this.
	float deadband = 0.18f;

	//! Once adjusting, keep going until within this of the target.
	float settle = 0.04f;
};

class Observer {
public:

	//! Creates an Observer at the origin looking in the -Z direction
	Observer(WindowDimensions windowDimensions, AutoScaleTuning tuning = AutoScaleTuning());
	/*! \brief Does matrix operations needed to get the observers point of view
	 *
	 * Steps:
	 * \n -Loads the Observers perspective matrix
	 * \n -Translates using pos
	 * \n -Rotates by multiplying by orientationMat
	 */
	void applyView() const;

	//! Move observer closer to target
	void zoomIn();
	//! Move observer further from target
	void zoomOut();

	//! Set to true to autoscale with simulation
	void setAutoScaling(bool shouldScale);
	void toggleAutoScaling();

	void calcMinPullback(MaximumValues maximumValues);

	//! Similar as MyShape::update(float), except only uses angular velocity right now
	void update();

	void adjustAngularVelocity(PhysicalVector dangVelocity);
	void adjustAngle(SGfloat dAngle, PhysicalVector rotAxis);
    void adjustAngle(PhysicalVector rotAxis);
	void setPos(float inX, float inY, float inZ);
private:
	PhysicalVector pos;
//	Moveable * target; // This has never actually been doing anyhting.
	float * perspectiveMat;

	bool autoScale;

	const AutoScaleTuning tuning;

	//! Where the camera actually is, as opposed to where the bounds want it.
	float currentPullback;

	//! True while closing on a new target; see AutoScaleTuning.
	bool adjustingPullback;

	const float fov;

	void BuildPerspProjMat(float *m, float aspect, float znear, float zfar);

    PhysicalVector angVelocity;
	sgQuat orientationQuat;
	sgMat4 orientationMat;
};
#endif /* OBSERVER_H_ */
