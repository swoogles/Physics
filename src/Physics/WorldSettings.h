/*
 * WorldSettings.h
 *
 *  Created on: Jul 28, 2011
 *      Author: brasure
 */

#ifndef WORLDSETTINGS_H_
#define WORLDSETTINGS_H_

#include <plib/sg.h>


/*! \brief Handles values and settings used for various physical interactions
 *
 *  This class handles values and settings that will affect the simulations in fundamental ways. It controls the flow of time, the type of collisions that will
 *  occur, and forces that will be in play.
 */
class WorldSettings {
private:
	static float DT;
	static double timeElapsed;

	static bool paused;

	static float totalMass;

	static bool allElastic;
	static bool allInelastic;

	static bool constGravField;
	static bool gravBetweenObjects;

	static sgVec4 gravField;

	static float minX, minY, maxX, maxY;

	static bool autoScaling;

public:
	//! Sets the timestep to be used for the simulation
	static void setDT(float newDT);
	//! Returns timestep currently being used
	static float getDT();

	//! Sets the total time elapsed for the simulation
	static void setTimeElapsed(double newTimeElapsed);
	//! Updates the total time elapsed for the simulation
	static void updateTimeElapsed();
	//! Returns the total time elapsed for the simulation
	static double getTimeElapsed();

	//! Pauses the simulation, freezing all objects in their current position
	static void Pause();
	//! Unpauses simulation
	static void unPause();
	//! Returns true if simulation is paused
	static bool isPaused();

	//TODO set up relavent mass functions and color-coding
	/*! \brief Alters the total mass in the system
	 *
	 * Normally used when adding/deleting objects
	 */
	static void adjustTotalMass(float dMass);
	//! Returns total amount of mass in current system
	static float getTotalMass();
	//! sets total amount of mass in current system
	static void setTotalMass(float newMass);

	//! Returns true if every collision is treated as completely elastic
	static bool isAllElastic();
	//! Returns true if every collision is treated as competely inelastic
	static bool isAllInelastic();
	/*! \brief Returns true if if isAllElastic() and isAllInelastic both return false
	 *
	 *  Indicates if collision action must be determined on a case-by-case basis
	 */
	static bool isMixedElasticity();

	//! Makes simulation treat all collisions as completely elastic
	static void makeAllElastic();
	//! Makes simulation treat all collisions as completely inelastic
	static void makeAllInelastic();
	//! Makes simulation treat collisions based on their characteristics
	static void makeMixedElasticity();

	//! Returns true if a ubiquitous force from gravity affects the entire system
	static bool isConstGravField();
	//! Set to true to enact a constant gravity vector
	static void setConstGravField(bool useGravField);
	//! Set gravity field vector to newGravField
	static void setConstGravFieldVal(sgVec4 newGravField);
	//! Return gravity field vector in retGravField
	static void getConstGravFieldVal(sgVec4 retGravField);

	//! Returns true if simulation is calculating gravity between objects
	static bool isGravBetweenObjects();
	//! Set to true to calculate gravity between objects
	static void setGravBetweenObjects(bool);

	static void resetXYMinsAndMaxes();
	static void updateXYMinsAndMaxes(sgVec4 curPos);

	static float getMinX();
	static float getMinY();
	static float getMaxX();
	static float getMaxY();

	//! Turn AutoScaling on or off
	static void setAutoScaling(bool);
	//! Returns true if simulation is currently AutoScaling
	static bool isAutoScaling();

};

#endif /* WORLDSETTINGS_H_ */
