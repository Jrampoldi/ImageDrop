#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "Image.h"
#include "stb_image.h"
#include "stb_image_write.h"


Image::Image(const char* filename){
	//constructor
	if(read(filename)) {
		printf("Read %s\n", filename);
		size = w * h * channels;	
}
	else {
		printf("Could not read %s\n", filename);
	}
}

Image::Image(int w, int h, int channels): w(w), h(h), channels(channels){
	//constructor
	size = w*h*channels;
	data = new uint8_t[size];
}

Image::Image(const Image& img): Image(img.w, img.h, img.channels){
	//copy constructor
	memcpy(data, img.data, img.size);
}

Image::~Image(){
	//destructor
	stbi_image_free(data);
}

bool Image::read(const char* filename){
	//read
	data = stbi_load(filename, &w, &h, &channels, 0);
	return data != NULL;
}

bool Image::write(const char* filename){
	//write image
	ImageType type = getFileType(filename);
	int success;
	switch(type){
		case PNG:
			success = stbi_write_png(filename, w, h, channels, data, w*channels);
			break;
		case BMP:	
			success = stbi_write_bmp(filename, w, h, channels, data);
			break;
		case JPG:
			success = stbi_write_jpg(filename, w, h, channels, data, 100);
		case TGA:
			success = stbi_write_tga(filename, w, h, channels, data);
			break;
	}
	return success != 0;
}

ImageType Image::getFileType(const char* filename){
	const char* ext = strchr(filename, '.');
	if (ext != nullptr){
		if (strcmp(ext, ".png") == 0){
			return PNG;		
		}
		else if (strcmp(ext, ".bmp") == 0){
			return BMP;		
		}
		else if (strcmp(ext, ".jpg") == 0){
			return JPG;		
		}
		else if (strcmp(ext, ".tga") == 0){
			return TGA;		
		}
	}
	return PNG;
}

Image& Image::grayscale_avg(){
	// r + g + b / 3
	if (channels < 3){
		printf("Values are less than 3, already greyscale");
	} else {
		for (int i = 0; i < size; i += channels){
			int gray = (data[i] + data[i + 1] + data[i + 2]) / 3;
			memset(data + i, gray, 3);
		}
	}
	return *this;
}
	

Image& Image::grayscale_lum(){
		if (channels < 3){
		printf("Values are less than 3, already greyscale");
	} else {
		for (int i = 0; i < size; i += channels){
			int gray = ((0.2126*data[i]) + (0.7152*data[i + 1]) + (0.0722 * data[i + 2])) / 3;
			memset(data + i, gray, 3);
		}
	}
	return *this;
}

Image& Image::dither(){
	//round(color * factor/ 255) * (255/factor)
	const int FACTOR = 1;
	for (int y = 0; y < h - 1; y++){
		for (int x = 1; x < w - 1; x++){
			float oldRed = data[index(x, y)];
			float oldGreen = data[index(x, y) + 1];
			float oldBlue = data[index(x, y) + 2];

			oldRed = (round((FACTOR * oldRed) / 255) * (255/FACTOR));
			oldGreen = (round((FACTOR * oldGreen) / 255) * (255/FACTOR));
			oldBlue = (round((FACTOR * oldBlue) / 255) * (255/FACTOR));
		

			data[index(x, y)] = oldRed;
			data[index(x, y) + 1] = oldGreen;
			data[index(x, y) + 2] = oldBlue;
		}
	}
	printf("Size: %lu",size);
	return *this;
}

int Image::index(int x, int y){
	return (x + (y * (w/channels)));
}
