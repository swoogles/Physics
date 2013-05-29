/*
 * StraightShape.cpp
 *
 *  Created on: Jul 10, 2011
 *      Author: brasure
 */

#include "StraightShape.h"

StraightShape::StraightShape() {
	width = 1;
	height = 1;
	depth = 1;

}

StraightShape::~StraightShape() {
}

float StraightShape::getMarkerSize() {
	return (width + height)/50;
}

void StraightShape::scaleMembers(float scalarFac) {
	width *= scalarFac;
	height *= scalarFac;
}

void StraightShape::scaleMembers(float xFac, float yFac, float zFac) {
	width *= xFac;
	height *= yFac;
	depth *= zFac;
}

void StraightShape::drawScale() {
	glScalef(width, height, depth);
}

void StraightShape::resize(float newWidth, float newHeight, float newDepth) {
	width = newWidth;
	height = newHeight;
	depth = newDepth;
}
