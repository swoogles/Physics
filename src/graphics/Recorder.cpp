#include "Recorder.h"

Recorder::Recorder(const std::time_t  start)
          :start(start)
          ,curImage(1)
{
    this->extension = "jpg";
    this->path = "output";
    this->outFileName = "outFrame";
}

bool Recorder::captureThisFrame(unsigned int width, unsigned int height) {
	int tempNum = curImage;
	int leadingZeros = 5;

	while (tempNum) {
		leadingZeros--;
		tempNum/=10;
	}

  ostringstream convert;   // stream used for the conversion
  convert << curImage;      // insert the textual representation of 'Number' in the characters in the stream
  string numString = convert.str(); // set 'Result' to the contents of the stream

	curImage++;

  string paddingZeros;
	while (leadingZeros) {
    paddingZeros+="0";
		leadingZeros--;
	}

  string outFileString = path + "/" + outFileName + paddingZeros + numString + "." + this->extension;

	return screenshot(width, height, outFileString, 100);
}

bool Recorder::screenshot(unsigned int width, unsigned int height, string path, int quality)
{
   bool ret=false;

   struct jpeg_compress_struct cinfo; // the JPEG OBJECT
   struct jpeg_error_mgr jerr; // error handler struct
   unsigned char *row_pointer[1]; // pointer to JSAMPLE row[s]
   GLubyte *pixels=0, *flip=0;
   FILE *shot;
   int row_stride; // width of row in image buffer

   if((shot=fopen(path.c_str(), "wb"))!=NULL) { // jpeg file
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

         int numComponents = 3; // 3 for R, G, Bs
         // give some specifications about the image to save to libjpeg
         cinfo.image_width = width;
         cinfo.image_height = height;
         cinfo.input_components = numComponents; // 3 for R, G, B
         cinfo.in_color_space = JCS_RGB; // type of image

         jpeg_set_defaults(&cinfo);
         jpeg_set_quality(&cinfo, quality, TRUE);
         jpeg_start_compress(&cinfo, TRUE);

         // OpenGL writes from bottom to top.
         // libjpeg goes from top to bottom.
         // flip lines.
         for (unsigned int y=0;y < height; y++) {
            for (unsigned int x=0;x < width; x++) {
               flip[(y*width+x)*numComponents] = pixels[((height-1-y)*width+x)*numComponents];
               flip[(y*width+x)*numComponents+1] = pixels[((height-1-y)*width+x)*numComponents+1];
               flip[(y*width+x)*numComponents+2] = pixels[((height-1-y)*width+x)*numComponents+2];
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

void Recorder::createVideo() {
   std::ostringstream stream;
   stream << std::put_time(std::localtime(&this->start), "%F %T");
   string ffmpegCommandAndOptions = "ffmpeg  -i ./output/outFrame%05d.jpg -framerate 1 -c:v libx264 -crf 18 -pix_fmt yuv420p \"";
   string outDirectory = "./WorthyVideos/";
   string outVideoFormat = ".mp4\"";
   string fullCommand = ffmpegCommandAndOptions + outDirectory + stream.str() + outVideoFormat;
   redi::ipstream in(fullCommand);
   std::string ffmpegOutput;
   while (in >> ffmpegOutput) { /* Do nothing */ }
   redi::ipstream cleanup("rm -f ./output/*");

}
