#ifndef IMAGE_H_
#define IMAGE_H_

#include <stdint.h>



struct Image{
	uint8_t* imageData = NULL;
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


	
};

#endif
