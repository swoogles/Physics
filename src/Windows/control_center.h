/*
 * control_center.h
 *
 *  Created on: Jul 17, 2011
 *      Author: brasure
 */


#ifndef CONTROL_CENTER_H_
#define CONTROL_CENTER_H_



#include <iostream>
#include <plib/pu.h>
#include <plib/puAux.h>
#include <plib/sg.h>
#include <boost/make_shared.hpp> 
#include <boost/shared_ptr.hpp>
//#include "../ShapeFiles/MyShape.h"
#include "../ShapeFiles/Box.h"
#include "../ShapeFiles/Circle.h"

#include "../Physics/WorldSettings.h"
#include "../Physics/Simulation.h"

#include "../Observation/Observer.h"

#include <string>

/*! \brief Main UI for making objects and controlling simulation
 *
 *  This class provides the ability to:
 *   \n -Create new objects on the fly
 *   \n -Speedup, slowdown, or pause the simulation
 *   \n -Rotate the camera around a point
 */
class control_center {
private:
  boost::shared_ptr<Observer> observer;
  boost::shared_ptr<Simulation> simulation;
	puGroup * placement_group;
		puaSelectBox * pick_object;
		puText * object_name_label;
		puInput * object_name_input;

		puText * placement_label;
		puText * placement_label2;

		puText * momentum_label;
		puText * momentum_label2;

		puText * mass_label;
      
    puButton * clear_objects;


	puGroup * runtime_group;
		puText * speed_c_label;
		puInput * speed_c_input;

		puOneShot * inc_dt_button;
		puOneShot * dec_dt_button;
		puButton * pause_dt_button;

		puArrowButton * inc_rotSide_button;
		puArrowButton * dec_rotSide_button;
		puButton * pause_rotSide_button;
		puArrowButton * rotUp_button;
		puArrowButton * rotDown_button;

		puButton * autoScale_button;

	puButton * viewSwitcher;


	bool showingRunTime;

public:
	//! Creates all UI elements and sets their values, positions, and callbacks
  void setSimulation( boost::shared_ptr<Simulation> simulation );
	void init( boost::shared_ptr<Simulation> residentSimulation, boost::shared_ptr<Observer> observer  );
  inline bool isShowingRunTime() { return showingRunTime ; };
  inline void setShowingRunTime( bool showingRunTime ) { this->showingRunTime = showingRunTime; };
  inline void printDec_dt_buttonAddress() { std::cout << "&dec_dt_button: " << dec_dt_button << endl; };

  inline boost::shared_ptr<Observer> setObserver() { return observer ; };
  inline void setObserver( boost::shared_ptr<Observer> observer ) { this->observer = observer; };

  inline void showRunTimeGroup() { runtime_group->reveal(); };
  inline void showPlacementGroup() { placement_group->reveal(); };

  inline void hideRunTimeGroup() { runtime_group->hide(); };
  inline void hidePlacementGroup() { placement_group->hide(); };

	//! Switches from Object Creation view to View/Time manipulation view
	static void switchViewNow(puObject *);

	sgVec3 userDat;

	/*! \brief Creates a new object based on UI text boxes
	 *
	 * The values for the new object are collecting from the placement*_in, momentum*_in, and mass_in puInput boxes.
	 * Look into changing text boxes to private members with appropriate accessor/mutator functions
	 */
	// static void makeNewObject(puObject *);

	puInput * placementX_in;
	puInput * placementY_in;
	puInput * placementZ_in;

	puInput * momentumX_in;
	puInput * momentumY_in;
	puInput * momentumZ_in;

	puInput * mass_in;

	//static void alterRotSide(puObject *);

	//! Increases camera rotation around +Y axis
	static void rotRight(puObject * caller);
	//! Increases camera rotation around -Y axis
	static void rotLeft(puObject * caller);
	//! Increases camera rotation around +X axis
	static void rotUp(puObject * caller);
	//! Increases camera rotation around -X axis
	static void rotDown(puObject * caller);
	//! Stops all camera rotation
	static void rotStop(puObject * caller);

	/*! \brief Changes current timestep value for simulation
	 *
	 *  Calls WorldSettings::setDT(float) using WorldSettings::getDT() divided or multiplied by 2 depending on whether simulation is being sped up or slowed down
	 */
	static void alterDT(puObject *);
	//! Pauses simulation using WorldSettings::Pause()
	static void pause_cb(puObject *);

  static void clearShapes(puObject * caller);

	//!Turn AutoScaling off
	static void uncheckAutoScaling();

	//! Turn AutoScaling on or off
	static void flipAutoScaling(puObject * caller);
};



#endif /* CONTROL_CENTER_H_ */

