#ifndef BOX_H_
#define BOX_H_

#include "MyShape.h"

class Box: public MyShape {
private:
	float sideLength;
public:
	Box();
    Box(VecStruct pos, float sideLength, sgVec3 color);

	double getScale();

    virtual float getWidth();
};

#endif
