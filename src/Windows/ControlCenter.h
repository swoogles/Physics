#ifndef CONTROL_CENTER_H_
#define CONTROL_CENTER_H_

#include <queue>
#include <optional>

#include <plib/pu.h>
#include <plib/puAux.h>

#include "../Input/CameraAction.h"

#include <Physics/UnitDefinitions.h>
#include <graphics/FfmpegClient.h>

using std::queue;
using std::optional;

using std::chrono::time_point;
using std::chrono::duration;
using std::chrono::system_clock;


/*! \brief Main UI for making objects and controlling simulation
 *
 *  This class provides the ability to:
 *   \n -Speedup, slowdown, or pause the simulation
 *   \n -Rotate the camera around a point
 */
class ControlCenter {
public:
	ControlCenter(local_duration dt, int windowWidth, time_point<system_clock, duration<long, ratio<1, 1000000000>>> start);

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

    static void createVideoCallback(puObject *);
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
	local_duration getDt() const;
	bool shouldRenderOctree();
	static optional<CameraAction> currentCameraAction();
    static void submitCameraAction(CameraAction cameraAction);
private:
	static queue<CameraAction> cameraActions;
	static bool paused;
	static local_duration dt;
	local_duration localDt;
	static bool renderOctree ;
    time_point<system_clock, duration<long, ratio<1, 1000000000>>> start;
    static time_point<system_clock, duration<long, ratio<1, 1000000000>>> static_start;

    puGroup * runtime_group;

    puOneShot * createVideo;
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
