/*
 * main_display.h
 *
 *  Created on: Jul 19, 2011
 *      Author: brasure
 */

#ifndef MAIN_DISPLAY_H_
#define MAIN_DISPLAY_H_

#include <iostream>
#include <plib/pu.h>
#include <plib/puAux.h>
//#include "../ShapeFiles/MyShape.h"
#include "../ShapeFiles/Box.h"
#include "../ShapeFiles/Circle.h"
#include <string.h>

#include "../fileInteraction.h"

#include "../Physics/WorldSettings.h"


#include <GL/freeglut.h>

#include "control_center.h"

using boost::shared_ptr;

/*! \brief UI class that handles everything in the main window
 *
 *  Right now this only contains the Main Menu bar, but will eventually display some additional bits of information;
 *  \n potential candidates:
 *  \n -Current time
 *  \n -Current DT
 *  \n -COM position
 *  \n -Average/total {momentum, velocities, mass, etc}
 */
class main_window_UI {
private:
  boost::shared_ptr<Simulation> simulation;

	puMenuBar * main_menu;
	puaFileSelector * open_selector;
	puaFileSelector * save_selector;

	puDialogBox * dialog_box;
	puText * dialog_box_message;
	puOneShot * dialog_box_ok_button;

	puGroup * astronomicalTimeGroup;
	puText * milleniaElapsed_label;
	puInput * milleniaElapsed_value;
	puText * centuriesElapsed_label;
	puInput * centuriesElapsed_value;
	puText * decadesElapsed_label;
	puInput * decadesElapsed_value;
	puText * yearsElapsed_label;
	puInput * yearsElapsed_value;
	puText * daysElapsed_label;
	puInput * daysElapsed_value;
	puText * hoursElapsed_label;
	puInput * hoursElapsed_value;


public:
	//! Unnecessary constructor for entirely static class
	main_window_UI();

  void setSimulation( boost::shared_ptr<Simulation> simulation );

	/*! \brief Initializes values for the Main Window interface
	 *
	 *  Creates the Main Menu puMenuBar and populates it with open, save, etc. funcitnos
	 */
	void init( boost::shared_ptr<Simulation> residentSimulation );

	/*! \brief Updates the time values
	 *
	 *  Queries WorldSettings for timElapsed, then uses a series of mods
	 *  and divisions to extract the different units of time that have
	 *  elapsed. Once calculated, it sets the appropriate labels to
	 *  these values.
	 */
	void update();

	//! Opens a file picker when Open is selected in Main Menu
	static void open_cb(puObject *);

	/*! \brief Opens a data file using openShapes(char *)
	 *
	 *  Deletes current shapes using clearShapes(), Pauses the simulation, then opens file selected in open_cb(puObject *).
	 *
	 */
	static void openFile_cb(puObject *);

	//! Opens a file picker when Save is selected in Main Menu
	static void save_cb(puObject *);
	/*! \brief Saves a data file using saveShapes(char *)
	 *
	 *  Saves to file selected in open_cb(puObject *).
	 */
	static void saveFile_cb(puObject * caller);

	/*! \brief Shuts down the program
	 *
	 *  Ultimately should query user whether or not they want to save the initial/current shape list
	 */
	static void exit_cb(puObject *);

	//! Displays file being saved in saveFile_cb
	// static void mk_dialog(char *);
	//! Deletes current dialogue box and sets dialog_box to NULL
	// static void close_dialog_cb(puObject * caller);

};
#endif /* MAIN_DISPLAY_H_ */
