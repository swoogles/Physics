#include <utility>

#include "ParticleList.h"

void ParticleList::ensureNoNullEntries(string caller) {
  for (const auto & curShape : this->shapes ) {
    if (curShape == nullptr) {
      fprintf(stderr, "Error: Null Shape in %s \n", caller.c_str());
      exit(1);
    }
  }

}

ParticleList::ParticleList() = default;


ParticleList::ParticleList(item_t initialShape): shapes{std::move(initialShape)} {
}

ParticleList::ParticleList(particleVector shapesIn)
:shapes(shapesIn.begin(), shapesIn.end()) {
    ensureNoNullEntries("ParticleList(particleVector)");
}

bool ParticleList::hasConflictsWith(Circle &insertShape)
{
  bool conflict = false;

  item_t object1;

  for (size_t i = 0; i < shapes.size() && conflict == false; i++)
  {
    object1 = shapes.at(i);
    conflict = object1->isTouching( insertShape  );
  }
  return conflict;
}

size_t ParticleList::addShapeToList(item_t insertShape)
{
  shapes.push_back(std::move(insertShape));
  return shapes.size();
}

size_t ParticleList::addList(ParticleList addList)
{
    particleVector & addedItems = addList.shapes;
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

size_t ParticleList::clearShapes() {
  shapes.clear();
  shapes.resize(0);
  return shapes.size();
}

particleVector ParticleList::getShapes() {
  return shapes;
}

void ParticleList::update(const float dt) {
  for (const auto & curShape : shapes ) {
      if (curShape == nullptr) {
          fprintf(stderr, "Error: Null Shape in ParticleList::update\n");
          exit(1);
      }
    curShape->update( dt );
  }
}

int ParticleList::remove(ParticleList &shapesToRemove) {
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

int ParticleList::removeShapeFromList(item_t shapeToRemove) {
    size_t newSize =  shapes.size() - 1;

    auto newIterator = std::remove_if(shapes.begin(), shapes.end(), [shapeToRemove](auto shape) {
        return shape == shapeToRemove;
    });

    shapes.resize(newSize);

    ensureNoNullEntries("removeShapeFromList");
//    shapes.erase(std::remove(shapes.begin(), shapes.end(), shapeToRemove), shapes.end());
    return 0;
}

bool ParticleList::contains(item_t searchShape) const {
    return std::any_of(shapes.begin(), shapes.end(), [searchShape](item_t curShape) {
        return searchShape == curShape;
    });
}
