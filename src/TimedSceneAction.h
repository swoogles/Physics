//
// Created by bfrasure on 6/21/20.
//

#ifndef PHYSICS_TIMEDSCENEACTION_H
#define PHYSICS_TIMEDSCENEACTION_H

#include "lib/units.h"
#include "CameraAction.h"
using namespace units::time;

class TimedSceneAction {
public:
    const second_t triggerTime;
    const CameraAction cameraAction;
    TimedSceneAction(
            second_t triggerTime,
            CameraAction cameraAction
    );
};


#endif //PHYSICS_TIMEDSCENEACTION_H
