/*
 * Quadrant.h
*  Author: brasure
 */

#ifndef QUADRANT_H_
#define QUADRANT_H_

#include <iostream>
#include <string>
#include <plib/sg.h>

#include "../ShapeFiles/MyShape.h"
#include "../ShapeFiles/Box.h"
#include "../ShapeFiles/Circle.h"
#include "../ShapeFiles/ShapeList.h"
// #include "../Physics/Interactions.h"

#include <boost/numeric/ublas/vector.hpp>
#include <boost/multi_array.hpp>
#include <boost/current_function.hpp>
#include <boost/ref.hpp>
#include <boost/foreach.hpp>
#include <cassert>

//using namespace boost::numeric::ublas;
//using boost::numeric::ublas::vector;

using namespace boost;

//using namespace std;

#define INVALID_OCTREE_INDEX -1
#define foreach_  BOOST_FOREACH                                                                                              

class Quadrant : public Box
{
  // Octree<Quadrant> quadOctree(); /* Create 4096x4096x4096 octree containing doubles. */

  // string label;

  // Quadrant * nextQuadrant;
  boost::shared_ptr<Box> borders;

  boost::shared_ptr<Circle> centerOfMassRepresentation;

  // sgVec4 preCenterOfMass;
  sgVec4 weightedPosition;

  float mass;

  typedef boost::shared_ptr<MyShape> shape_pointer;

  typedef boost::shared_ptr<Quadrant> quad_pointer;
  typedef multi_array< quad_pointer, 3> array_typeNew;
  array_typeNew  quadOctree;

  typedef multi_array<Quadrant *, 3> array_type;
  typedef array_type::index index;
  array_type  quadOctreeMine;

  bool isLeaf;
  bool containsBody;

  public:
  int level;
  // Octree<Quadrant *> * quadOctree; /* Create 4096x4096x4096 octree containing doubles. */
  Quadrant(int numCells, int level, sgVec4 pos, sgVec4 dimensions );
  ~Quadrant();
  void subDivide( int x, int y, int z, int numCells );
  void subDivideAll( int levels, int numCells );
  quad_pointer getQuadrantFromCell( int x, int y, int z );

  void printCorners();

  shape_pointer shapeInQuadrant;
  boost::shared_ptr<MyShape> shapeInQuadrantNew;

  sgVec4 dimensions;

  //sgVec4 pos;

  void insertShape( shape_pointer newShape );

  inline float getMass() { return mass; }
  inline void setMass( float mass ) { this->mass = mass; }
  inline void adjustMass ( float deltaMass ) { mass+= deltaMass; }

  boost::shared_ptr<MyShape> getShapeInQuadrant();

  void getWeightedPosition(sgVec4 weightedPosition);
  void setWeightedPosition(sgVec4 weightedPosition);

  void getCenterOfMass(sgVec4 centerOfMass);
  void setCenterOfMass( sgVec4 centerOfMass );

  quad_pointer determineShapeQuadrant( shape_pointer shapeToInsert );

	void getPos(sgVec4 retVec);

  void calcForceOnShape( shape_pointer curShape, sgVec3 netForceFromQuadrant, float dt );

  ShapeList getShapesRecursive( int curLevel );

};

#endif /* QUADRANT_H_ */

