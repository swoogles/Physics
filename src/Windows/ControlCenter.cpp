/*
 * ControlCenter.cpp
 *
 *  Created on: Jul 17, 2011
 *      Author: brasure
 */

#include "ControlCenter.h"

using namespace std;

bool ControlCenter::paused = false;
duration ControlCenter::dt;
bool ControlCenter::renderOctree = false;

queue<CameraAction> ControlCenter::cameraActions;

void ControlCenter::flipAutoScaling(puObject * caller) {
  ControlCenter::cameraActions.push(CameraAction::TOGGLE_AUTOSCALING);
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
    ControlCenter::cameraActions.push(CameraAction::ROTATE_RIGHT);
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

duration ControlCenter::getDt() const {
  return ControlCenter::dt;
}

void ControlCenter::toggleOctreeRendering(puObject *) {
    renderOctree = !renderOctree;
}

bool ControlCenter::shouldRenderOctree() {
    return renderOctree;
}

ControlCenter::ControlCenter(duration dt, int windowWidth): localDt(dt) {
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

ControlCenter::ControlCenter() {

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

