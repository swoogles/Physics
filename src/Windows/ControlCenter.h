#ifndef CONTROL_CENTER_H_
#define CONTROL_CENTER_H_

#include <plib/pu.h>
#include <plib/puAux.h>

#include "../Observation/Observer.h"

#include <Physics/UnitDefinitions.h>

/*! \brief Main UI for making objects and controlling simulation
 *
 *  This class provides the ability to:
 *   \n -Speedup, slowdown, or pause the simulation
 *   \n -Rotate the camera around a point
 */
class ControlCenter {
public:
	ControlCenter();
	ControlCenter(duration dt, int windowWidth);
	void init(duration dt, int windowWidth);
  inline bool isShowingRunTime() { return showingRunTime ; };
  inline void setShowingRunTime( bool showingRunTime ) { this->showingRunTime = showingRunTime; };

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

	static void toggleOctreeRendering(puObject *);

	//! Turn AutoScaling on or off
	static void flipAutoScaling(puObject * caller);

	bool isPaused() const;
	duration getDt() const;
	bool shouldRenderOctree();
private:
	static bool paused;
	static duration dt;
	duration localDt;
	static bool renderOctree ;

	puGroup * runtime_group;

	puOneShot * inc_dt_button;
	puOneShot * dec_dt_button;
	puButton * pause_dt_button;

	puArrowButton * inc_rotSide_button;
	puArrowButton * dec_rotSide_button;
	puButton * pause_rotSide_button;
	puArrowButton * rotUp_button;
	puArrowButton * rotDown_button;

	puButton * autoScale_button;
	puButton * renderOctree_button;

	bool showingRunTime;

};

#endif /* CONTROL_CENTER_H_ */
