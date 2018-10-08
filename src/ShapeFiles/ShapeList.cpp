#include "ShapeList.h"

bool ShapeList::hasConflictsWith( shapePointer_t insertShape )
{
  bool conflict = false;

  shapePointer_t object1;

  for (size_t i = 0; i < shapes.size() && conflict == false; i++)
  {
    object1 = shapes.at(i);
    conflict = object1->isTouching( insertShape  );
  }
  return conflict;
}

size_t ShapeList::addShapeToList(shapePointer_t insertShape)
{
  shapes.push_back(insertShape);
  return shapes.size();
}

size_t ShapeList::addList(ShapeList addList)
{
  std::vector<shapePointer_t> addShapes = addList.getShapes();
  size_t additionSize = addShapes.size();
  size_t curSize = shapes.size();
  shapes.resize( curSize + additionSize );
  for( const auto & curShape : addShapes ) {
    this->addShapeToList( curShape );
  }

  addList.clearShapes();

  return shapes.size();
}

int ShapeList::removeShapeFromList( shapePointer_t shapeToRemove )
{
  std::vector<shapePointer_t> newShapeVector;
  size_t newSize =  shapes.size();
  newShapeVector.resize(newSize);
  bool removedShape = false;


  size_t curIndex = 0;
  int removedIndex = -1;
  for( const auto & curShape : shapes) {
    if ( curShape.get() != shapeToRemove.get() ) {
      newShapeVector.push_back(curShape);
      curIndex++;
    }
    else {
      removedShape = true;
      newShapeVector.resize(newSize-1);
      removedIndex = curIndex;
    }
  }
  if ( removedShape ) {
    shapes = std::vector<shapePointer_t>( newShapeVector );
  }

  return removedIndex;
}

size_t ShapeList::clearShapes() {
  shapes.clear();
  shapes.resize(0);
  return shapes.size();
}

std::vector<shapePointer_t> ShapeList::getShapes() {
  return shapes;
}

void ShapeList::update(const float dt) {
  for (const auto & curShape : shapes ) {
    curShape->update( dt );
  }
}
