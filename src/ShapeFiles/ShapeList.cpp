#include "ShapeList.h"

bool ShapeList::hasConflictsWith( shapePointer_t insertShape )
{
  bool conflict = false;

  shapePointer_t object1;

  for (int i = 0; i < shapes.size() && conflict == false; i++) 
  {
    object1 = shapes(i);
    conflict = object1->isTouching( insertShape  );
  }
  return conflict;
}

int ShapeList::addShapeToList( shapePointer_t insertShape )
{
  int curSize = shapes.size();
  shapes.resize(curSize + 1);
  shapes(curSize) = insertShape;
  return curSize;
}

int ShapeList::addList( ShapeList addList )
{
  compressed_vector<shapePointer_t> addShapes = addList.getShapes();
  int additionSize = addShapes.size();
  int curSize = shapes.size();
  shapes.resize( curSize + additionSize );
  foreach_( shapePointer_t curShape, addShapes )
  {
    this->addShapeToList( curShape );
  }

  addList.clearShapes();

  return shapes.size();
}

int ShapeList::removeShapeFromList( shapePointer_t shapeToRemove )
{
  compressed_vector<shapePointer_t> newShapeVector;
  int newSize =  shapes.size();
  newShapeVector.resize(newSize);
  bool removedShape = false;


  int curIndex = 0;
  int removedIndex = -1;
  foreach_( shapePointer_t curShape, shapes)
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
    shapes = compressed_vector<shapePointer_t>( newShapeVector );
  }

  return removedIndex;
}

int ShapeList::clearShapes() {
  shapes.clear();
  shapes.resize(0);
  return shapes.size();
}

compressed_vector<shapePointer_t> ShapeList::getShapes()
{
  return shapes;
}

void ShapeList::update(const float dt)
{
  foreach_ ( shapePointer_t curShape, shapes )
  {
    curShape->update( dt );
  }
}
