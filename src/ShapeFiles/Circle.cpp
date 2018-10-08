#include "Circle.h"

Circle::Circle(
		sgVec4 pos,
		float mass,
		float radius,
		sgVec4 momentum,
		float density,
		sgVec3 color
) {
	sgCopyVec4(this->pos, pos);
	this->mass = mass;
	this->radius = radius;
	sgCopyVec4(this->momentum, momentum);
	this->density = density;
	sgCopyVec3(this->color, color);
}

// Circle::Circle( const Circle& copyCircle )
// {
//   // super( copyCircle );
//   this->radius = copyCircle.getRadius();
// }
// 
// Circle::Circle( Circle& copyCircle )
// {
//   // super( copyCircle );
//   this->radius = copyCircle.getRadius();
// }

Circle::~Circle() {
//	cout << "Theoretically deleting this circle." << endl;
}

float Circle::getScale() {
    return radius;
}

float Circle::getRadius() {
	return radius;
}

void Circle::setRadius(float inRadius) {
	radius = inRadius;
}

ShapeType Circle::getType() {
	return ShapeType::circle;
}

float Circle::getMomentOfInertia() {
	return (2 * mass * (radius * radius))/ 5;
}
