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
	static puMenuBar * main_menu;
	static puaFileSelector * open_selector;
	static puaFileSelector * save_selector;

	static puDialogBox * dialog_box;
	static puText * dialog_box_message;
	static puOneShot * dialog_box_ok_button;

	static puGroup * astronomicalTimeGroup;
	static puText * milleniaElapsed_label;
	static puInput * milleniaElapsed_value;
	static puText * centuriesElapsed_label;
	static puInput * centuriesElapsed_value;
	static puText * decadesElapsed_label;
	static puInput * decadesElapsed_value;
	static puText * yearsElapsed_label;
	static puInput * yearsElapsed_value;
	static puText * daysElapsed_label;
	static puInput * daysElapsed_value;
	static puText * hoursElapsed_label;
	static puInput * hoursElapsed_value;


public:
	//! Unnecessary constructor for entirely static class
	main_window_UI();

	/*! \brief Initializes values for the Main Window interface
	 *
	 *  Creates the Main Menu puMenuBar and populates it with open, save, etc. funcitnos
	 */
	static void init();

	/*! \brief Updates the time values
	 *
	 *  Queries WorldSettings for timElapsed, then uses a series of mods
	 *  and divisions to extract the different units of time that have
	 *  elapsed. Once calculated, it sets the appropriate labels to
	 *  these values.
	 */
	static void update();

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
	static void mk_dialog(char *);
	//! Deletes current dialogue box and sets dialog_box to NULL
	static void close_dialog_cb(puObject * caller);

};


#endif /* MAIN_DISPLAY_H_ */
