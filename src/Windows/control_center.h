#ifndef CONTROL_CENTER_H_
#define CONTROL_CENTER_H_

#include <plib/pu.h>
#include <plib/puAux.h>
#include <plib/sg.h>
#include "../Dimensions/Moveable.h"

#include "../Observation/Observer.h"

/*! \brief Main UI for making objects and controlling simulation
 *
 *  This class provides the ability to:
 *   \n -Create new objects on the fly
 *   \n -Speedup, slowdown, or pause the simulation
 *   \n -Rotate the camera around a point
 */
class control_center {
private:
	static bool paused;
	static float dt;

	puGroup * placement_group;
		puText * object_name_label;
		puInput * object_name_input;

		puText * placement_label;
		puText * placement_label2;

		puText * momentum_label;
		puText * momentum_label2;

		puText * mass_label;
      
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

    puInput * placementX_in;
    puInput * placementY_in;
    puInput * placementZ_in;

    puInput * momentumX_in;
    puInput * momentumY_in;
    puInput * momentumZ_in;

    puInput * mass_in;

public:
	void init(float dt, int windowWidth, int windowHeight);
  inline bool isShowingRunTime() { return showingRunTime ; };
  inline void setShowingRunTime( bool showingRunTime ) { this->showingRunTime = showingRunTime; };

  inline void showRunTimeGroup() { runtime_group->reveal(); };
  inline void showPlacementGroup() { placement_group->reveal(); };

  inline void hideRunTimeGroup() { runtime_group->hide(); };
  inline void hidePlacementGroup() { placement_group->hide(); };

	//! Switches from Object Creation view to View/Time manipulation view
	static void switchViewNow(puObject *);

	sgVec3 userDat;

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
	 *  Calls Simulation.setDT(float) using Simulation.getDT() divided or multiplied by 2 depending on whether simulation is being sped up or slowed down
	 */
	static void alterDT(puObject *);
	//! Pauses simulation 
	static void pause_cb(puObject *);

	//!Turn AutoScaling off
	static void uncheckAutoScaling();

	//! Turn AutoScaling on or off
	static void flipAutoScaling(puObject * caller);

	bool isPaused();
	float getDt();
};

#endif /* CONTROL_CENTER_H_ */
