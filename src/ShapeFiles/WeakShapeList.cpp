//
// Created by bfrasure on 8/10/18.
//

#include "WeakShapeList.h"
WeakShapeList::WeakShapeList() { }

WeakShapeList::WeakShapeList(weakVecT shapesIn)
        :shapes(shapesIn.begin(), shapesIn.end()) {
}
