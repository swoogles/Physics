#include "Particle.h"
#include <algorithm>

// Default to realistic collision detection
float Particle::collisionRadiusMultiplier = 1.0f;
float Particle::collisionRadiusStartMultiplier = 1.0f;
float Particle::mergeTargetFraction = 0.9f;
int Particle::mergeTargetSteps = 4320;
int Particle::initialParticleCount = 0;
int Particle::lastMergeStep = 0;
int Particle::previousParticleCount = 0;

void Particle::setCollisionRadiusMultiplier(float startMultiplier, float targetFraction, int targetSteps, int initialCount) {
    collisionRadiusStartMultiplier = startMultiplier;
    mergeTargetFraction = targetFraction;
    mergeTargetSteps = targetSteps;
    initialParticleCount = initialCount;
    previousParticleCount = initialCount;
    lastMergeStep = 0;
    collisionRadiusMultiplier = startMultiplier;
    cout << "Collision radius multiplier initialized: start=" << startMultiplier
         << ", mergeTarget=" << (targetFraction * 100) << "%"
         << ", targetSteps=" << targetSteps
         << ", initialParticles=" << initialCount << endl;
}

void Particle::updateCollisionRadiusMultiplier(int currentParticleCount, int currentStep) {
    if (initialParticleCount <= 0 || mergeTargetSteps <= 0) return;

    // Detect if a merge happened
    if (currentParticleCount < previousParticleCount) {
        lastMergeStep = currentStep;
    }
    previousParticleCount = currentParticleCount;

    // Progress: fraction of particles that have merged (0.0 to 1.0)
    float progress = 1.0f - (float)currentParticleCount / (float)initialParticleCount;

    // Stop growing once we hit the target
    if (progress >= mergeTargetFraction) return;

    // Time progress (0.0 to 1.0)
    float timeProgress = (float)currentStep / (float)mergeTargetSteps;

    // How far behind are we? (positive = behind schedule)
    // Expected progress at this time = timeProgress * mergeTargetFraction
    float expectedProgress = timeProgress * mergeTargetFraction;
    float behindBy = std::max(0.0f, expectedProgress - progress);

    // Stall detection: frames since last merge - exponential response
    int framesSinceLastMerge = currentStep - lastMergeStep;
    // Exponential growth: doubles every 10 frames of stall
    float stallFactor = std::pow(2.0f, (float)framesSinceLastMerge / 10.0f);

    // Urgency: increases as we approach deadline with work remaining
    float remainingProgress = mergeTargetFraction - progress;
    float remainingTime = std::max(0.01f, 1.0f - timeProgress);
    float urgency = remainingProgress / remainingTime;  // How much progress per remaining time unit

    // Base increment
    float baseIncrement = 0.05f;

    // Scale by factors:
    // - stallFactor: grows when no merges happening
    // - urgency: grows as deadline approaches
    // - behindBy: extra boost when behind schedule
    float increment = baseIncrement * stallFactor * (1.0f + urgency) * (1.0f + behindBy * 5.0f);

    float previousMultiplier = collisionRadiusMultiplier;
    collisionRadiusMultiplier += increment;

    // Log periodically (every ~20 units or significant stall)
    bool significantChange = (int)(collisionRadiusMultiplier / 20) > (int)(previousMultiplier / 20);
    bool stallWarning = (framesSinceLastMerge > 0 && framesSinceLastMerge % 500 == 0);
    if (significantChange || stallWarning) {
        cout << "Collision multiplier: " << collisionRadiusMultiplier
             << " | " << (progress * 100) << "% merged"
             << " | step " << currentStep << "/" << mergeTargetSteps
             << " | stall: " << framesSinceLastMerge << " frames" << endl;
    }
}

double Particle::scale() const {
    return _radius.value();
}

meter_t Particle::radius() const {
	return _radius;
}

void Particle::setRadius(meter_t radius) {
	this->_radius = radius;
}

double Particle::momentOfInertia() const {
	return ((2 * _mass * (_radius * _radius))/ 5).value();
}

Particle::Particle(
        PhysicalVector pos,
        double mass,
        float radius,
        PhysicalVector momentum,
        PhysicalVector color
):Moveable(ShapeType::circle, momentum) // FIX VECSTRUCT!!
,_radius(meter_t(radius))
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
): Moveable(ShapeType::circle, momentum)
{
	this->pos = pos;
	this->_mass = mass;
    this->_radius =   meter_t(sqrt(((3*mass.value()) / 4 * M_PI ) / density.value()));
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

void Particle::mergeWith(Particle &otherShape) {
    this->_color =
            (this->mass() < otherShape.mass() )
            ? otherShape._color: this->_color ;
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

	otherShape.setMass(kilogram_t(0));
	otherShape.setRadius(meter_t(0));

	this->adjustMomentum(otherShape.momentum());
	this->setAngularMomentum(totalAngMom);
	this->calcColor();
	this->setPos(COM);
}

PhysicalVector Particle::calcMergedAngMomentum(Particle &otherShape) const {
	// TODO PhysicalVector is a little too vague here. *Everything* is a PhysicalVector??
	PhysicalVector aPos(this->position());
	PhysicalVector bPos(otherShape.position());
	PhysicalVector aMomentum(this->momentum());
	PhysicalVector bMomentum(otherShape.momentum());

    std::unique_ptr<PhysicalVector> sepVec(PhysicalVector::vecFromAtoB(aPos, bPos));

	PhysicalVector hitPointOnA = sepVec->unit().scaledBy(this->radius().value());

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

bool Particle::isTouching(Particle &otherShape) const {
	PhysicalVector sepVec(this->vectorTo(otherShape));
	double minSep = (this->radius() + otherShape.radius()).value() * collisionRadiusMultiplier;

	return (sepVec.length() < minSep);
}

bool Particle::isTouching(PhysicalVector pos, meter_t radius) const {
    double minSep = (this->radius() + radius).value() * collisionRadiusMultiplier;
    return this->position().minus(pos).length() < minSep;
}
