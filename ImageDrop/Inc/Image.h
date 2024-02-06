#ifndef IMAGE_H_
#define IMAGE_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <cstddef>
#include <cmath>

enum ImageType{
	PNG, JPG, BMP, TGA
};

struct Image{

	/* Struct attributes */
	uint8_t* data = NULL;
	size_t size = 0;
	int w;
	int h;
	int channels;
	bool fileRead = false;

	/* Constants */
	int CYAN[3] = {0, 255, 255};
	int MAGENTA[3] = {255, 0, 255};
	int YELLOW[3] = {255, 255, 0};
	int BLACK[3] = {0, 0, 0};

	/*Constructors and Destructors*/
	Image(const char* filename);
	Image(int w, int h, int channels);
	Image(const Image&);
	~Image();

	bool read(const char* filename);
	bool write(const char* filename);

	ImageType getFileType(const char* filename);
	
	Image& grayscale_avg();
	/* Take average of each pixel in data[]
  		and set index to value.				*/

	Image& cyanHalftoneIntensity(int resolution);
	/* Take cyan percentage of a group of pixels
  		and define color circle based on intensity
		of color.						 	*/

	Image& magentaHalftoneIntensity(int resolution);
	/*Take magenta percentage of a group of pixels
  		and define color circle based on intensity
  		of color.							*/	

	Image& yellowHalftoneIntensity(int resolution);
	/*Take yellow percentage of a group of pixels
  		and define color circle based on intensity
  		of color.							*/

	Image& keyHalftoneIntensity(int resolution);
	/*Take key(black) percentage of a group of 
  		pixels and define color circle based on
  		intensity of color.					*/

	Image& convertToCMYKHalftone(const Image& cyan, const Image& magenta,
									const Image& yellow, const Image& key);
	/*Take processed CMYK halftone images and 
  		copy color data into self.			*/


	bool isInCircle(int x1, int x2, int y1, int y2, float radius);	
	int index(int x, int y);
	/*Data[] is a 1 dimensional array and
  		must use (x, y) coordinate values 	*/

};//end of struct

#endif
