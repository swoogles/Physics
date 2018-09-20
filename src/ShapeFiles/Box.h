#ifndef BOX_H_
#define BOX_H_

#include "MyShape.h"

class Box: public MyShape {
private:
	float sideLength;
public:
	Box();
    Box(sgVec4 pos, float sideLength, sgVec3 color);
	virtual ~Box();

    float getScale();

	float getWidth();

	ShapeType getType();
};

#endif
