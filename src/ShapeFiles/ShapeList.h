#ifndef SHAPE_LIST_H
#define SHAPE_LIST_H

#include "MyShape.h"
#include <list>
#include <algorithm>

using std::size_t;

typedef typename std::vector<shapePointer_t> vectorT;

class ShapeList {
  private:
    vectorT shapes;
    void ensureNoNullEntries(string caller);

  public:
    ShapeList();
    ShapeList(shapePointer_t initialShape);
    ShapeList(vectorT shapesIn);

    size_t addShapeToList(shapePointer_t insertShape);
    size_t addList(ShapeList addList);

    int removeShapeFromList(shared_ptr<MyShape> shapeToRemove);
    int remove(ShapeList & shapesToRemove);
    size_t clearShapes();
    vectorT getShapes();
    void update(const float dt);
    inline size_t size() { return shapes.size(); };

    bool contains(shared_ptr<MyShape> searchShape) const;
};
#endif
