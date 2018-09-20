#include "Box.h"

Box::Box()
{
  sideLength = 1;
}

Box::Box(sgVec4 pos, float sideLength, sgVec3 color) {
	sgCopyVec4(this->pos, pos);
	sideLength = sideLength;
	sgCopyVec3(this->color, color);
}

Box::~Box() {
}

float Box::getScale() {
	return sideLength;
}

float Box::getWidth() {
	return sideLength;
}

ShapeType Box::getType() {
	return ShapeType::box;
}
