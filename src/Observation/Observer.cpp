#include "Observer.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
using std::cout;
using std::endl;

Observer::Observer(WindowDimensions windowDimensions, AutoScaleTuning tuning)
          : pos(PhysicalVector(0,0,0))
          , autoScale( true )
          , tuning( tuning )
          , currentPullback( 0 )
          , adjustingPullback( false )
          , fov(45.0f) {
	sgMakeIdentQuat(orientationQuat);
	sgQuatToMatrix(orientationMat, orientationQuat);

	float aspect= windowDimensions.width;
	aspect /= windowDimensions.height;
	float znear=1.0f;
	float zfar=1e15f;
	perspectiveMat = new float [16];
    this->BuildPerspProjMat(perspectiveMat, aspect, znear, zfar);
}

void Observer::applyView() const {
	glLoadMatrixf(perspectiveMat);

	glTranslatef(pos.x(), pos.y(), pos.z());

	//Orientation below translate gives orbit around point
	//Orientation above translate gives pivot at point
	glMultMatrixf( (const GLfloat*)orientationMat);
}

void Observer::zoomIn() {
	pos.vec[2] *= .99;
}

void Observer::zoomOut() {
	pos.vec[2] *= 1.01;
}

void Observer::setAutoScaling(bool shouldScale) {
	autoScale = shouldScale;
}

void Observer::toggleAutoScaling() {
	autoScale = !autoScale;
	cout << "Autoscaling: " << (autoScale ? "ON" : "OFF") << endl;
}

void Observer::update() {
    PhysicalVector rotVec;
    rotVec = PhysicalVector(1,  0, 0);
    adjustAngle(angVelocity.x(), rotVec);
    rotVec = PhysicalVector(0,  1, 0);
    adjustAngle(angVelocity.y(), rotVec);
    rotVec = PhysicalVector(0,  0, 1);
    adjustAngle(angVelocity.z(), rotVec);
}

void Observer::calcMinPullback(MaximumValues maximumValues) {

    if (autoScale) {
        const float absMaxX =
                (abs(maximumValues.minX) > maximumValues.maxX)
                ? abs(maximumValues.minX)
                : maximumValues.maxX;

        const float absMaxY =
                (abs(maximumValues.minY) > maximumValues.maxY)
                ? abs(maximumValues.minY)
                : maximumValues.maxY;

        const double pullBack =
                (absMaxY > absMaxX)
                ? absMaxY / tan(this->fov * M_PI / 360)
                : absMaxX / tan(this->fov * M_PI / 360);

        const float target = (float) (pullBack * 1.4);
        if (target <= 0) {
            return;
        }

        if (currentPullback <= 0) {
            currentPullback = target;   // first frame: just frame it
        } else {
            /* Hold still until the framing is properly wrong, then close the
             * whole way. Reacting to every small difference is what makes the
             * camera stutter; a plain threshold would instead leave it forever
             * making tiny corrections at the edge of the band. */
            const float error = fabs(target / currentPullback - 1.0f);
            if (adjustingPullback) {
                if (error < tuning.settle) {
                    adjustingPullback = false;
                }
            } else if (error > tuning.deadband) {
                adjustingPullback = true;
            }

            if (adjustingPullback) {
                const float rate = (target > currentPullback)
                        ? tuning.zoomOutRate
                        : tuning.zoomInRate;
                currentPullback += (target - currentPullback) * rate;
            }
        }

        setPos(0, 0, -currentPullback);
        // Set PHYSICS_LOG_ZOOM=1 to dump the camera distance per frame, which
        // is how you tell smooth framing from stutter without eyeballing video.
        static const bool logZoom = getenv("PHYSICS_LOG_ZOOM") != nullptr;
        if (logZoom) {
            fprintf(stderr, "ZOOM %.6e\n", currentPullback);
        }
    }

}

void Observer::BuildPerspProjMat(float *m, float aspect, float znear, float zfar)
{
  float xymax = znear * tan(this->fov * M_PI/360.0);
  float ymin = -xymax;
  float xmin = -xymax;

  float width = xymax - xmin;
  float height = xymax - ymin;



  float depth = zfar - znear;
  float q = -(zfar + znear) / depth;
  float qn = -2 * (zfar * znear) / depth;

  float w = 2 * znear / width;
  w = w / aspect;
  float h = 2 * znear / height;

  m[0]  = w;
  m[1]  = 0;
  m[2]  = 0;
  m[3]  = 0;

  m[4]  = 0;
  m[5]  = h;
  m[6]  = 0;
  m[7]  = 0;

  m[8]  = 0;
  m[9]  = 0;
  m[10] = q;
  m[11] = -1;

  m[12] = 0;
  m[13] = 0;
  m[14] = qn;
  m[15] = 0;
}

void Observer::adjustAngularVelocity(PhysicalVector dAngVelocity) {
    PhysicalVector dAngV(dAngVelocity);
    this->angVelocity = angVelocity.plus(dAngV);
}

void Observer::adjustAngle(SGfloat dAngle, const PhysicalVector rotAxis) {
    sgQuat tempRotQuat;
    sgAngleAxisToQuat(tempRotQuat, dAngle,  rotAxis.vec);

    sgPostMultQuat(orientationQuat, tempRotQuat);
    sgQuatToMatrix(orientationMat, orientationQuat);
}

void Observer::adjustAngle(const PhysicalVector rotAxis) {
    sgQuat tempRotQuat;
    sgAngleAxisToQuat(tempRotQuat, 1,  rotAxis.vec);

    sgPostMultQuat(orientationQuat, tempRotQuat);
    sgQuatToMatrix(orientationMat, orientationQuat);
}

void Observer::setPos(float inX, float inY, float inZ) {
    pos.vec[0] = inX;
    pos.vec[1] = inY;
    pos.vec[2] = inZ;
}

