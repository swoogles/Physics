#include "Box.h"

Box::Box(): MyShape(ShapeType::box, VecStruct()) {
  sideLength = 1;
}
// Apparently I don't want any moving boxes. I should eject this from the MyShape hierarchy entirely.
Box::Box(VecStruct pos, float sideLength, sgVec3 color
): MyShape(ShapeType::box, VecStruct()) {  // Empty momentum because Boxes are never moving in my cases.
	this->pos = pos;
	this->sideLength = sideLength;
	sgCopyVec3(this->color.vec, color);
}

double Box::getScale() {
	return sideLength;
}

float Box::getWidth() {
	return sideLength;
}
