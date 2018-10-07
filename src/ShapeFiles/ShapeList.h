#ifndef SHAPE_LIST_H
#define SHAPE_LIST_H

#include <boost/numeric/ublas/vector_sparse.hpp>

#include "MyShape.h"

using boost::numeric::ublas::compressed_vector;

using std::size_t;

class ShapeList {
  private:
    compressed_vector<shapePointer_t> shapes;

  public:
    bool hasConflictsWith( shapePointer_t insertShape );
    size_t addShapeToList(shapePointer_t insertShape);
    size_t addList(ShapeList addList);
    int removeShapeFromList( shapePointer_t shapeToRemove );
    size_t clearShapes();
    compressed_vector<shapePointer_t> getShapes();
    void update(const float dt);
};
#endif
