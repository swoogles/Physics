/*
 * control_center.cpp
 *
 *  Created on: Jul 17, 2011
 *      Author: brasure
 */

#include "control_center.h"

using namespace std;

bool control_center::paused = false;
duration control_center::dt;
bool control_center::renderOctree = false;

// TODO Is there any better candidate for breaking things apart than this functino?
void control_center::init(duration dt, int windowWidth) {
  control_center::dt = dt;
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
  autoScale_button->setValue(1);
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

void control_center::flipAutoScaling(puObject * caller) {
  Observer::getCurObserverRef().toggleAutoScaling();
}

void control_center::alterDT(puObject * caller) {
  if (strcmp(caller->getLegend(), "Slower") == 0) {
    control_center::dt /= 2;
  }

  if (strcmp(caller->getLegend(), "Faster") == 0) {
    control_center::dt *= 2;
  }

}

void control_center::pause_cb(puObject * caller) {
  paused = !paused;
}

void control_center::rotRight(puObject *) {
  PhysicalVector angVelocity(0,.5,0);
    Observer::getCurObserverRef().adjustAngularVelocity(angVelocity);
}

void control_center::rotLeft(puObject *) {
  PhysicalVector angVelocity(0,-.5,0);
    Observer::getCurObserverRef().adjustAngularVelocity(angVelocity);
}

void control_center::rotUp(puObject *) {
  PhysicalVector angVelocity(+.5,0,0);
    Observer::getCurObserverRef().adjustAngularVelocity(angVelocity);
}
void control_center::rotDown(puObject *) {
  PhysicalVector angVelocity(-.5,0,0);
    Observer::getCurObserverRef().adjustAngularVelocity(angVelocity);
}

void control_center::rotStop(puObject *) {
  PhysicalVector angVelocity(0,0,0);
    Observer::getCurObserverRef().adjustAngularVelocity(angVelocity);
}

bool control_center::isPaused() {
  return control_center::paused;
}

duration control_center::getDt() {
  return control_center::dt;
}

void control_center::toggleOctreeRendering(puObject *) {
    renderOctree = !renderOctree;
}

bool control_center::shouldRenderOctree() {
    return renderOctree;
}

