#ifndef WORLDSETTINGS_H_
#define WORLDSETTINGS_H_

/*! \brief Handles values and settings used for various physical interactions
 *
 *  This class handles values and settings that will affect the simulations in fundamental ways. It controls the flow of time, the type of collisions that will
 *  occur, and forces that will be in play.
 */
class WorldSettings {
private:
	static bool autoScaling;

public:

	//! Turn AutoScaling on or off
	static void setAutoScaling(bool);
	//! Returns true if simulation is currently AutoScaling
	static bool isAutoScaling();

};

#endif
