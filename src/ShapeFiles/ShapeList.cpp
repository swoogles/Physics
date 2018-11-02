#include <utility>

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


ShapeList::ShapeList(shapePointer_t initialShape): shapes{std::move(initialShape)} {
}

ShapeList::ShapeList(vectorT shapesIn)
:shapes(shapesIn.begin(), shapesIn.end()) {
    ensureNoNullEntries("ShapeList(vectorT)");
}

bool ShapeList::hasConflictsWith(MyShape &insertShape)
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
    vectorT & addedItems = addList.shapes;
    if (shapes.empty())
    {
        cout << "XXX empty case" << endl;
        shapes = std::move(addedItems);
    }
    else
    {
        cout << "XXX existing items case" << endl;
        shapes.reserve(shapes.size() + addedItems.size());
        std::move(std::begin(addedItems), std::end(addedItems), std::back_inserter(shapes));
        // TODO see if I can re-add at some point
        // addedItems.clear();
    }
    return shapes.size();
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

int ShapeList::remove(ShapeList &shapesToRemove) {
    size_t newSize =  shapes.size() - shapesToRemove.size();

    auto newIterator = std::remove_if(shapes.begin(), shapes.end(), [shapesToRemove](auto shape) {
        // THIS CAN'T WORK UNTIL CONTAINS IS FIXED
        return shapesToRemove.contains(shape);
//        std::any_of(shapesToRemove.shapes.begin(), shapesToRemove.shapes.begin())
//        return &(curShapeRef) == &shapeToRemove;
    });

    shapes.resize(newSize);

    ensureNoNullEntries("removeShapeFromList");
//    shapes.erase(std::remove(shapes.begin(), shapes.end(), shapeToRemove), shapes.end());
    return 0;

}

int ShapeList::removeShapeFromList(shared_ptr<MyShape> shapeToRemove) {
    size_t newSize =  shapes.size() - 1;

    auto newIterator = std::remove_if(shapes.begin(), shapes.end(), [shapeToRemove](auto shape) {
        return shape == shapeToRemove;
    });

    shapes.resize(newSize);

    ensureNoNullEntries("removeShapeFromList");
//    shapes.erase(std::remove(shapes.begin(), shapes.end(), shapeToRemove), shapes.end());
    return 0;
}

bool ShapeList::contains(shared_ptr<MyShape> searchShape) const {
    return std::any_of(shapes.begin(), shapes.end(), [searchShape](shapePointer_t curShape) {
        return searchShape == curShape;
    });
}
