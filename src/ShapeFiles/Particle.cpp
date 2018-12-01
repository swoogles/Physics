#include "Particle.h"

double Particle::scale() {
    return radius.value();
}

meter_t Particle::getRadius() {
	return radius;
}

void Particle::setRadius(meter_t radius) {
	this->radius = radius;
}

double Particle::momentOfInertia() {
	return (2 * _mass.value() * (radius.value() * radius.value()))/ 5;
}

Particle::Particle(
        PhysicalVector pos,
        double mass,
        float radius,
        PhysicalVector momentum,
        PhysicalVector color
):MyShape(ShapeType::circle, momentum) // FIX VECSTRUCT!!
,radius(meter_t(radius))
  {
    this->pos = pos;
	this->_mass = kilogram_t(mass);
//	ρ=(3m)/(4πr^2)
	this->_density = kilograms_per_cubic_meter_t((3*mass) / (4 * M_PI * (radius * radius)));
	this->_color = color;
}

Particle::Particle(
        PhysicalVector pos,
        kilogram_t mass,
        PhysicalVector momentum,
        kilograms_per_cubic_meter_t density,
        PhysicalVector color
): MyShape(ShapeType::circle, momentum)
{
	this->pos = pos;
	this->_mass = mass;
    this->radius =   meter_t(sqrt(((3*mass.value()) / 4 * M_PI ) / density.value()));
	this->_density = density;
	this->_color = color;
}

meter_t Particle::calcRadius(kilogram_t mass, kilograms_per_cubic_meter_t density) {
	return meter_t(sqrt(((3*mass.value()) / 4 * M_PI ) / density.value()));
}

/*
 * Consult method here:
 *   https://www.cmu.edu/biolphys/deserno/pdf/sphere_equi.pdf
 */
vector<PhysicalVector> Particle::pointsEvenlyDistributedOnSphere(int numPoints, float radius) {
	PhysicalVector invalidDefault(-1, -1, -1);
    return vector<PhysicalVector>{invalidDefault};
}

/*
float calcRadius(float mass, int density ) {
	float pieceRadius = pieceVol/(M_PI * 4.0/3.0);
	pieceRadius = pow(pieceRadius, 1.0/3.0);
	return pieceRadius;
}
 */

void Particle::mergeWith(Particle &otherShape)
{
	kilogram_t combinedMass = this->mass() + otherShape.mass();
	kilograms_per_cubic_meter_t density = this->density();

	meter_t newRadius = calcRadius(combinedMass, density);

	PhysicalVector totalAngMom = calcMergedAngMomentum(otherShape);

	PhysicalVector COM =
			this->weightedPosition()
					.plus(otherShape.weightedPosition())
					.scaledBy(1/(combinedMass.value()));

	this->setMass(combinedMass);
	this->setRadius(newRadius);

	// TODO Verify this stuff
	otherShape.setMass(kilogram_t(0));
	otherShape.setRadius(meter_t(0));
	// TODO /Verify this stuff

	this->adjustMomentum(otherShape.momentum());

	this->setAngularMomentum(totalAngMom);

	this->calcColor();

	this->setPos(COM);
}

// TODO This should return totalAngMom, instead of mutating parameter.
PhysicalVector Particle::calcMergedAngMomentum(Particle &otherShape)
{
	// TODO PhysicalVector is a little too vague here. *Everything* is a PhysicalVector??
	PhysicalVector aPos(this->position());
	PhysicalVector bPos(otherShape.position());
	PhysicalVector aMomentum(this->momentum());
	PhysicalVector bMomentum(otherShape.momentum());

	vecPtr sepVec(PhysicalVector::vecFromAtoB(aPos, bPos));

	PhysicalVector hitPointOnA = sepVec->unit().scaledBy(this->getRadius().value());

	PhysicalVector hitPt = aPos.plus(hitPointOnA);

	PhysicalVector rForA = aPos.minus(hitPt);

	PhysicalVector newAngularMomentumForA = rForA.vectorProduct3(aMomentum);

	PhysicalVector rForB = bPos.minus(hitPt);

	PhysicalVector newAngularMomentumForB = rForB.vectorProduct3(bMomentum);
	PhysicalVector totalAngMom = newAngularMomentumForA.plus(newAngularMomentumForB);

	return totalAngMom
			.plus(this->angularMomentum())
			.plus(otherShape.angularMomentum());
}

bool Particle::isTouching(Particle &otherShape)
{
	PhysicalVector sepVec(this->vectorTo(otherShape));
	double minSep = (this->getRadius() + otherShape.getRadius()).value();

	return (sepVec.length() < minSep);
}

