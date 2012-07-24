/*
 * CameraFunctions.h
 *
 *  Created on: Jul 11, 2011
 *      Author: brasure
 */

#ifndef CAMERAFUNCTIONS_H_
#define CAMERAFUNCTIONS_H_

#include <cmath>
#include <boost/numeric/ublas/matrix.hpp>
#include <iostream>
#include <boost/numeric/ublas/io.hpp>


using namespace boost::numeric::ublas;

void glhPerspectivef2(matrix<double> & matrix, float fovInDegrees, float aspectRatio,
						float znear, float zfar);

void glhFrustrumf2(matrix<double> & matrix, float left, float right, float bottom, float top,
						float zear, float zfar);

void BuildPerspProjMat(float *m, float fov, float aspect, float znear, float zfar);

float calcMinPullback(float fov,
		float minX, float minY, float maxX, float maxY);

#endif /* CAMERAFUNCTIONS_H_ */
