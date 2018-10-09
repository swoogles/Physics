#ifndef SHAPE_LIST_H
#define SHAPE_LIST_H

#include "MyShape.h"

using std::size_t;

typedef typename std::vector<shapePointer_t> vectorT;

class ShapeList {
  private:
    vectorT shapes;

  public:
    ShapeList();
    ShapeList(vectorT shapesIn);
    bool hasConflictsWith( shapePointer_t insertShape );
    size_t addShapeToList(shapePointer_t insertShape);
    size_t addList(ShapeList addList);
    int removeShapeFromList( shapePointer_t shapeToRemove );
    size_t clearShapes();
    vectorT getShapes();
    void update(const float dt);
    inline size_t size() { return shapes.size(); };
    bool contains(shapePointer_t searchShape);
};
#endif
