#include "Box.h"

Box::Box():MyShape(ShapeType::box) {
  sideLength = 1;
}

Box::Box(VecStruct pos, float sideLength, sgVec3 color
):MyShape(ShapeType::box) {
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
