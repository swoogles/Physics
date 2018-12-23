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
#include <string>
#include <sstream>
#include <fstream>
#include <string.h>
//#include <stdlib.h>
//#include <stdio.h>
#include <Magick++.h>

#include "jpeglib.h"

using namespace std;

void intToAsc(int iCurPic, char * sCurPic);

bool screenshot(unsigned int width, unsigned int height, const string path, int quality);

using namespace Magick;
// using Magick::Image;
// using Magick::Geometry;

/*! \brief Handles writing of frames to a jpeg files
 *
 *  This is the class that makes video output of the simulations possible. It keeps track of the current frame in the simulation and can be told to
 *  write to a file every X frames.
 *  \n Currently always records at highest quality, might add in option for lowering quality when desired
 */
class Recorder {
private:
	char * outFilePrefix;
	char * outFileSuffix;
	char * outFile;

	string path;
	string outFileName;
	string extension;

	int skipFrames;
	int curFrame;
	int curImage;

	bool recording;

public:
  Recorder();

	//! Increment current frame count (Called everytime display() is called)
	void incCurFrame();

  //! Decide if display frames should be output as images.
  void setRecording(bool recording);

	/*! \brief Return true if appropriate number of have been skipped
	 *
	 *  \return (curFrame % skipFrames == 0)
	 */
	bool shouldCaptureThisFrame();

	/*! \brief Call whenever you want to save current frame
	 *
	 *  Pieces together the correct filename for output image, then calls screenshot()
	 */
	bool captureThisFrame(unsigned int, unsigned int);

	/*! \brief Performs the actual writing of the image file
	 *
	 *  Steps:
	 *  \n -Sets up the jpeg structs needed for writing images
	 *  \n -Gets frame buffer information
	 *  \n -Loops through all pixels
	 *  \n -Compresses image
	 */
	friend bool screenshot(unsigned int width, unsigned int height, const string path, int quality);

};

#endif /* RECORDING_H_ */
