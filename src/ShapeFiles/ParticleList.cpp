#include "ParticleList.h"
#include "PairCollection.h" // TODO Ugh. No good.

void ParticleList::ensureNoNullEntries(string caller) {
  for (const auto & curShape : this->shapes ) {
    if (curShape == nullptr) {
      fprintf(stderr, "Error: Null Shape in %s \n", caller.c_str());
      exit(1);
    }
  }

}

ParticleList::ParticleList() = default;


ParticleList::ParticleList(shared_ptr<Particle> initialShape): shapes{std::move(initialShape)} {
}

ParticleList::ParticleList(std::vector<shared_ptr<Particle>> shapesIn)
:shapes(shapesIn.begin(), shapesIn.end()) {
    ensureNoNullEntries("ParticleList(particleVector)");
}

bool ParticleList::hasConflictsWith(Particle &insertShape)
{
  bool conflict = false;

  shared_ptr<Particle> object1;

  for (size_t i = 0; i < shapes.size() && conflict == false; i++)
  {
    object1 = shapes.at(i);
    conflict = object1->isTouching( insertShape  );
  }
  return conflict;
}

size_t ParticleList::addShapeToList(shared_ptr<Particle> insertShape)
{
  shapes.push_back(std::move(insertShape));
  return shapes.size();
}

size_t ParticleList::addList(ParticleList addList)
{
    std::vector<shared_ptr<Particle>> & addedItems = addList.shapes;
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

void ParticleList::update(hour_t dt) {
    PairCollection deleteList;
  for (const auto & curShape : shapes ) {
      if (curShape == nullptr) {
          fprintf(stderr, "Error: Null Shape in ParticleList::update\n");
          exit(1);
      }
    curShape->update( dt );
      for (const auto & innerShape : shapes) {
          if (curShape != innerShape) {
              if (curShape->isTouching(*innerShape)) {
                  TouchingPair touchingPair(curShape, innerShape);
                  deleteList.insertIfUnique(touchingPair);
              }
          }
      }
  }

    deleteList.mergePairs();
    ParticleList doomedList(deleteList.doomed() );
    if (doomedList.size() > 0) {
        cout << "removing this many particles: " << doomedList.size() << endl;
    }
    ParticleList::remove(doomedList);
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

int ParticleList::removeShapeFromList(shared_ptr<Particle> shapeToRemove) {
    size_t newSize =  shapes.size() - 1;

    auto newIterator = std::remove_if(shapes.begin(), shapes.end(), [shapeToRemove](auto shape) {
        return shape == shapeToRemove;
    });

    shapes.resize(newSize);

    ensureNoNullEntries("removeShapeFromList");
//    shapes.erase(std::remove(shapes.begin(), shapes.end(), shapeToRemove), shapes.end());
    return 0;
}

bool ParticleList::contains(shared_ptr<Particle> searchShape) const {
    return std::any_of(shapes.begin(), shapes.end(), [searchShape](shared_ptr<Particle> curShape) {
        return searchShape == curShape;
    });
}

ostream &operator<<(ostream &os, const ParticleList &particleList) {
    os << "[";
    for (auto i : particleList.shapes)
        os  << i << ' ';
    os << "]";
    return os;
}

void ParticleList::applyToAllParticles(
        function<void (Particle &)> functor ) {
    for ( const auto & curShape : shapes ) {
        functor(*curShape);
    }

}

void ParticleList::checkForAllParticles(
        function<void (const Particle &)> functor ) const {
    for ( const auto & curShape : shapes ) {
        functor(*curShape);
    }
}
