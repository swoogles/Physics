#include "ParticleList.h"
#include "PairCollection.h" // TODO Ugh. No good.
#include "Particle.h"
#include <omp.h>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <algorithm>

void ParticleList::ensureNoNullEntries(string caller) {
  for (const auto & curShape : this->shapes ) {
    if (curShape == nullptr) {
      fprintf(stderr, "Error: Null Shape in %s \n", caller.c_str());
      exit(1);
    }
  }

}

ParticleList::ParticleList() = default;


ParticleList::ParticleList(std::vector<shared_ptr<Particle>> shapesIn)
    :shapes(shapesIn.begin(), shapesIn.end()
) {
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
    // Simple position/velocity update - no collision detection here
    for (const auto & curShape : shapes ) {
        if (curShape == nullptr) {
            fprintf(stderr, "Error: Null Shape in ParticleList::update\n");
            exit(1);
        }
        curShape->update( dt );
    }
}

void ParticleList::updateWithCollisions(hour_t dt, PairCollection& collisionPairs) {
    // Update all particle positions
    for (const auto & curShape : shapes ) {
        if (curShape == nullptr) {
            fprintf(stderr, "Error: Null Shape in ParticleList::updateWithCollisions\n");
            exit(1);
        }
        curShape->update( dt );
    }

    // Process pre-collected collision pairs (collected during octree traversal)
    // No O(n²) search needed - pairs were found during O(n log n) force calculation
    ParticleList doomedList(collisionPairs.doomed());

    if (doomedList.size() > 0) {
        cout << "removing this many particles: " << doomedList.size() << endl;
    }
    ParticleList::remove(doomedList);
}

int ParticleList::remove(ParticleList &shapesToRemove) {
    static int stepCount = 0;
    stepCount++;

    auto now = chrono::system_clock::now();
    auto time_t_now = chrono::system_clock::to_time_t(now);
    auto ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()).count() % 1000;

    cout << "[" << std::put_time(std::localtime(&time_t_now), "%H:%M:%S")
         << "." << setfill('0') << setw(3) << ms << "] "
         << "step=" << stepCount
         << " shapes=" << shapes.size()
         << " removing=" << shapesToRemove.size() << endl;
    cout.flush();

    size_t newSize =  shapes.size() - shapesToRemove.size();

    auto newIterator = std::remove_if(shapes.begin(), shapes.end(), [shapesToRemove](auto shape) {
        return shapesToRemove.contains(shape);
    });

    shapes.resize(newSize);

    ensureNoNullEntries("removeShapeFromList");
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

void ParticleList::applyToAllParticlesParallel(
        function<void (Particle &)> functor ) {
    const size_t n = shapes.size();
    #pragma omp parallel for schedule(dynamic, 64)
    for (size_t i = 0; i < n; i++) {
        functor(*shapes[i]);
    }
}

void ParticleList::applyToAllParticlesParallelWithPtr(
        function<void (shared_ptr<Particle>)> functor ) {
    const size_t n = shapes.size();
    #pragma omp parallel for schedule(dynamic, 64)
    for (size_t i = 0; i < n; i++) {
        functor(shapes[i]);
    }
}

void ParticleList::forEachWithPtr(
        function<void (shared_ptr<Particle>)> functor ) const {
    for (const auto & shape : shapes) {
        functor(shape);
    }
}

void ParticleList::checkForAllParticles(
        function<void (const Particle &)> functor ) const {
    for ( const auto & curShape : shapes ) {
        functor(*curShape);
    }
}

