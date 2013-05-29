/*
 * ImageOutput.h
 *
 *  Created on: Jul 28, 2011
 *      Author: brasure
 */

#ifndef IMAGEOUTPUT_H_
#define IMAGEOUTPUT_H_

#include <fstream>

struct Rect
{
	int left,top,right,bottom;
};


#if defined WIN32
#	pragma pack(push)
#	pragma pack(1)
#	define PACKED
#else
#	define PACKED __attribute__((packed))
#endif

struct TGAHeader
{
	unsigned char  identsize		PACKED;   // size of ID field that follows 18 uint8 header (0 usually)
	unsigned char  colourmaptype	PACKED;   // type of colour map 0=none, 1=has palette
	unsigned char  imagetype		PACKED;   // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

	unsigned short colourmapstart	PACKED;   // first colour map entry in palette
	unsigned short colourmaplength	PACKED;   // number of colours in palette
	unsigned char  colourmapbits	PACKED;   // number of bits per palette entry 15,16,24,32

	unsigned short xstart			PACKED;   // image x origin
	unsigned short ystart			PACKED;   // image y origin
	unsigned short width			PACKED;   // image width in pixels
	unsigned short height			PACKED;   // image height in pixels
	unsigned char  bits				PACKED;   // image bits per pixel 8,16,24,32
	unsigned char  descriptor		PACKED;   // image descriptor bits (vh flip bits)

	inline bool IsFlippedHorizontal() const
	{
		return (descriptor & 0x10) != 0;
	}

	inline bool IsFlippedVertical() const
	{
		return (descriptor & 0x20) != 0;
	}

	// pixel data follows header
};

#if WIN32
#	pragma pack(pop)
#endif
#undef PACKED

void ScreenShot( std::string fileName, unsigned int width, unsigned int height, bool includeAlpha = false )
{
	unsigned int pixelSize = 3;
	unsigned int pixelSizeBits = 24;
	GLenum pixelFormat = GL_BGR_EXT;

	if(includeAlpha)
	{
		pixelSize = sizeof(unsigned int);
		pixelSizeBits = 32;
		pixelFormat = GL_BGRA_EXT;
	}

	char* pBuffer = new char[pixelSize*width*height ];

	glReadPixels( 0,0,width,height,pixelFormat,GL_UNSIGNED_BYTE,pBuffer );

	TGAHeader tgah;
	memset( &tgah,0,sizeof(TGAHeader) );

	tgah.bits = pixelSizeBits;
	tgah.height = height;
	tgah.width = width;
	tgah.imagetype = 2;
	//tgah.xstart = 550;

	std::ofstream ofile( fileName.c_str(), std::ios_base::binary );

	cout << "FileName: " << fileName.c_str() << endl;

	ofile.write( (char*)&tgah, sizeof(tgah) );
	ofile.write( pBuffer, pixelSize*width*height );

	ofile.close();

	delete [] pBuffer;
}

#endif /* IMAGEOUTPUT_H_ */
