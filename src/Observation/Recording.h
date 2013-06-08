/*
 * Recording.h
 *
 *  Created on: Jul 28, 2011
 *      Author: brasure
 */

#ifndef RECORDING_H_
#define RECORDING_H_

#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <string.h>
//#include <stdlib.h>
//#include <stdio.h>

#include "jpeglib.h"

using namespace std;

void intToAsc(int iCurPic, char * sCurPic);

bool screenshot(unsigned int width, unsigned int height, char *path, int quality);


/*! \brief Handles writing of frames to a jpeg files
 *
 *  This is the class that makes video output of the simulations possible. It keeps track of the current frame in the simulation and can be told to
 *  write to a file every X frames.
 *  \n Currently always records at highest quality, might add in option for lowering quality when desired
 */
class Recorder {
private:
	static char * outFilePrefix;
	static char * outFileSuffix;
	static char * outFile;

	static int curFrame;
	static int skipFrames;
	static int curImage;

	static bool recording;

public:
	//! Sets up default values for completely static class
	static void init();
	/*! \brief Set the file base name for frames to be stored
	 *
	 *  This won't be the full filename, but rather the base that frame numbers will be appended to when writing.
	 */
	static void setPath(char * outFile);

	//! Returns the current frame number
	static int getCurFrame();
	//! Set the current frame number to newFrameNum (Useful when loading new simulation)
	static void setCurFrame(int newFrameNum);
	//! Increment current frame count (Called everytime display() is called)
	static void incCurFrame();

	//! Return number of discarded frames between saved frames
	static int getSkipFrames();
	//! Set the number of discarded frames between saved frames
	static void setSkipFrames(int );

  //! Determine if display frames are being output as images.
  static bool getRecording();
  //! Decide if display frames should be output as images.
  static void setRecording(bool recording);

	/*! \brief Return true if appropriate number of have been skipped
	 *
	 *  \return (curFrame % skipFrames == 0)
	 */
	static bool shouldCaptureThisFrame();

	/*! \brief Call whenever you want to save current frame
	 *
	 *  Pieces together the correct filename for output image, then calls screenshot()
	 */
	static bool captureThisFrame(unsigned int, unsigned int);

	/*! \brief Performs the actual writing of the image file
	 *
	 *  Steps:
	 *  \n -Sets up the jpeg structs needed for writing images
	 *  \n -Gets frame buffer information
	 *  \n -Loops through all pixels
	 *  \n -Compresses image
	 */
	friend bool screenshot(unsigned int width, unsigned int height, char * path, int quality);

};

#endif /* RECORDING_H_ */
