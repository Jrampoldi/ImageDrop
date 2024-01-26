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
	uint8_t* pixels = NULL;
	size_t size = 0;
	int w;
	int h;
	int channels;
	
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
	void checkPercentage(int loadValue);
	int index(int x, int y);
};

#endif
