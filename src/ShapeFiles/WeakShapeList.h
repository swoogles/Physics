//
// Created by bfrasure on 8/10/18.
//

#ifndef PHYSICS_WEAKSHAPELIST_H
#define PHYSICS_WEAKSHAPELIST_H

#include "MyShape.h"

typedef typename std::vector<weak_ptr<MyShape>> weakVecT;

class WeakShapeList {
private:
    weakVecT shapes;

public:
    WeakShapeList();
    WeakShapeList(weakVecT shapesIn);

    inline size_t size() { return shapes.size(); };
};


#endif //PHYSICS_WEAKSHAPELIST_H
