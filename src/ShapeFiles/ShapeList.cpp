/*
 * ShapeList.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#include "ShapeList.h"

using namespace std;

int ShapeList::addShapeToList( shape_pointer insertShape )
{
  int curSize = shapes.size();
  shapes.resize(curSize + 1);
  shapes(curSize) = insertShape;
  return curSize;
}

int ShapeList::removeShapeFromList( shape_pointer shapeToRemove )
{
  compressed_vector<shape_pointer> newShapeVector;
  int newSize =  shapes.size();
  newShapeVector.resize(newSize);
  bool removedShape = false;


  int curIndex = 0;
  int removedIndex = -1;
  foreach_( shape_pointer curShape, shapes)
  {
    if ( curShape.get() != shapeToRemove.get() )
    {
      newShapeVector.insert_element(curIndex, curShape);
      curIndex++;
    }
    else
    {
      removedShape = true;
      newShapeVector.resize(newSize-1);
      removedIndex = curIndex;
    }
  }
  if ( removedShape )
  {
    shapes = compressed_vector<shape_pointer>( newShapeVector );
  }

  return removedIndex;
}

int ShapeList::clearShapes() {
  shapes.clear();
  shapes.resize(0);
  return shapes.size();
}

compressed_vector<shape_pointer> ShapeList::getShapes()
{
  return shapes;
}

void ShapeList::update(const float dt, bool parallelize)
{

  int numShapes = 0;
  shape_pointer curShape;
  if ( parallelize )
  {
    omp_set_num_threads(2);
    #pragma omp parallel for 
    for (unsigned int i = 0; i < MyShape::shapes.size(); i++) 
    {
      curShape = MyShape::shapes(i);
      curShape->update( dt );
    }
  }
  else
  {
    for (unsigned int i = 0; i < MyShape::shapes.size(); i++) 
    {
      curShape = MyShape::shapes(i);
      curShape->update( dt );
    }
  }
}
