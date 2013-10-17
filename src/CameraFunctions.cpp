/*
 * CameraFunctions.cpp
 *
 *  Created on: Jul 11, 2011
 *      Author: brasure
 */

#include "CameraFunctions.h"

using namespace boost::numeric::ublas;
using namespace std;


void glhPerspectivef2( matrix<double> & perspectiveMatrix, float fovyInDegrees, float aspectRatio,
						float znear, float zfar)
{
	float ymax, xmax;
	cout << perspectiveMatrix;
	ymax = znear * tanf(fovyInDegrees * M_PI/360.0);
	xmax = ymax * aspectRatio;
	glhFrustrumf2(perspectiveMatrix, -xmax, xmax, -ymax, ymax, znear, zfar);
	//cout << perspectiveMatrix;

}

void glhFrustrumf2(matrix<double> & perspectiveMatrix, float left, float right, float bottom, float top,
						float znear, float zfar)
{
	float temp, temp2, temp3, temp4;
	temp = 2.0 * znear;
	temp2 = right - left;
	temp3 = top - bottom;
	temp4 = zfar - znear;

	perspectiveMatrix(0,0) = temp / temp2;
	perspectiveMatrix(1,1) = temp / temp3;
	perspectiveMatrix(2,0) = (right+left) / temp2;
	perspectiveMatrix(2,1) = (top + bottom) / temp3;
	perspectiveMatrix(2,2) = (-zfar - znear) / temp4;
	perspectiveMatrix(2,3) = -1.0;
	perspectiveMatrix(3,2) = (-temp * zfar) / temp4;
}

void BuildPerspProjMat(float *m, float fov, float aspect,
float znear, float zfar)
{
  float xymax = znear * tan(fov * M_PI/360.0);
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

float calcMinPullback(float fov,
		float minX, float minY, float maxX, float maxY)
{
	  float absMaxX;
	  float absMaxY;
	  float pullBack;

	  if (abs(minX) > maxX)
		  absMaxX = abs(minX);
	  else
		  absMaxX = maxX;

	  if (abs(minY) > maxY)
		  absMaxY = abs(minY);
	  else
		  absMaxY = maxY;


	  if (absMaxY > absMaxX) {
		  pullBack = absMaxY / tan(fov * M_PI / 360);
	  }
	  else {
		  pullBack = absMaxX / tan(fov * M_PI / 360);
	  }

	  //cout << "pullBack Distance: " << pullBack << endl;

	  return pullBack;
}

