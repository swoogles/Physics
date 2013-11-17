/*
 * Recording.cpp
 *
 *  Created on: Jul 28, 2011
 *      Author: brasure
 */

#include "Recording.h"

int Recorder::curImage;

Recorder::Recorder()
{
	outFilePrefix = new char [80];
	outFileSuffix = new char[7];
	strcpy(outFileSuffix, ".jpg");
	outFile = new char[100];
	strcpy(outFilePrefix, outFile);
	curFrame = 0;
	skipFrames = 10;
}


void Recorder::init() {


	curImage = 1;

}

void Recorder::setPath(char * outFile) {
}

int Recorder::getCurFrame() {
	return curFrame;
}

void Recorder::setCurFrame(int newFrameNum) {
	curFrame = newFrameNum;
}

void Recorder::incCurFrame() {
	curFrame++;
}

void Recorder::setSkipFrames(int inSkipFrames) {
	skipFrames = inSkipFrames;
}

int Recorder::getSkipFrames() {
	return skipFrames;
}

bool Recorder::getRecording() {
	return recording;
}

void Recorder::setRecording(bool inRecording) {
	recording = inRecording;
}

bool Recorder::shouldCaptureThisFrame() {
	return (curFrame % skipFrames == 0);
}

bool Recorder::captureThisFrame(unsigned int width, unsigned int height) {
	//cout << "Capturing Frame" << endl;
	int tempNum = curImage;
	int leadingZeros = 5;

	while (tempNum) {
		leadingZeros--;
		tempNum/=10;
	}

	char temp[50];
	intToAsc(curImage, temp);
	curImage++;

	//itoa(curFrame, temp, 10);
	strcpy(outFile, outFilePrefix);
	while (leadingZeros) {
		strcat(outFile, "0");
		leadingZeros--;
	}
	strcat(outFile, temp);
	strcat(outFile, outFileSuffix);

	return screenshot(width, height, outFile, 100);
}


bool screenshot(unsigned int width, unsigned int height, char *path, int quality)
{
   bool ret=false;

   struct jpeg_compress_struct cinfo; // the JPEG OBJECT
   struct jpeg_error_mgr jerr; // error handler struct
   unsigned char *row_pointer[1]; // pointer to JSAMPLE row[s]
   GLubyte *pixels=0, *flip=0;
   FILE *shot;
   int row_stride; // width of row in image buffer

   if((shot=fopen(path, "wb"))!=NULL) { // jpeg file
      // initializatoin
      cinfo.err = jpeg_std_error(&jerr); // error handler
      jpeg_create_compress(&cinfo); // compression object
      jpeg_stdio_dest(&cinfo, shot); // tie stdio object to JPEG object
      row_stride = width * 3;

      pixels = (GLubyte *)malloc(sizeof(GLubyte)*width*height*3);
      flip = (GLubyte *)malloc(sizeof(GLubyte)*width*height*3);

      if (pixels!=NULL && flip!=NULL) {
         // save the screen shot into the buffer
         //glReadBuffer(GL_FRONT_LEFT);
         glPixelStorei(GL_PACK_ALIGNMENT, 1);
         glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

         // give some specifications about the image to save to libjpeg
         cinfo.image_width = width;
         cinfo.image_height = height;
         cinfo.input_components = 3; // 3 for R, G, B
         cinfo.in_color_space = JCS_RGB; // type of image

         jpeg_set_defaults(&cinfo);
         jpeg_set_quality(&cinfo, quality, TRUE);
         jpeg_start_compress(&cinfo, TRUE);

         // OpenGL writes from bottom to top.
         // libjpeg goes from top to bottom.
         // flip lines.
         for (unsigned int y=0;y < height; y++) {
            for (unsigned int x=0;x < width; x++) {
               flip[(y*width+x)*3] = pixels[((height-1-y)*width+x)*3];
               flip[(y*width+x)*3+1] = pixels[((height-1-y)*width+x)*3+1];
               flip[(y*width+x)*3+2] = pixels[((height-1-y)*width+x)*3+2];
            }
         }

         // write the lines
         while (cinfo.next_scanline < cinfo.image_height) {
            row_pointer[0] = &flip[cinfo.next_scanline * row_stride];
            jpeg_write_scanlines(&cinfo, row_pointer, 1);
         }

         ret=true;
         // finish up and free resources
         jpeg_finish_compress(&cinfo);
         jpeg_destroy_compress(&cinfo);
      }
      fclose(shot);
   }

   if(pixels!=0)
      free(pixels);
   if(flip!=0)
      free(flip);

   return ret;
}

void intToAsc(int iCurPic, char * sCurPic)
{
   int i = 0;

   int tempNum = 0;

   while(iCurPic != 0)
   {
      tempNum *= 10;
      tempNum += iCurPic % 10;
      iCurPic /= 10;
      i++;
   }


   for (int j = 0; j < i; j++)
   {
      sCurPic[j] = (tempNum % 10) + 48;
      tempNum /= 10;
   }
   sCurPic[i] = '\0';

}
