#include "Box.h"

// Apparently I don't want any moving boxes. I should eject this from the MyShape hierarchy entirely.
Box::Box(VecStruct pos, float sideLength, VecStruct color
): MyShape(ShapeType::box, VecStruct()) {  // Empty momentum because Boxes are never moving in my cases.
	this->pos = pos;
	this->sideLength = sideLength;
	this->color = color;
}

double Box::getScale() {
	return sideLength;
}

float Box::getWidth() {
	return sideLength;
}
