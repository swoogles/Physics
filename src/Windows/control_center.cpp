/*
 * control_center.cpp
 *
 *  Created on: Jul 17, 2011
 *      Author: brasure
 */

#include "control_center.h"

//Group 1
// puGroup * control_center::placement_group;
// puaSelectBox * control_center::pick_object;
// puText * control_center::object_name_label;
// puInput * control_center::object_name_input;
// 
// puText * control_center::placement_label;
// puText * control_center::placement_label2;
// puInput * control_center::placementX_in;
// puInput * control_center::placementY_in;
// puInput * control_center::placementZ_in;
// 
// puText * control_center::momentum_label;
// puText * control_center::momentum_label2;
// puInput * control_center::momentumX_in;
// puInput * control_center::momentumY_in;
// puInput * control_center::momentumZ_in;
// 
// puText * control_center::mass_label;
// puInput * control_center::mass_in;
// 
// puButton * control_center::clear_objects;

//Group 2
// puGroup * control_center::runtime_group;
// puText * control_center::speed_c_label;
// puInput * control_center::speed_c_input;

//Control speed
// puOneShot * control_center::inc_dt_button;
// puOneShot * control_center::dec_dt_button;
// puButton * control_center::pause_dt_button;

//Control viewpoint
// puArrowButton * control_center::inc_rotSide_button;
// puArrowButton * control_center::dec_rotSide_button;
// puButton * control_center::pause_rotSide_button;
// puArrowButton * control_center::rotUp_button;
// puArrowButton * control_center::rotDown_button;

//Autoscale
// puButton * control_center::autoScale_button;


// bool control_center::showingRuntime = false;

// sgVec3 control_center::userDat = {2,-1,0};

// puButton * control_center::viewSwitcher;

using namespace std;

void control_center::clearShapes(puObject * caller) {
  shared_ptr<Simulation> * spPointer = (shared_ptr<Simulation> *)caller->getUserData();
  shared_ptr<Simulation> curSimulation = shared_ptr<Simulation>( *spPointer );
  curSimulation->getPhysicalObjects().clearShapes();
}

void control_center::switchViewNow(puObject * caller) {
  control_center * ccPointer = ( control_center *)caller->getUserData();
	if ( ccPointer->isShowingRunTime() ) {
    ccPointer->hideRunTimeGroup();
    ccPointer->showPlacementGroup();
    ccPointer->setShowingRunTime( false );
	}
	else {
    ccPointer->showRunTimeGroup();
    ccPointer->hidePlacementGroup();
    ccPointer->setShowingRunTime( true );
	}

}

void control_center::setSimulation( shared_ptr<Simulation> simulation )
{
  // this->simulation = boost::make_shared<Simulation>( simulation );
  this->simulation =  simulation ;
  cout << "setSimulation.dt.post: " << (this->simulation)->getDT() << endl;
}

void control_center::init( shared_ptr<Simulation> residentSimulation ) {
  showingRunTime = false;
  userDat[0]=2;
  userDat[1]=-1;
  userDat[2]=0;

  int winHeight = glutGet(GLUT_WINDOW_HEIGHT);

  int curHeight = winHeight;
  int elementHeight = 30;
  int *textWidth = new int;
  int *textHeight = new int;
  *textWidth = 0;
  *textHeight = 0;

  int vsXinit = 5;
  int vsYinit = 5;
  int vsHeight = 40;
  int vsWidth = 100;
  viewSwitcher = new puButton(vsXinit, vsYinit, vsXinit+vsWidth, vsYinit+vsHeight);
  viewSwitcher->setLegend("SwitchView");
  viewSwitcher->setUserData( this );
  viewSwitcher->setCallback(switchViewNow);

  placement_group = new puGroup(0,0);
  object_name_label = new puText(5, curHeight-elementHeight);
  object_name_label->setLabel("Name:");
  object_name_label->setColour(PUCOL_LABEL, 1.0, 0.0, 1.0, .5);

  object_name_input = new puInput(55, curHeight-elementHeight, 55+45, curHeight);
  object_name_input->acceptInput();

  curHeight -= elementHeight;

  curHeight = 200;
  int startX = glutGet(GLUT_WINDOW_WIDTH)/2;
  int curX = startX, placementWidth = 50, gap = 5;
  placement_label = new puText(curX-55, curHeight-elementHeight);
  placement_label->setLabel("Pos: <");
  placementX_in = new puInput(curX, curHeight-elementHeight, curX+placementWidth, curHeight ); curX += (placementWidth+gap);
  placementX_in->setStyle(PUSTYLE_PLAIN);
  placementX_in->setValue(0);
  placementY_in = new puInput(curX, curHeight-elementHeight, curX+placementWidth, curHeight ); curX += (placementWidth+gap);
  placementY_in->setStyle(PUSTYLE_PLAIN);
  placementY_in->setValue(0);
  placementZ_in = new puInput(curX, curHeight-elementHeight, curX+placementWidth, curHeight ); curX += (placementWidth+gap);
  placementZ_in->setStyle(PUSTYLE_PLAIN);
  placementZ_in->setValue(0);
  placement_label2 = new puText(curX, curHeight-elementHeight);
  placement_label2->setLabel(">");
  curHeight -= (elementHeight+gap);

  curX = startX;
  momentum_label = new puText(curX-95, curHeight-elementHeight);
  momentum_label->setLabel("Momentum: <");
  momentumX_in = new puInput(curX, curHeight - elementHeight, curX+placementWidth, curHeight); curX += (placementWidth +gap);
  momentumX_in->setStyle(PUSTYLE_PLAIN);
  momentumX_in->setValue(0);
  momentumY_in = new puInput(curX, curHeight - elementHeight, curX+placementWidth, curHeight); curX += (placementWidth +gap);
  momentumY_in->setStyle(PUSTYLE_PLAIN);
  momentumY_in->setValue(0);
  momentumZ_in = new puInput(curX, curHeight - elementHeight, curX+placementWidth, curHeight); curX += (placementWidth +gap);
  momentumZ_in->setStyle(PUSTYLE_PLAIN);
  momentumZ_in->setValue(0);
  momentum_label2 = new puText(curX, curHeight-elementHeight);
  momentum_label2->setLabel(">");
  curHeight -= (elementHeight+gap);

  curX = startX;
  mass_label = new puText(curX-65, curHeight-elementHeight);
  mass_label->setLabel("Mass:");
  mass_in = new puInput (curX, curHeight-elementHeight, curX+placementWidth, curHeight);
  mass_in->setStyle(PUSTYLE_PLAIN);
  mass_in->setValue(1);
  curHeight -= (elementHeight+gap);

  clear_objects = new puButton(curX, curHeight-elementHeight, curX+placementWidth, curHeight);
  clear_objects->setLegend("Clear Objects(x)");
  clear_objects->setUserData( &residentSimulation );
  clear_objects->setCallback(clearShapes);

  placement_group->close();
  placement_group->hide();


  //Next button group
  curHeight = 200;
  runtime_group = new puGroup(0,0);
  speed_c_label = new puText(5, curHeight-elementHeight);
  speed_c_label->setLabel("Light Speed:");
  speed_c_label->setColour(PUCOL_LABEL, 1.0, 0.0, 0.0, 1.0);

  speed_c_input = new puInput(110, curHeight-elementHeight, 110+60, curHeight);
  //speed_c_input->setLabel("Type c here");
  speed_c_input->setValue("3e6");
  speed_c_input->setStyle(PUSTYLE_PLAIN);
  curHeight -= elementHeight;
  //Way to pass in data for use in callback function
  // filler->setUserData(& userDat);
  //****Could definitely be useful

  curHeight -= elementHeight;

  curHeight = 200;
  curX = startX;
  placementWidth = 60;
  gap = 5;

  dec_dt_button = new puOneShot(curX, curHeight - elementHeight, curX+placementWidth, curHeight);
  curX += (placementWidth +gap);
  dec_dt_button->setLegend("Slower");
  dec_dt_button->setUserData( &residentSimulation );
  dec_dt_button->setCallback( alterDT );

  shared_ptr<Simulation> * spPointer = (shared_ptr<Simulation> *)dec_dt_button->getUserData();

  pause_dt_button = new puButton(curX, curHeight - elementHeight, curX+placementWidth, curHeight);
  curX += (placementWidth +gap);
  pause_dt_button->setLegend("Pause");
  pause_dt_button->setUserData( &residentSimulation );
  pause_dt_button->setCallback(pause_cb);
  pause_dt_button->setValue(0);


  inc_dt_button = new puOneShot(curX, curHeight - elementHeight, curX+placementWidth, curHeight);
  curX += (placementWidth +gap);
  inc_dt_button->setLegend("Faster");
  inc_dt_button->setUserData( &residentSimulation );
  inc_dt_button->setCallback(alterDT);

  curHeight -= elementHeight;

  curX = startX;
  curX += (placementWidth + gap);

  rotUp_button = new puArrowButton(curX, curHeight - elementHeight, curX+placementWidth, curHeight, PUARROW_UP);
  rotUp_button->setCallback(rotUp);

  //Put +Y rotation here

  curHeight -= elementHeight;


  //curHeight = 200;
  curX = startX;

  dec_rotSide_button = new puArrowButton(curX, curHeight - elementHeight, curX+placementWidth, curHeight, PUARROW_LEFT);
  curX += (placementWidth +gap);
  //dec_rotSide_button->setLegend("-X Rotation");
  dec_rotSide_button->setCallback(rotRight);

  pause_rotSide_button = new puButton(curX, curHeight - elementHeight, curX+placementWidth, curHeight);
  curX += (placementWidth +gap);
  pause_rotSide_button->setLegend("Stop");
  pause_rotSide_button->setCallback(rotStop);
  pause_rotSide_button->setValue(0);


  inc_rotSide_button = new puArrowButton(curX, curHeight - elementHeight, curX+placementWidth, curHeight, PUARROW_RIGHT);
  curX += (placementWidth +gap);
  //inc_rotSide_button->setLegend("Faster");
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

  runtime_group->close();
  // runtime_group->hide();

  /*
     button = new puButton ( 90, 100, 90+16, 100+16, PUBUTTON_RADIO ) ;
     button->setLabelPlace ( PUPLACE_CENTERED_RIGHT ) ;
     button->setLabel ( "Label" ) ;
     button->setValue ( 1 ) ;
     button->setCallback(flipAutoScaling);
     */

}

void control_center::uncheckAutoScaling() {
  // TODO I gutted this function while making the class nonstatic. Reimplement eventually.
  // autoScale_button->setValue(0);
}

void control_center::flipAutoScaling(puObject * caller) {
  //int isAutoScaling = caller->getValue();
  bool isAutoScaling = WorldSettings::isAutoScaling();
  if (isAutoScaling) {
    WorldSettings::setAutoScaling(false);
    cout << "AutoScaling Off: " << WorldSettings::isAutoScaling() << endl;
    //caller->setValue(0);
  }
  else {
    WorldSettings::setAutoScaling(true);
    cout << "AutoScaling On: " << WorldSettings::isAutoScaling() << endl;
    //caller->setValue(1);
  }
}

void control_center::alterDT(puObject * caller) {
  cout << "Altering DT..." << endl;
  shared_ptr<Simulation> * spPointer = (shared_ptr<Simulation> *)caller->getUserData();
  shared_ptr<Simulation> curSimulation = shared_ptr<Simulation>( *spPointer );


  cout << "Alterdt.preDT: " << curSimulation->getDT() << endl;
  if (strcmp(caller->getLegend(), "Slower") == 0) {
    curSimulation->setDT(curSimulation->getDT() / 2);
    // WorldSettings::setDT(WorldSettings::getDT() / 2);
    cout << caller->getLegend() << endl;
  }

  if (strcmp(caller->getLegend(), "Faster") == 0) {
    curSimulation->setDT(curSimulation->getDT() * 2);
    // WorldSettings::setDT(WorldSettings::getDT() * 2);
  }
  cout << "Alterdt.postDT: " << curSimulation->getDT() << endl << endl;

}

void control_center::pause_cb(puObject * caller) {
  shared_ptr<Simulation> * spPointer = (shared_ptr<Simulation> *)caller->getUserData();
  shared_ptr<Simulation> curSimulation = shared_ptr<Simulation>( *spPointer );
  if (caller->getIntegerValue() == 0) {
    curSimulation->Pause();
  }
  else {
    curSimulation->unPause();
  }
}

void control_center::rotRight(puObject *) {
  Moveable * curObserver =  Observer::observers(Observer::getCurObserver());
  sgVec3 angVelocity = {0,.5,0};
  curObserver->adjustAngVelocity(angVelocity);
}

void control_center::rotLeft(puObject *) {
  Observer * curObserver =  Observer::observers(Observer::getCurObserver());
  sgVec3 angVelocity = {0,-.5,0};
  curObserver->adjustAngVelocity(angVelocity);
}

void control_center::rotUp(puObject *) {
  Observer * curObserver =  Observer::observers(Observer::getCurObserver());
  sgVec3 angVelocity = {+.5,0,0};
  curObserver->adjustAngVelocity(angVelocity);
}
void control_center::rotDown(puObject *) {
  Observer * curObserver =  Observer::observers(Observer::getCurObserver());
  sgVec3 angVelocity = {-.5,0,0};
  curObserver->adjustAngVelocity(angVelocity);
}

void control_center::rotStop(puObject *) {
  Observer * curObserver =  Observer::observers(Observer::getCurObserver());
  sgVec3 angVelocity = {0,0,0};
  curObserver->setAngVelocity(angVelocity);
}

