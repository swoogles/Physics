#include "ShapeList.h"

void ShapeList::ensureNoNullEntries(string caller) {
  for (const auto & curShape : this->shapes ) {
    if (curShape == nullptr) {
      fprintf(stderr, "Error: Null Shape in %s \n", caller.c_str());
      exit(1);
    }
  }

}

ShapeList::ShapeList() = default;


ShapeList::ShapeList(shapePointer_t initialShape): shapes{initialShape} {
}

ShapeList::ShapeList(vectorT shapesIn)
:shapes(shapesIn.begin(), shapesIn.end()) {
    ensureNoNullEntries("ShapeList(vectorT)");
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
  size_t newSize =  shapes.size() - 1;
  std::list<shapePointer_t> newShapeList;

  for( auto & curShape : shapes) {
    if ( curShape != shapeToRemove) {
//      newShapeVector.push_back(std::move(curShape));
      newShapeList.push_front(curShape);
    }
  }

  std::vector<shapePointer_t> newShapeVector{ std::make_move_iterator(std::begin(newShapeList)),
                    std::make_move_iterator(std::end(newShapeList)) };

  shapes.resize(newSize);
  shapes = vectorT ( newShapeVector );

    ensureNoNullEntries("removeShapeFromList");
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
  for (const auto & curShape : shapes ) {
      if (curShape == nullptr) {
          fprintf(stderr, "Error: Null Shape in ShapeList::update\n");
          exit(1);
      }
    curShape->update( dt );
  }
}

bool ShapeList::contains(shapePointer_t searchShape) {
  return std::any_of(shapes.begin(), shapes.end(), [searchShape](auto curShape) {
    return curShape == searchShape;
  });
}
