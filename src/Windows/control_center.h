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
//#include "../ShapeFiles/MyShape.h"
#include "../ShapeFiles/Box.h"
#include "../ShapeFiles/Circle.h"

#include "../Physics/WorldSettings.h"

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
	static puGroup * placement_group;
		static puaSelectBox * pick_object;
		static puText * object_name_label;
		static puInput * object_name_input;
		static puButton * make_object;

		static puText * placement_label;
		static puText * placement_label2;

		static puText * momentum_label;
		static puText * momentum_label2;

		static puText * mass_label;
      
      static puButton * clear_objects;


	static puGroup * runtime_group;
		static puText * speed_c_label;
		static puInput * speed_c_input;
		static puButton * filler;

		static puOneShot * inc_dt_button;
		static puOneShot * dec_dt_button;
		static puButton * pause_dt_button;

		static puArrowButton * inc_rotSide_button;
		static puArrowButton * dec_rotSide_button;
		static puButton * pause_rotSide_button;
		static puArrowButton * rotUp_button;
		static puArrowButton * rotDown_button;

		//TODO AutoScale checkbox
		static puButton * autoScale_button;

	static puButton * viewSwitcher;



public:
	//! Creates all UI elements and sets their values, positions, and callbacks
	static void init();
	static bool showingRuntime;

	//! Switches from Object Creation view to View/Time manipulation view
	static void switchViewNow(puObject *);

	static sgVec3 userDat;

	/*! \brief Creates a new object based on UI text boxes
	 *
	 * The values for the new object are collecting from the placement*_in, momentum*_in, and mass_in puInput boxes.
	 * Look into changing text boxes to private members with appropriate accessor/mutator functions
	 */
	static void makeNewObject(puObject *);

	static puInput * placementX_in;
	static puInput * placementY_in;
	static puInput * placementZ_in;

	static puInput * momentumX_in;
	static puInput * momentumY_in;
	static puInput * momentumZ_in;


	//static void alterRotSide(puObject *);

	//! Increases camera rotation around +Y axis
	static void rotRight(puObject *);
	//! Increases camera rotation around -Y axis
	static void rotLeft(puObject *);
	//! Increases camera rotation around +X axis
	static void rotUp(puObject *);
	//! Increases camera rotation around -X axis
	static void rotDown(puObject *);
	//! Stops all camera rotation
	static void rotStop(puObject *);

	/*! \brief Changes current timestep value for simulation
	 *
	 *  Calls WorldSettings::setDT(float) using WorldSettings::getDT() divided or multiplied by 2 depending on whether simulation is being sped up or slowed down
	 */
	static void alterDT(puObject *);
	//! Pauses simulation using WorldSettings::Pause()
	static void pause_cb(puObject *);

	//!Turn AutoScaling off
	static void uncheckAutoScaling();

	//! Turn AutoScaling on or off
	static void flipAutoScaling(puObject * caller);


	static puInput * mass_in;
};



#endif /* CONTROL_CENTER_H_ */

