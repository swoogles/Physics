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
	numPts = 16;
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
}

void Circle::drawScale(){
	glScalef(radius, radius, radius);
}

void Circle::drawUnit(){
	glutWireSphere( 1, numPts, numPts);
	//glutSolidSphere( 1, numPts, numPts);
}

float Circle::getRadius() {
	return radius;
}

void Circle::setRadius(float inRadius) {
	radius = inRadius;
}

int Circle::getType() {
	return 2;
}

float Circle::getMomentOfInertia() {
	return (2 * mass * (radius * radius))/ 5;
}
