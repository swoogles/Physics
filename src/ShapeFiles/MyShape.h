/*
 * MyShape.h
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#ifndef MYSHAPE_H_
#define MYSHAPE_H_

#include "Moveable.h"

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
//class MyShape {
class MyShape: public Moveable{
protected:
	//vector<double> pos;
	GLint numPts;
	matrix<double> pts;
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
  float kineticEnergy;

	sgVec3 color;


public:
	/*! \brief Sets default values of members common to all shapes
	 */
	MyShape();

	virtual ~MyShape();

	/*! \brief Returns points used for outline markers
	 *
	 *  This was useful when originally getting the linear algebra figured out for openGL, but has been put on the shelf for the time being
	 */
	virtual matrix<double> getPts();


	//! Print outline coordinates
	void printPts();



	//! Calculates orientationMat based on orientationQuat
	void calcRotMat();

	//! Sets kineticEnergy of object to newDensity
	bool setKineticEnergy(float newKineticEnergy);
	//! Returns kineticEnergy of object
	float getKineticEnergy();

	/*! \brief Calculates color of object based on how a star of that mass/density would burn
	 *
	 *  Right now this function only handles stars near the size of the Sun
	 */
	void calcColor();

	/*
	void hFlip();
	void vFlip();
	*/

	//! Another relic from figuring out the linear algebra, keeping around just in case
	virtual float getMarkerSize();

	//! Purely virtual method used by Circle
	virtual void setRadius(float );
	//! Purely virtual method used by Circle
	virtual float getRadius();

	/*! \brief Returns an integer indicating the type of shape
	 *
	 *  Possible values:
	 *  \n 1: Circle
	 *  \n 2: Box
	 *  \n Rest to be determined
	 */
	virtual int getType();

  // TODO make generic versions of these variables and methods.
  // Eg. Observers::getObserverFromList, Quadrant
	/*! \brief Vector that holds all currently active shapes
	 *
	 *  One of the biggest decisions still to be made is how/if to alter this to make it less scary
	 */
	static vector<MyShape *> shapes;

	/*! \brief Returns a shape from the main shapes list
	 *
	 *  /param shapeIndex The index of the shape you want to retrieve from the main shapes list
	 *  \return Pointer to desired shape
	 */
  static MyShape * getShapeFromList( int shapeIndex );

	/*! \brief Returns a shape from the main shapes list
	 *
	 *  /param shapeIndex The index of the shape you want to retrieve from the main shapes list
	 *  \return Pointer to desired shape
	 */
  static int addShapeToList( MyShape * insertShape );

	/*! \brief Removes a shape from the main shapes list
	 *
	 *  /param shapeIndex The index of the shape you want to retrieve from the main shapes list
	 *  \return Pointer to desired shape
	 */
  static void removeShapeFromList( int shapeIndex );




	//Get rid of this and store in WorldSettings instead
	static float G;

  static void clearShapes();

};


#endif /* MYSHAPE_H_ */

//******Non-Class Functions*****

