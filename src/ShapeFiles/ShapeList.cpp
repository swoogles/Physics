/*
 * ShapeList.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#include "ShapeList.h"
#include "../Physics/WorldSettings.h"

#include <iostream>

using namespace std;
using namespace boost::numeric::ublas;

int ShapeList::addShapeToList( shape_pointer insertShape )
{
  int curSize = shapes.size();
  shapes.resize(curSize + 1);
  shapes(curSize) = insertShape;
  return curSize;
}

int ShapeList::removeShapeFromList( shape_pointer shapeToRemove )
{
  boost::numeric::ublas::vector<shape_pointer> newShapeVector;
  int newSize =  shapes.size();
  newShapeVector.resize(newSize);
  bool removedShape = false;


  int curIndex = 0;
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
    }
  }
  if ( removedShape )
  {
    shapes = boost::numeric::ublas::vector<shape_pointer>( newShapeVector );
  }

}

int ShapeList::clearShapes() {
  shapes.clear();
  shapes.resize(0);
  return shapes.size();
}
