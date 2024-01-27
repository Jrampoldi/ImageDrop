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
	uint8_t* data = NULL;
	size_t size = 0;
	int w;
	int h;
	int channels;
	bool fileRead = false;

	int CYAN[] = {0, 255, 255};
	int MAGENTA[] = {255, 0, 255};
	int YELLOW[] = {255, 255, 0}

	
	Image(const char* filename);
	Image(int w, int h, int channels);
	Image(const Image&);
	~Image();

	bool read(const char* filename);
	bool write(const char* filename);

	ImageType getFileType(const char* filename);
	
	Image& grayscale_avg();
	Image& grayscale_lum();
	Image& dither();
	Image& averageGroupOfPixels(int threshold);
	Image& halftone(int dotSize);


	void convertToCYMK();
	bool isInCircle(int x1, int x2, int y1, int y2, int radius);	
	void checkPercentage(int loadValue);
	int index(int x, int y);
};

#endif
