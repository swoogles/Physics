#include "Box.h"

Box::Box()
{
  width = 1;
  height = 1;
  depth = 1;
}

Box::Box(sgVec4 pos, float sideLength, sgVec3 color) {
	sgCopyVec4(this->pos, pos);
	width = sideLength;
	height = sideLength;
	depth = sideLength;
	sgCopyVec3(this->color, color);
}

Box::~Box() {
}

float Box::getScale() {
	return width;
}

float Box::getWidth() {
	return width;
}

ShapeType Box::getType() {
	return ShapeType::box;
}
