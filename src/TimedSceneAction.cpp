//
// Created by bfrasure on 6/21/20.
//

#include "TimedSceneAction.h"

TimedSceneAction::TimedSceneAction(
        second_t triggerTime,
        CameraAction cameraAction
): triggerTime(triggerTime)
, cameraAction(cameraAction)
{}
