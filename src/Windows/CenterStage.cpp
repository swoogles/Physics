#include <graphics/FfmpegClient.h>
#include "CenterStage.h"

using namespace std;

time_t CenterStage::start = 0;

CenterStage::CenterStage() noexcept{
	/*
	main_menu = new puMenuBar();

	char      *file_submenu    [] = { "Exit" , "--------", "Save", "Open" , NULL};
	puCallback file_submenu_cb [] = { exit_cb,  NULL,      save_cb, open_cb, NULL};
	//char      *help_submenu    [] = { "About...", "Help" , NULL } ;
	//puCallback help_submenu_cb [] = { about_cb  , help_cb, NULL } ;

	main_menu->add_submenu("File", file_submenu, file_submenu_cb);

	main_menu->close();

	//open_selector->hide();

	*/
}

void CenterStage::update(double timeElapsed) {
	double intPart;
	double fracPart;
	float curDiv;


	float curVal;

	//Make initial cut down to hours
	timeElapsed /= 3600;
	modf(timeElapsed, &intPart);
	timeElapsed = intPart;

	curDiv = 24;
	timeElapsed /= curDiv;
	fracPart = modf(timeElapsed, &intPart);
	curVal = fracPart*curDiv;
	hoursElapsed_value->setValue(curVal);
	timeElapsed = intPart;


	curDiv = 365;
	timeElapsed /= curDiv;
	fracPart = modf(timeElapsed, &intPart);
	curVal = fracPart*curDiv;
	daysElapsed_value->setValue(curVal);
	timeElapsed = intPart;

	curDiv = 1000;
	timeElapsed /= curDiv;
	fracPart = modf(timeElapsed, &intPart);
	curVal = fracPart*curDiv;
	yearsElapsed_value->setValue(curVal);
	timeElapsed = intPart;

	curVal = timeElapsed;
	milleniaElapsed_value->setValue(curVal);

}

void CenterStage::open_cb(puObject * caller) {
	// open_selector = new puaFileSelector(0,0, 320, 270, 2, "/media/Media Hog/ProjectOutput/ftPhysics/", "Choose File to Open");
	// open_selector->setCallback( openFile_cb );
	// open_selector->setInitialValue("lastRun.phys");

}

void CenterStage::openFile_cb(puObject * caller) {
	// char fileName[150];
	// open_selector->getValue (fileName);

	// cout << "Filename: " << fileName << endl;

	// // Must nest interface creation/deletion, else stack will be messed up
	// puDeleteObject( open_selector );
	// open_selector = 0;

  // MyShape::clearShapes();
	// WorldSettings::Pause();
	// openShapes(fileName);

}

void CenterStage::save_cb(puObject * caller) {
	// save_selector = new puaFileSelector(0,0, 320, 270, 2, "/media/Media Hog/ProjectOutput/ftPhysics/", "Choose File to Open");
	// save_selector->setCallback( saveFile_cb );
	// save_selector->setInitialValue("lastRun.phys");

}

void CenterStage::saveFile_cb(puObject * caller) {
	// char fileName[150];
	// save_selector->getValue(fileName);

	// cout << "Save Location: " << fileName << endl;

	// puDeleteObject( save_selector );
	// save_selector = 0;

	// saveShapes(fileName);
}

void CenterStage::exit_cb(puObject * caller) {
	exit(1);
}

CenterStage::CenterStage(int windowWidth, time_t start) noexcept
		{
	CenterStage::start = start;
	main_menu = new puMenuBar();

	char      *file_submenu    [] = { "Exit" , "--------", "Save", "Open" , "Create Video & Exit", NULL};
	puCallback file_submenu_cb [] = { exit_cb,  NULL,      save_cb, open_cb, createVideoAndExit,  NULL};

	main_menu->add_submenu("File", file_submenu, file_submenu_cb);

	main_menu->close();

	int curX = windowWidth;
	int timeHeight = 5;

	int textWidth;
	int valueWidth;
	astronomicalTimeGroup = new puGroup(0,0);

	textWidth = 50;
	valueWidth = 50;
	curX -= textWidth+valueWidth;
	hoursElapsed_label = new puText(curX, timeHeight);
	hoursElapsed_label->setLabel("Hours:");
	hoursElapsed_value = new puInput(curX+textWidth, timeHeight, curX+textWidth+valueWidth, timeHeight+20);
	hoursElapsed_value->setValue("23");

	curX -= textWidth+valueWidth;
	daysElapsed_label = new puText(curX, timeHeight);
	daysElapsed_label->setLabel("Days:");
	daysElapsed_value = new puInput(curX+textWidth, timeHeight, curX+textWidth+valueWidth, timeHeight+20);
	daysElapsed_value->setValue("324");


	curX -= textWidth+valueWidth;
	yearsElapsed_label = new puText(curX, timeHeight);
	yearsElapsed_label->setLabel("Years:");
	yearsElapsed_value = new puInput(curX+textWidth, timeHeight, curX+textWidth+valueWidth, timeHeight+20);
	yearsElapsed_value->setValue("234");

	curX -= textWidth+valueWidth;
	milleniaElapsed_label = new puText(curX, timeHeight);
	milleniaElapsed_label->setLabel("Millenia:");
	milleniaElapsed_value = new puInput(curX+textWidth, timeHeight, curX+textWidth+valueWidth, timeHeight+20);
	milleniaElapsed_value->setValue("553");


	astronomicalTimeGroup->close();
	astronomicalTimeGroup->setChildColour(PUCLASS_TEXT, PUCOL_LABEL, 1.0, 1.0, 1.0);

}

void CenterStage::createVideoAndExit(puObject *) {
	FfmpegClient client;
	client.createVideo(CenterStage::start);
	exit(0);
}

// void CenterStage::mk_dialog(char * dialogText) {
// 	dialog_box = new puDialogBox(150, 50);
// 	{
// 		new puFrame( 0, 0, 400, 100);
// 		dialog_box_message = new puText (10, 70) ;
// 		dialog_box_message->setLabel(dialogText);
// 
// 		//TODO need dialog box to display correct dialog, rather than fucked up string
// 		cout << "mk_dialog text: " << dialogText << endl;
// 
// 
// 		dialog_box_ok_button = new puOneShot(180, 10, 240, 50);
// 		dialog_box_ok_button->setLegend("OK");
// 		dialog_box_ok_button->makeReturnDefault(TRUE);
// 		dialog_box_ok_button->setCallback(close_dialog_cb);
// 	}
// 	dialog_box->close();
// 	dialog_box->reveal();
// }
// 
// void CenterStage::close_dialog_cb(puObject * caller) {
// 	puDeleteObject (dialog_box);
// 	dialog_box = NULL;
// }

