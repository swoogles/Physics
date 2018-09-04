/*
 * Observer.h
 *
 *  Created on: Jul 20, 2011
 *      Author: brasure
 */

#ifndef OBSERVER_H_
#define OBSERVER_H_

#include <boost/numeric/ublas/vector_sparse.hpp>
#include <GL/glut.h>
#include <iostream>
#include <plib/sg.h>

#include "../ShapeFiles/Moveable.h"

using boost::numeric::ublas::compressed_vector;

/*! \brief Moveable "camera" entities that can give views from different frames of reference
 *
 *  This class allows normal simulations to be observed from different positions and orientations, but was really created for eventually helping to
 *  demonstrate different aspects of relativity.
 *
 */
class Observer:public Moveable {
private:
	Moveable * target;
	float * perspectiveMat;

	int id;
	bool autoScale;
	static int curObserver;

public:
	static void init();

	//! Creates an Observer at the origin looking in the -Z direction
	Observer();
	/*! \brief Does matrix operations needed to get the observers point of view
	 *
	 * Steps:
	 * \n -Loads the Observers perspective matrix
	 * \n -Translates using pos
	 * \n -Rotates by multiplying by orientationMat
	 */
	void getView();

	//! Move observer closer to target
	void zoomIn();
	//! Move observer further from target
	void zoomOut();

	//! Set to true to autoscale with simulation
	void setAutoScaling(bool shouldScale);
	//! Returns true if Observer is currently set to autoscale with simulation
	bool isAutoScaling();

	//! Change ID number of observer to newId
	void setId(int newId);
	//! Returns ID number of observer
	int getId();

	//! Switches active Observer to newLooker
	static void setCurObserver(int newLooker);
	//! Returns an integer indicating the active Observer
	static Observer * getCurObserver();

	//! Stores all created Observers
	static compressed_vector<Observer *> observers;

  void calcMinPullback(float fov, float minX, float minY, float maxX, float maxY);

  //! Similar as MyShape::update(float), except only uses angular velocity right now
  void update(float);

  static void BuildPerspProjMat(float *m, float fov, float aspect, float znear, float zfar);
};
#endif /* OBSERVER_H_ */
