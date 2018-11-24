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
    return radius.value();
}

meter_t Circle::getRadius() {
	return radius;
}

void Circle::setRadius(float inRadius) {
	radius = meter_t(inRadius);
}

ShapeType Circle::getType() {
	return ShapeType::circle;
}

float Circle::getMomentOfInertia() {
	return (2 * mass.value() * (radius.value() * radius.value()))/ 5;
}

Circle::Circle(
		sgVec4 pos,
		float mass,
		float radius,
		sgVec4 momentum,
		sgVec3 color
		) {

	sgCopyVec4(this->pos.vec, pos);
	this->mass = kilogram_t(mass);
	this->radius = meter_t(radius);
	sgCopyVec4(this->momentum.vec, momentum);
//	ρ=(3m)/(4πr^2)
	this->density = (3*mass) / (4 * M_PI * (radius * radius));
	sgCopyVec3(this->color.vec, color);
}

Circle::Circle(
		sgVec4 pos,
		kilogram_t mass,
		sgVec4 momentum,
		float density,
		sgVec3 color
) {
	sgCopyVec4(this->pos.vec, pos);
	this->mass = mass;
	// TODO calculate this
//	this->radius = radius;
	 this->radius =   meter_t(sqrt(((3*mass.value()) / 4 * M_PI ) / density));

	sgCopyVec4(this->momentum.vec, momentum);
	this->density = density;
	sgCopyVec3(this->color.vec, color);

}

float Circle::calcRadius(float mass, float density) {
	return sqrt(((3*mass) / 4 * M_PI ) / density);
}

/*
 * Consult method here:
 *   https://www.cmu.edu/biolphys/deserno/pdf/sphere_equi.pdf
 */
vector<VecStruct> Circle::pointsEvenlyDistributedOnSphere(int numPoints, float radius) {
	VecStruct invalidDefault(-1, -1, -1);
    return vector<VecStruct>{invalidDefault};
}

/*
float calcRadius(float mass, int density ) {
	float pieceRadius = pieceVol/(M_PI * 4.0/3.0);
	pieceRadius = pow(pieceRadius, 1.0/3.0);
	return pieceRadius;
}
 */

