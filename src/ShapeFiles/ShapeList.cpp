#include "ShapeList.h"

ShapeList::ShapeList() { }

ShapeList::ShapeList(vectorT shapesIn)
:shapes(shapesIn.begin(), shapesIn.end()) {
  cout << "in counstructor" << endl;
  std::for_each(this->shapes.begin(), this->shapes.end(), [] (shapePointer_t shape) { cout << "shape: " << shape << endl; });
}

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
  shapes.push_back(std::move(insertShape));
  return shapes.size();
}

size_t ShapeList::addList(ShapeList addList)
{
  vectorT addShapes = addList.getShapes();
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
    cout << "removeShapeFromList invocation" << endl;
  vectorT newShapeVector;
  size_t newSize =  shapes.size();
  newShapeVector.resize(newSize);
  bool removedShape = false;


  size_t curIndex = 0;
  for( auto & curShape : shapes) {
    if ( curShape != shapeToRemove) {
      newShapeVector.push_back(std::move(curShape));
      curIndex++;
    }
    else {
      removedShape = true;
      newShapeVector.resize(newSize-1);
    }
  }
  cout << "A" << endl;
  if ( removedShape ) {
    shapes = vectorT ( newShapeVector );
  }
  cout << "B" << endl;

//    shapes.erase(std::remove(shapes.begin(), shapes.end(), shapeToRemove), shapes.end());
    return 0;
}

size_t ShapeList::clearShapes() {
  shapes.clear();
  shapes.resize(0);
  return shapes.size();
}

vectorT ShapeList::getShapes() {
  return shapes;
}

void ShapeList::update(const float dt) {
    cout << "Shapelist.update numShapes: " << shapes.size() << endl;
  for (const auto & curShape : shapes ) {
      if (curShape == nullptr) {
          fprintf(stderr, "Error: Null Shape in ShapeList::update\n");
          exit(1);
      }
    curShape->update( dt );
  }
  cout << "Shapelist.update finished update " << endl;
}

bool ShapeList::contains(shapePointer_t searchShape) {
  return std::any_of(shapes.begin(), shapes.end(), [searchShape](auto curShape) {
    return curShape == searchShape;
  });
}
