#ifndef MAIN_DISPLAY_H_
#define MAIN_DISPLAY_H_

#include <plib/pu.h>
#include <plib/puAux.h>
#include <chrono>
using std::time_t;

/*! \brief UI class that handles everything in the main window
 *
 *  Right now this only contains the Main Menu bar, but will eventually display some additional bits of information;
 *  \n potential candidates:
 *  \n -Current time
 *  \n -Current DT
 *  \n -COM position
 *  \n -Average/total {momentum, velocities, mass, etc}
 */
class CenterStage {
public:
	//! Unnecessary constructor for entirely static class
	CenterStage() noexcept;
	CenterStage(int windowWidth, time_t start) noexcept;

	/*! \brief Updates the time values
     *
     *  Queries WorldSettings for timElapsed, then uses a series of mods
     *  and divisions to extract the different units of time that have
     *  elapsed. Once calculated, it sets the appropriate labels to
     *  these values.
     */
	void update(double timeElapsed);

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

	static void createVideoAndExit(puObject *);
private:
    static time_t start;
	puMenuBar * main_menu;
	puaFileSelector * open_selector;
	puaFileSelector * save_selector;

	puDialogBox * dialog_box;
	puText * dialog_box_message;
	puOneShot * dialog_box_ok_button;

	puGroup * astronomicalTimeGroup;
	puText * milleniaElapsed_label;
	puInput * milleniaElapsed_value;
	puText * yearsElapsed_label;
	puInput * yearsElapsed_value;
	puText * daysElapsed_label;
	puInput * daysElapsed_value;
	puText * hoursElapsed_label;
	puInput * hoursElapsed_value;


};
#endif /* MAIN_DISPLAY_H_ */
