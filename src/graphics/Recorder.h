#ifndef RECORDING_H_
#define RECORDING_H_

#include <GL/glut.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <lib/pstream.h>
#include <chrono>

#include "jpeglib.h"

#include "FfmpegClient.h"

using namespace std;

/*! \brief Handles writing of frames to a jpeg files
 *
 *  This is the class that makes video output of the simulations possible. It keeps track of the current frame in the simulation and can be told to
 *  write to a file every X frames.
 *  \n Currently always records at highest quality, might add in option for lowering quality when desired
 */
class Recorder {
public:
	Recorder();

	/*! \brief Call whenever you want to save current frame
	 *
	 *  Pieces together the correct filename for output image, then calls screenshot()
	 */
	bool captureThisFrame(unsigned int, unsigned int);

private:
	string path;
	string outFileName;
	string extension;

	int curImage;

    /*! \brief Performs the actual writing of the image file
     *
     *  Steps:
     *  \n -Sets up the jpeg structs needed for writing images
     *  \n -Gets frame buffer information
     *  \n -Loops through all pixels
     *  \n -Compresses image
     */
    bool screenshot(unsigned int width, unsigned int height, const string path, int quality);
};

#endif
