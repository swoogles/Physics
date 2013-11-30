/*
 * MyShape.h
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#ifndef SHAPE_LIST_H
#define SHAPE_LIST_H

#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/make_shared.hpp> 
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/foreach.hpp>
#include <iostream>

#include <plib/sg.h>

#include "MyShape.h"

//#include "../Physics/Simulations.h"


#define PI 3.14159265
#define foreach_  BOOST_FOREACH                                                                                              

using boost::shared_ptr;
using boost::numeric::ublas::compressed_vector;
typedef shared_ptr<MyShape> shape_pointer;

/*! \brief Holds the objects to be used in a Simulation
 *
 */
class ShapeList {
  private:
    compressed_vector<shape_pointer> shapes;

  public:
    int addShapeToList( shape_pointer insertShape );
    int removeShapeFromList( shape_pointer shapeToRemove );
    int clearShapes();
    compressed_vector<shape_pointer> getShapes();

};

#endif /* SHAPE_LIST_H */
