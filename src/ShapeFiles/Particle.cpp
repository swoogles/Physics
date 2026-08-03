#include "Particle.h"
#include <algorithm>

// Default to realistic collision detection
float Particle::collisionRadiusMultiplier = 1.0f;
float Particle::collisionRadiusStartMultiplier = 1.0f;
float Particle::mergeTargetFraction = 0.9f;
int Particle::mergeTargetSteps = 4320;
int Particle::baseMergeTargetSteps = 4320;
//! Five seconds of video at 24 fps.
int Particle::mergeGraceSteps = 120;
int Particle::initialParticleCount = 0;
int Particle::lastMergeStep = 0;
int Particle::previousParticleCount = 0;

// Damping and boundary settings
float Particle::velocityDamping = 1.0f;
float Particle::boundaryStrength = 0.0f;
float Particle::boundaryRadius = 0.0f;
float Particle::systemCenterX = 0.0f;
float Particle::systemCenterY = 0.0f;
float Particle::systemCenterZ = 0.0f;

void Particle::setDampingAndBoundary(float damping, float strength, float radius, PhysicalVector center) {
    velocityDamping = damping;
    boundaryStrength = strength;
    boundaryRadius = radius;
    systemCenterX = center.x();
    systemCenterY = center.y();
    systemCenterZ = center.z();
    cout << "Damping and boundary initialized: damping=" << damping
         << ", boundaryStrength=" << strength
         << ", boundaryRadius=" << radius
         << ", center=(" << systemCenterX << ", " << systemCenterY << ", " << systemCenterZ << ")" << endl;
}

// Minimum merges per frame
int Particle::minimumMergesPerFrame = 0;

void Particle::setMinimumMergesPerFrame(int minMerges) {
    minimumMergesPerFrame = minMerges;
    cout << "Minimum merges per frame: " << minMerges << endl;
}

float Particle::maxCollisionRadiusMultiplier = 0;
double Particle::maxMergeJump = 0;
double Particle::largestMergeJump = 0;
double Particle::largestVisibleMergeJump = 0;

void Particle::setMergeLimits(float maxMultiplier, double maxJump) {
    maxCollisionRadiusMultiplier = maxMultiplier;
    maxMergeJump = maxJump;
    largestMergeJump = 0;
    largestVisibleMergeJump = 0;
    cout << "Merge limits: multiplier capped at " << maxMultiplier
         << ", merge jump capped at " << maxJump << " m" << endl;
}

double Particle::mergeJumpFor(const Particle &a, const Particle &b) {
    const double massA = a._mass.value();
    const double massB = b._mass.value();
    const double total = massA + massB;
    if (total <= 0) {
        return 0;
    }

    const double separation = a.pos.minus(b.pos).length();
    return separation * std::min(massA, massB) / total;
}

/*! How far this body may be moved by a merge before it reads as a teleport.
 *
 *  How much a jump bothers the eye scales with how big the thing that jumped
 *  is: a speck hopping across the frame goes unnoticed, a fat dot doing it is
 *  jarring. So the allowance is inversely proportional to drawn size, pinned
 *  so that max_merge_jump_fraction is the budget for a mid-sized body. Holding
 *  specks to the same limit would throttle early coalescing for no visual gain.
 */
double Particle::allowedJumpFor(double mass) {
    const double referenceSize = 4.0;
    const double size = std::max(1.0, (double) pointSizeFor(mass));
    return maxMergeJump * referenceSize / size;
}

bool Particle::mergeAllowed(const Particle &a, const Particle &b) {
    if (maxMergeJump <= 0) {
        return true;
    }

    const double heavier = std::max(a._mass.value(), b._mass.value());
    return mergeJumpFor(a, b) <= allowedJumpFor(heavier);
}

double Particle::renderReferenceMass = 0;
double Particle::maxPointSize = 14.0;

void Particle::setRenderScale(double referenceMass, double maxPointSize) {
    Particle::renderReferenceMass = referenceMass;
    Particle::maxPointSize = std::max(1.0, maxPointSize);
    cout << "Render scale: one particle = " << referenceMass
         << " kg at 1px, capped at " << Particle::maxPointSize << "px" << endl;
}

/*! Cube root of the mass ratio, so a dot grows the way a real radius would:
 *  1000 merged particles draw 10x wider, not 1000x.
 */
float Particle::pointSizeFor(double mass) {
    if (renderReferenceMass <= 0 || mass <= renderReferenceMass) {
        return 1.0f;
    }
    const double size = cbrt(mass / renderReferenceMass);
    return (float) std::min(size, maxPointSize);
}

void Particle::setCollisionRadiusMultiplier(float startMultiplier, float targetFraction, int targetSteps, int initialCount) {
    collisionRadiusStartMultiplier = startMultiplier;
    mergeTargetFraction = targetFraction;
    mergeTargetSteps = targetSteps;
    baseMergeTargetSteps = targetSteps;
    initialParticleCount = initialCount;
    previousParticleCount = initialCount;
    lastMergeStep = 0;
    collisionRadiusMultiplier = startMultiplier;
    cout << "Collision radius multiplier initialized: start=" << startMultiplier
         << ", mergeTarget=" << (targetFraction * 100) << "%"
         << ", targetSteps=" << targetSteps
         << ", initialParticles=" << initialCount << endl;
}

void Particle::markArrivedAt(int step) {
    _arrivedAtStep = step;
}

bool Particle::withinArrivalGrace(const Particle &particle, int currentStep) {
    if (particle._arrivedAtStep <= 0 || mergeGraceSteps <= 0) {
        return false;
    }
    return (currentStep - particle._arrivedAtStep) < mergeGraceSteps;
}

void Particle::noteGroupArrived(int particleCount, int currentStep) {
    if (particleCount <= 0) {
        return;
    }

    // There is genuinely more material to merge now, so say so.
    initialParticleCount += particleCount;
    previousParticleCount += particleCount;

    /* Push the deadline out by however long this group was held back. A group
     * arriving at 20s otherwise inherits a schedule the original material has
     * already spent 20 seconds against, and the run tries to make up the
     * difference by merging the newcomer as fast as it can. */
    mergeTargetSteps = currentStep + baseMergeTargetSteps;

    // Let the new material ramp up from the same place the originals did.
    collisionRadiusMultiplier = collisionRadiusStartMultiplier;
    lastMergeStep = currentStep;

    cout << "Group arrived: " << particleCount << " particles"
         << " | merge deadline now step " << mergeTargetSteps
         << " | multiplier back to " << collisionRadiusMultiplier
         << " | grace " << mergeGraceSteps << " frames" << endl;
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

    if (maxCollisionRadiusMultiplier > 0 && collisionRadiusMultiplier > maxCollisionRadiusMultiplier) {
        collisionRadiusMultiplier = maxCollisionRadiusMultiplier;
    }

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

	const double jump = COM.minus(this->position()).length();
	if (jump > largestMergeJump) {
		largestMergeJump = jump;
	}
	if (pointSizeFor(combinedMass.value()) >= 4.0f && jump > largestVisibleMergeJump) {
		largestVisibleMergeJump = jump;
	}

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
