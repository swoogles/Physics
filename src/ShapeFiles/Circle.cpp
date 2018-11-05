#include "Circle.h"

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

Circle::Circle(
		sgVec4 pos,
		float mass,
		float radius,
		sgVec4 momentum,
		sgVec3 color
		) {

	sgCopyVec4(this->pos, pos);
	this->mass = mass;
	this->radius = radius;
	sgCopyVec4(this->momentum, momentum);
//	ρ=(3m)/(4πr^2)
	this->density = (3*mass) / (4 * M_PI * (radius * radius));
	sgCopyVec3(this->color, color);
}

Circle::Circle(
		sgVec4 pos,
		float mass,
		sgVec4 momentum,
		float density,
		sgVec3 color
		) {
	sgCopyVec4(this->pos, pos);
	this->mass = mass;
	// TODO calculate this
//	this->radius = radius;
	 this->radius =   sqrt(((3*mass) / 4 * M_PI ) / density);

	sgCopyVec4(this->momentum, momentum);
	this->density = density;
	sgCopyVec3(this->color, color);

}

float Circle::calcRadius(float mass, float density) {
	return sqrt(((3*mass) / 4 * M_PI ) / density);
}

/*
float calcRadius(float mass, int density ) {
	float pieceRadius = pieceVol/(M_PI * 4.0/3.0);
	pieceRadius = pow(pieceRadius, 1.0/3.0);
	return pieceRadius;
}
 */

