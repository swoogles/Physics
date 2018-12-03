#include "Box.h"

// Apparently I don't want any moving boxes. I should eject this from the MyShape hierarchy entirely.
Box::Box(PhysicalVector pos, float sideLength, PhysicalVector color
): Moveable(ShapeType::box, PhysicalVector()) {  // Empty momentum because Boxes are never moving in my cases.
	this->pos = pos;
	this->sideLength = sideLength;
	this->_color = color;
}

double Box::scale() {
	return sideLength;
}

float Box::getWidth() {
	return sideLength;
}
