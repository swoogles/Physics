/*
 * ShapeList.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#include "ShapeList.h"

using namespace std;

bool ShapeList::hasConflictsWith( shape_pointer insertShape )
{
  sgVec4 sepVec;
  SGfloat distanceSquared, distance, minSep;
  bool conflict = false;

  boost::shared_ptr<MyShape> object1;

  for (int i = 0; i < shapes.size() && conflict == false; i++) 
  {
    object1 = shapes(i);
    conflict = object1->isTouching( insertShape  );
  }
  return conflict;
}

int ShapeList::addShapeToList( shape_pointer insertShape )
{
  int curSize = shapes.size();
  shapes.resize(curSize + 1);
  shapes(curSize) = insertShape;
  return curSize;
}

int ShapeList::addList( ShapeList addList )
{
  compressed_vector<shape_pointer> addShapes = addList.getShapes();
  int additionSize = addShapes.size();
  int curSize = shapes.size();
  shapes.resize( curSize + additionSize );
  foreach_( shape_pointer curShape, addShapes )
  {
    this->addShapeToList( curShape );
  }

  addList.clearShapes();

  return shapes.size();
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

void ShapeList::update(const float dt)
{
  foreach_ ( shape_pointer curShape, shapes )
  {
    curShape->update( dt );
  }
}
