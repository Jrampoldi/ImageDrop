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

	const float WHITE_SPACE_MULTIPLIER = 1;
	const int CMYK_THRESHOLD = 0;
	int CYAN[3] = {0, 255, 255};
	int MAGENTA[3] = {255, 0, 255};
	int YELLOW[3] = {255, 255, 0};
	int BLACK[3] = {0, 0, 0};
	
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
	Image& cyanHalftoneIntensity(int resolution);
	Image& magentaHalftoneIntensity(int resolution);
	Image& yellowHalftoneIntensity(int resolution);
	Image& keyHalftoneIntensity(int resolution);
	Image& convertToCMYKHalftone(const Image& cyan, const Image& magenta,
									const Image& yellow, const Image& key);

	Image& convertToCYMK(int resolution);
	bool isInCircle(int x1, int x2, int y1, int y2, float radius);	
	void checkPercentage(int loadValue);
	int index(int x, int y);
};

#endif
