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

#include <boost/numeric/ublas/vector.hpp>
#include <boost/multi_array.hpp>
#include <cassert>

//using namespace boost::numeric::ublas;
//using boost::numeric::ublas::vector;

using namespace boost;

//using namespace std;

#define INVALID_OCTREE_INDEX -1

class Quadrant : public Box
{
  // Octree<Quadrant> quadOctree(); /* Create 4096x4096x4096 octree containing doubles. */

  // string label;

  // Quadrant * nextQuadrant;
  Box * borders;
  boost::shared_ptr<Box> bordersNew;                                                                      

  sgVec4 preCenterOfMass;

  float mass;


  typedef boost::shared_ptr<Quadrant *> quadPointer;
  typedef multi_array< quadPointer, 3> array_typeNew;
  array_typeNew  quadOctreeMineNew;

  typedef multi_array<Quadrant *, 3> array_type;
  typedef array_type::index index;
  array_type  quadOctreeMine;

  bool isLeaf;
  bool containsBody;

  public:
  int level;
  // Octree<Quadrant *> * quadOctree; /* Create 4096x4096x4096 octree containing doubles. */
  Quadrant(int numCells, int level, sgVec4 pos, sgVec4 dimensions );
  void subDivide( int x, int y, int z, int numCells );
  void subDivideAll( int levels, int numCells );
  Quadrant * getQuadrantFromCell( int x, int y, int z );

  void printCorners();

  MyShape * shapeInQuadrant;
  boost::shared_ptr<MyShape> shapeInQuadrantNew;

  sgVec4 dimensions;

  //sgVec4 pos;

  void insertShape( MyShape * newShape );

  inline float getMass() { return mass; }
  inline void setMass( float mass ) { this->mass = mass; }
  inline void adjustMass ( float deltaMass ) { mass+= deltaMass; }

  void getCenterOfMass(sgVec4 centerOfMass);
  void setCenterOfMass( sgVec4 centerOfMass );

  Quadrant * determineShapeQuadrant( MyShape * shapeToInsert );

};

#endif /* QUADRANT_H_ */

