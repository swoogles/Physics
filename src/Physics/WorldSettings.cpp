#include "WorldSettings.h"

bool WorldSettings::autoScaling;

void WorldSettings::setAutoScaling(bool shouldAutoScale) {
	autoScaling = shouldAutoScale;
}

bool WorldSettings::isAutoScaling() {
	return autoScaling;
}
