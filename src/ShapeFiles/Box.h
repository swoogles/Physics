#ifndef BOX_H_
#define BOX_H_

#include "MyShape.h"

class Box: public MyShape {
private:
	float depth;
	float width;
	float height;
public:
	Box();
    Box(sgVec4 pos, float sideLength, sgVec3 color);
	virtual ~Box();

	void drawUnit();
	void drawScale();

	float getWidth();

	int getType();
};

#endif
