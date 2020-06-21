/*
 * ControlCenter.cpp
 *
 *  Created on: Jul 17, 2011
 *      Author: brasure
 */

#include "ControlCenter.h"

using namespace std;

// TODO Ground these. No longer a need to make them static
bool ControlCenter::paused = false;
local_duration ControlCenter::dt;
time_point<system_clock, duration<long, ratio<1, 1000000000>>> ControlCenter::static_start;
bool ControlCenter::renderOctree = false;

queue<CameraAction> ControlCenter::cameraActions;

void ControlCenter::flipAutoScaling(puObject * caller) {
    cout << "I can use this button to record instead" << endl;
  ControlCenter::cameraActions.push(CameraAction::TOGGLE_AUTOSCALING);
}

void ControlCenter::createVideoCallback(puObject * caller) {
    cout << "should create a new video now.";
    FfmpegClient client;
    client.createVideo(system_clock::to_time_t(static_start));

}
void ControlCenter::alterDT(puObject * caller) {
  if (strcmp(caller->getLegend(), "Slower") == 0) {
    ControlCenter::dt /= 2;
  }

  if (strcmp(caller->getLegend(), "Faster") == 0) {
    ControlCenter::dt *= 2;
  }

}

void ControlCenter::pause_cb(puObject * caller) {
  paused = !paused;
}

void ControlCenter::rotRight(puObject *) {
    for (int i = 0; i < 90; i ++) {
        ControlCenter::cameraActions.push(CameraAction::ROTATE_RIGHT);
    }
}

void ControlCenter::rotLeft(puObject *) {
  ControlCenter::cameraActions.push(CameraAction::ROTATE_LEFT);
}

void ControlCenter::rotUp(puObject *) {
  ControlCenter::cameraActions.push(CameraAction::ROTATE_UP);
}
void ControlCenter::rotDown(puObject *) {
  ControlCenter::cameraActions.push(CameraAction::ROTATE_DOWN);
}

void ControlCenter::rotStop(puObject *) {
  ControlCenter::cameraActions.push(CameraAction::STOP_ROTATION);
}

bool ControlCenter::isPaused() const {
  return ControlCenter::paused;
}

local_duration ControlCenter::getDt() const {
  return ControlCenter::dt;
}

void ControlCenter::toggleOctreeRendering(puObject *) {
    renderOctree = !renderOctree;
}

bool ControlCenter::shouldRenderOctree() {
    return renderOctree;
}

ControlCenter::ControlCenter(local_duration dt, int windowWidth, time_point<system_clock, duration<long, ratio<1, 1000000000>>> startParam):
localDt(dt),
start(startParam)
{
  this->static_start = startParam;
  this->dt = dt;
  showingRunTime = false;

  int curHeight = 200;
  int elementHeight = 30;
  int startX = windowWidth/2;
  int curX = startX, placementWidth = 60, gap = 5;

  runtime_group = new puGroup(0,0);


  dec_dt_button = new puOneShot(curX, curHeight - elementHeight, curX+placementWidth, curHeight);
  curX += (placementWidth +gap);
  dec_dt_button->setLegend("Slower");
  dec_dt_button->setCallback( alterDT );

  pause_dt_button = new puButton(curX, curHeight - elementHeight, curX+placementWidth, curHeight);
  curX += (placementWidth +gap);
  pause_dt_button->setLegend("Pause");
  pause_dt_button->setCallback(pause_cb);
  pause_dt_button->setValue(0);


  inc_dt_button = new puOneShot(curX, curHeight - elementHeight, curX+placementWidth, curHeight);
  inc_dt_button->setLegend("Faster");
  inc_dt_button->setCallback(alterDT);

    curX += (placementWidth +gap);
    createVideo = new puOneShot(curX, curHeight - elementHeight, curX+placementWidth*2, curHeight);
    createVideo->setLegend("Create Video");
    createVideo->setCallback(this->createVideoCallback);
//createVideo(puOneShot(curX, curHeight - elementHeight, curX+placementWidth, curHeight))

  curHeight -= elementHeight;

  curX = startX;
  curX += (placementWidth + gap);

  rotUp_button = new puArrowButton(curX, curHeight - elementHeight, curX+placementWidth, curHeight, PUARROW_UP);
  rotUp_button->setCallback(rotUp);

  curHeight -= elementHeight;

  curX = startX;

  dec_rotSide_button = new puArrowButton(curX, curHeight - elementHeight, curX+placementWidth, curHeight, PUARROW_LEFT);
  curX += (placementWidth +gap);
  dec_rotSide_button->setCallback(rotRight);

  pause_rotSide_button = new puButton(curX, curHeight - elementHeight, curX+placementWidth, curHeight);
  curX += (placementWidth +gap);
  pause_rotSide_button->setLegend("Stop");
  pause_rotSide_button->setCallback(rotStop);
  pause_rotSide_button->setValue(0);


  inc_rotSide_button = new puArrowButton(curX, curHeight - elementHeight, curX+placementWidth, curHeight, PUARROW_RIGHT);
  inc_rotSide_button->setCallback(rotLeft);

  curHeight -= elementHeight;

  curX = startX;
  curX += (placementWidth + gap);

  rotDown_button = new puArrowButton(curX, curHeight - elementHeight, curX+placementWidth, curHeight, PUARROW_DOWN);
  rotDown_button->setCallback(rotDown);

  curHeight -= elementHeight;
  curX = startX;

  autoScale_button = new puButton(curX, curHeight, curX+16, curHeight+16, PUBUTTON_RADIO);
  autoScale_button->setLabelPlace( PUPLACE_CENTERED_RIGHT );
  autoScale_button->setLabel("Autoscale");
  autoScale_button->setValue(0);
  autoScale_button->setCallback(flipAutoScaling);

  curHeight -= elementHeight;
  renderOctree_button = new puButton(curX, curHeight, curX+16, curHeight+16, PUBUTTON_RADIO);
  renderOctree_button->setLabelPlace( PUPLACE_CENTERED_RIGHT );
  renderOctree_button->setLabel("Render Octree");
  renderOctree_button->setValue(1);
  renderOctree_button->setCallback(toggleOctreeRendering);
  renderOctree_button->clrValue();

  runtime_group->close();
}

optional<CameraAction> ControlCenter::currentCameraAction() {
  if (!ControlCenter::cameraActions.empty()) {
    cout << "Number of actions remaining in CameraAction queue: " << ControlCenter::cameraActions.size() << endl;
    auto currentAction = ControlCenter::cameraActions.front();
    ControlCenter::cameraActions.pop();
    return currentAction;
  } else {
    return {};
  }
}

