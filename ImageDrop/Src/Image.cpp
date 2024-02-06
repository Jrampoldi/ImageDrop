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
	} else {
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
	fileRead = (data != NULL);
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
	

int Image::index(int x, int y){
	return (channels*(x + (y * w)));
}

Image& Image::cyanHalftoneIntensity(int resolution){
	for (int y = (resolution/2); y<h; y+=(resolution)){
		for (int x = 0; x<w; x+=(resolution)){
			
			float avgR = 0;
			float avgG = 0;
			float avgB = 0;
			
			for(int c_y = (y-(resolution/2)); c_y < (y + (resolution/2));c_y++){
				for(int c_x = (x-(resolution/2)); c_x < (x + (resolution/2)); c_x++){
					if ((index(c_x,c_y) + 2) > size || index(c_x, c_y) < 0){continue;}
					avgR += data[index(c_x, c_y)]; 
					avgG += data[index(c_x, c_y) + 1];
					avgB +=	data[index(c_x, c_y) + 2];				
				}
			}
			
			avgR = (avgR / (resolution * resolution));
			avgG = (avgG / (resolution * resolution));
			avgB = (avgB / (resolution * resolution));

			float red = avgR / 255.0;
			float green = avgG / 255.0;
			float blue = avgB / 255.0;

			float key = 1.0 - std::max(std::max(red,green), blue);

			float cyan = 0;


			if (key < 1.0 - 1e-6){

				cyan = (1.0 - red - key) / (1.0 - key);
				cyan *= 100.0;

			} else {
				/* Avoiding division by zero */
				cyan = 0.0;
			}


			float radius = ((int)((cyan/100.0) * (float)resolution)) % resolution;

			for(int c_y = (y-(resolution/2)); c_y < (y + (resolution/2));c_y++){
				for(int c_x = (x-(resolution/2)); c_x < (x + (resolution/2)); c_x++){
					if (index(c_x, c_y) > size || index(c_x, c_y) < 0){continue;}
					if  (isInCircle(c_x, x, c_y, y, radius)){
						data[index(c_x, c_y)] =  CYAN[0];
						data[index(c_x, c_y) + 1] = CYAN[1];
						data[index(c_x, c_y) + 2] = CYAN[2];
					} else {
						data[index(c_x, c_y)] = 255;
						data[index(c_x, c_y) + 1] = 255;
						data[index(c_x, c_y) + 2] = 255;
					}
				}
			}
		}
	}
	return *this;
}

Image& Image::magentaHalftoneIntensity(int resolution){
	for (int y = 0; y<h; y+=resolution){
		for (int x = (resolution/2); x<w; x+=resolution){
			
			float avgR = 0;
			float avgG = 0;
			float avgB = 0;
			
			for(int c_y = (y - (resolution/2)); c_y < (y + (resolution/2));c_y++){
				for(int c_x = (x-(resolution/2)); c_x < (x + (resolution/2)); c_x++){
					if ((index(c_x,c_y) + 2) > size || index(c_x, c_y) < 0){continue;}
					avgR += data[index(c_x, c_y)]; 
					avgG += data[index(c_x, c_y) + 1];
					avgB +=	data[index(c_x, c_y) + 2];				
				}
			}
			
			avgR = (avgR / (resolution * resolution));
			avgG = (avgG / (resolution * resolution));
			avgB = (avgB / (resolution * resolution));

			float red = avgR / 255.0;
			float green = avgG / 255.0;
			float blue = avgB / 255.0;

			float key = 1.0 - std::max(std::max(red,green), blue);


			float magenta = 0;

			if ((key < 1.0 - 1e-6)){

				magenta = (1.0 - green - key) / (1.0 - key);
				magenta *= 100.0;

			} else {
				/* Avoiding division by zero */
				magenta = 0.0;
			}		


			float radius = ((int)((magenta/100.0) * (float)resolution)) % resolution;

			for(int c_y = (y-(resolution/2)); c_y < (y + (resolution/2));c_y++){
				for(int c_x = (x-(resolution/2)); c_x < (x + (resolution/2)); c_x++){
					if (index(c_x, c_y) > size || index(c_x, c_y) < 0){continue;}				
					if  (isInCircle(c_x, x, c_y, y, radius)){
						data[index(c_x, c_y)] =  MAGENTA[0];
						data[index(c_x, c_y) + 1] = MAGENTA[1];
						data[index(c_x, c_y) + 2] = MAGENTA[2];
					} else {
						data[index(c_x, c_y)] = 255;
						data[index(c_x, c_y) + 1] = 255;
						data[index(c_x, c_y) + 2] = 255;
					}
				}
			}
		}
	}
	return *this;
}

Image& Image::yellowHalftoneIntensity(int resolution){

	for (int y = 0; y<h; y+=(resolution)){
		for (int x = 0; x<w; x+=(resolution)){
			
			float avgR = 0;
			float avgG = 0;
			float avgB = 0;
			
			for(int c_y = (y - (resolution/2)); c_y < (y + (resolution/2));c_y++){
				for(int c_x = (x - (resolution/2)); c_x < (x + (resolution/2)); c_x++){
					if ((index(c_x,c_y) + 2) > size || index(c_x, c_y) < 0){continue;}
					avgR += data[index(c_x, c_y)]; 
					avgG += data[index(c_x, c_y) + 1];
					avgB +=	data[index(c_x, c_y) + 2];				
				}
			}
			
			avgR = (avgR / (resolution * resolution));
			avgG = (avgG / (resolution * resolution));
			avgB = (avgB / (resolution * resolution));

			float red = avgR / 255.0;
			float green = avgG / 255.0;
			float blue = avgB / 255.0;

			float key = 1.0 - std::max(std::max(red,green), blue);
			float yellow = 0;


			if ((key < 1.0 - 1e-6)){
				yellow = (1.0 - blue - key) / (1.0 - key);
				yellow *= 100.0;

			} else {
				/* Avoiding division by zero */
				yellow = 0.0;
			}
		

			float radius = ((int)((yellow/100.0) * (float)resolution)) % resolution;

			for(int c_y = (y - (resolution/2)); c_y < (y + (resolution/2));c_y++){
				for(int c_x = (x - (resolution/2)); c_x < (x + (resolution/2)); c_x++){
					if (index(c_x, c_y) > size || index(c_x, c_y) < 0){continue;}
					if  (isInCircle(c_x, x, c_y, y, radius)){
						data[index(c_x, c_y)] =  YELLOW[0];
						data[index(c_x, c_y) + 1] = YELLOW[1];
						data[index(c_x, c_y) + 2] = YELLOW[2];
					} else {
						data[index(c_x, c_y)] = 255;
						data[index(c_x, c_y) + 1] = 255;
						data[index(c_x, c_y) + 2] = 255;
					}
				}
			}
		}
	}
	return *this;
}

Image& Image::keyHalftoneIntensity(int resolution){
	for (int y = (resolution/2); y<h; y+=resolution){
		for (int x = (resolution/2); x<w; x+=resolution){
			
			float avgR = 0;
			float avgG = 0;
			float avgB = 0;
			
			for(int c_y = (y-(resolution/2)); c_y < (y + (resolution/2));c_y++){
				for(int c_x = (x-(resolution/2)); c_x < (x + (resolution/2)); c_x++){
					if ((index(c_x,c_y) + 2) > size){continue;}
					avgR += data[index(c_x, c_y)]; 
					avgG += data[index(c_x, c_y) + 1];
					avgB +=	data[index(c_x, c_y) + 2];				
				}
			}
			
			avgR = (avgR / (resolution * resolution));
			avgG = (avgG / (resolution * resolution));
			avgB = (avgB / (resolution * resolution));

			float red = avgR / 255.0;
			float green = avgG / 255.0;
			float blue = avgB / 255.0;

			float key = 1.0 - std::max(std::max(red,green), blue);

			key *= 100.0;


			float radius = ((int)((key/100.0) * (float)resolution)) % resolution;
			for(int c_y = (y-(resolution/2)); c_y < (y + (resolution/2));c_y++){
				for(int c_x = (x-(resolution/2)); c_x < (x + (resolution/2)); c_x++){
					if  (isInCircle(c_x, x, c_y, y, radius)){
						data[index(c_x, c_y)] =  BLACK[0];
						data[index(c_x, c_y) + 1] = BLACK[1];
						data[index(c_x, c_y) + 2] = BLACK[2];
					} else {
						data[index(c_x, c_y)] = 255;
						data[index(c_x, c_y) + 1] = 255;
						data[index(c_x, c_y) + 2] = 255;
					}
				}
			}
		}
	}
	return *this;
}
Image& Image::convertToCMYKHalftone(const Image& cyan, const Image& magenta,
									const Image& yellow, const Image& key){
	for (int pixel = 0;pixel < size; pixel += channels){
		if (key.data[pixel] != 255){
			data[pixel] = BLACK[0];
			data[pixel + 1] = BLACK[1];
			data[pixel + 2] = BLACK[2];
		} else {
			data[pixel] = ((cyan.data[pixel] + magenta.data[pixel] + yellow.data[pixel]) / 3);
			data[pixel + 1] = ((cyan.data[pixel + 1] + magenta.data[pixel + 1] + yellow.data[pixel + 1]) / 3);	
			data[pixel + 2] = ((cyan.data[pixel + 2] + magenta.data[pixel + 2] + yellow.data[pixel + 2]) / 3);
		}
	}
	return *this;
}


bool Image::isInCircle(int x1, int x2, int y1, int y2, float radius){
	return ( radius >= sqrt(pow(x2 - x1, 2) + pow(y2-y1, 2)));

}
