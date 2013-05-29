/*
 * WorldSettings.cpp
 *
 *  Created on: Jul 28, 2011
 *      Author: brasure
 */

#include "WorldSettings.h"

float WorldSettings::DT;
double WorldSettings::timeElapsed;
bool WorldSettings::paused;
float WorldSettings::totalMass;

bool WorldSettings::allElastic;
bool WorldSettings::allInelastic;

bool WorldSettings::constGravField;
bool WorldSettings::gravBetweenObjects;

sgVec4 WorldSettings::gravField;

float WorldSettings::minX;
float WorldSettings::minY;
float WorldSettings::maxX;
float WorldSettings::maxY;

bool WorldSettings::autoScaling;

void WorldSettings::setAutoScaling(bool shouldAutoScale) {
	autoScaling = shouldAutoScale;
}

bool WorldSettings::isAutoScaling() {
	return autoScaling;
}



void WorldSettings::setDT(float newDT) {
	DT = newDT;
}

float WorldSettings::getDT() {
	return DT;
}

void WorldSettings::setTimeElapsed(double newTimeElapsed) {
	timeElapsed = newTimeElapsed;
}

void WorldSettings::updateTimeElapsed() {
	timeElapsed += DT;
}

double WorldSettings::getTimeElapsed() {
	return timeElapsed;
}

void WorldSettings::Pause() {
	paused = true;
}

void WorldSettings::unPause() {
	paused = false;
}

bool WorldSettings::isPaused() {
	return paused;
}

void WorldSettings::setTotalMass(float inMass) {
	totalMass = inMass;
}

void WorldSettings::adjustTotalMass(float dMass) {
	totalMass += dMass;
}

float WorldSettings::getTotalMass() {
	return totalMass;
}

bool WorldSettings::isAllElastic() {
	return allElastic;
}

bool WorldSettings::isAllInelastic() {
	return allInelastic;
}

bool WorldSettings::isMixedElasticity() {
	return !(allElastic || allInelastic);
}

void WorldSettings::makeAllElastic() {
	allElastic = true;
	allInelastic = false;
}

void WorldSettings::makeAllInelastic() {
	allInelastic = true;
	allElastic = false;
}

void WorldSettings::makeMixedElasticity() {
	allElastic = false;
	allInelastic = false;
}

bool WorldSettings::isConstGravField() {
	return constGravField;
}

void WorldSettings::setConstGravField(bool useGravField) {
	constGravField = useGravField;
}

void WorldSettings::setConstGravFieldVal(sgVec4 newGravField) {
	sgCopyVec4(gravField, newGravField);
}

void WorldSettings::getConstGravFieldVal(sgVec4 retGravField) {
	sgCopyVec4(retGravField, gravField);
}

bool WorldSettings::isGravBetweenObjects() {
	return gravBetweenObjects;
}

void WorldSettings::setGravBetweenObjects(bool newVal) {
	gravBetweenObjects = newVal;
}

void WorldSettings::resetXYMinsAndMaxes() {
	minX = FLT_MAX;
	minY = FLT_MAX;

	maxX = -(FLT_MAX-1);
	maxY = -(FLT_MAX-1);
}

void WorldSettings::updateXYMinsAndMaxes(sgVec4 curPos) {
	if (curPos[0] < minX)
		minX = curPos[0];
	if (curPos[0] > maxX)
		maxX = curPos[0];
	if (curPos[1] < minY)
		minY = curPos[1];
	if (curPos[1] > maxY)
		maxY = curPos[1];
}

float WorldSettings::getMinX() {
	return minX;
}

float WorldSettings::getMinY() {
	return minY;
}

float WorldSettings::getMaxX() {
	return maxX;
}

float WorldSettings::getMaxY() {
	return maxY;
}
