#include "Particle.h"

double Particle::getScale() {
    return radius.value();
}

meter_t Particle::getRadius() {
	return radius;
}

void Particle::setRadius(meter_t radius) {
	this->radius = radius;
}

double Particle::getMomentOfInertia() {
	return (2 * mass.value() * (radius.value() * radius.value()))/ 5;
}

Particle::Particle(
        sgVec4 pos,
        double mass,
        float radius,
        VecStruct momentum,
        sgVec3 color
):MyShape(ShapeType::circle, momentum) // FIX VECSTRUCT!!
  {
	sgCopyVec4(this->pos.vec, pos);
	this->mass = kilogram_t(mass);
	this->radius = meter_t(radius);
//	ρ=(3m)/(4πr^2)
	this->density = kilograms_per_cubic_meter_t((3*mass) / (4 * M_PI * (radius * radius)));
	sgCopyVec3(this->color.vec, color);
}

Particle::Particle(
		sgVec4 pos,
		kilogram_t mass,
		VecStruct momentum,
		kilograms_per_cubic_meter_t density,
		sgVec3 color
): MyShape(ShapeType::circle, momentum) {
	sgCopyVec4(this->pos.vec, pos);
	this->mass = mass;
    this->radius =   meter_t(sqrt(((3*mass.value()) / 4 * M_PI ) / density.value()));
	this->density = density;
	sgCopyVec3(this->color.vec, color);
}

meter_t Particle::calcRadius(kilogram_t mass, kilograms_per_cubic_meter_t density) {
	return meter_t(sqrt(((3*mass.value()) / 4 * M_PI ) / density.value()));
}

/*
 * Consult method here:
 *   https://www.cmu.edu/biolphys/deserno/pdf/sphere_equi.pdf
 */
vector<VecStruct> Particle::pointsEvenlyDistributedOnSphere(int numPoints, float radius) {
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

void Particle::mergeWith(Particle &otherShape)
{
	kilogram_t combinedMass = this->getMass() + otherShape.getMass();
	kilograms_per_cubic_meter_t density = this->getDensity();

	meter_t newRadius = calcRadius(combinedMass, density);

	VecStruct totalAngMom = calcMergedAngMomentum(otherShape);

	VecStruct COM =
			this->getWeightedPosition()
					.plus(otherShape.getWeightedPosition())
					.scaledBy(1/(combinedMass.value()));

	this->setMass(combinedMass);
	this->setRadius(newRadius);

	// TODO Verify this stuff
	otherShape.setMass(kilogram_t(0));
	otherShape.setRadius(meter_t(0));
	// TODO /Verify this stuff

	this->adjustMomentum(otherShape.getMomentum());

	this->setAngMomentum(totalAngMom);

	this->calcColor();

	this->setPos(COM);
}

// TODO This should return totalAngMom, instead of mutating parameter.
VecStruct Particle::calcMergedAngMomentum(Particle &otherShape)
{
	// TODO VecStruct is a little too vague here. *Everything* is a VecStruct??
	VecStruct aPos(this->getPos());
	VecStruct bPos(otherShape.getPos());
	VecStruct aMomentum(this->getMomentum());
	VecStruct bMomentum(otherShape.getMomentum());

	vecPtr sepVec(VecStruct::vecFromAtoB(aPos, bPos));

	VecStruct hitPointOnA = sepVec->unit().scaledBy(this->getRadius().value());

	VecStruct hitPt = aPos.plus(hitPointOnA);

	VecStruct rForA = aPos.minus(hitPt);

	VecStruct newAngularMomentumForA = rForA.vectorProduct3(aMomentum);

	VecStruct rForB = bPos.minus(hitPt);

	VecStruct newAngularMomentumForB = rForB.vectorProduct3(bMomentum);
	VecStruct totalAngMom = newAngularMomentumForA.plus(newAngularMomentumForB);

	return totalAngMom
			.plus(this->getAngMomentum())
			.plus(otherShape.getAngMomentum());
}

bool Particle::isTouching(Particle &otherShape)
{
	VecStruct sepVec(this->getVectorToObject(otherShape));
	double minSep = (this->getRadius() + otherShape.getRadius()).value();

	return (sepVec.length() < minSep);
}

