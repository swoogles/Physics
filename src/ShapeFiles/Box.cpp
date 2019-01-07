#include "Box.h"

// Apparently I don't want any moving boxes. I should eject this from the MyShape hierarchy entirely.
Box::Box(PhysicalVector pos, float sideLength, PhysicalVector color
): Moveable(ShapeType::box, PhysicalVector()), sideLength(sideLength) {  // Empty momentum because Boxes are never moving in my cases.
	this->pos = pos;
	this->_color = color;
}

double Box::scale() const {
	return sideLength;
}

float Box::getWidth() const {
	return sideLength;
}
