#ifndef OBSERVER_H_
#define OBSERVER_H_

#include <boost/numeric/ublas/vector_sparse.hpp>
#include <GL/glut.h>
#include <plib/sg.h>

#include "../Dimensions/Moveable.h"
#include "../MaximumValues.h"

#include "../WindowDimensions.h"

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

	float fov;

	void BuildPerspProjMat(float *m, float aspect, float znear, float zfar);

	//! Stores all created Observers
	static std::vector<shared_ptr<Observer>> observers;
public:
	static shared_ptr<Observer> init(WindowDimensions windowDimensions);

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
	//! Returns true if Observer is currently set to autoscale with simulation
	bool isAutoScaling();

	static shared_ptr<Observer> getCurObserver();
	static Observer & getCurObserverRef();

	void calcMinPullback(MaximumValues maximumValues);

  //! Similar as MyShape::update(float), except only uses angular velocity right now
  void update(float);
};
#endif /* OBSERVER_H_ */
