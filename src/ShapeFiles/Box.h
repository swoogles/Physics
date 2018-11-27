#ifndef BOX_H_
#define BOX_H_

#include "MyShape.h"

class Box: public MyShape {
private:
	float sideLength;
public:
	Box(VecStruct pos, float sideLength, sgVec3 color);

	double getScale() override;

    virtual float getWidth();
};

#endif
