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
	const float FACTOR = 1.0;
	for (int y = 0; y < h - 1; y++){
		for (int x = 1; x < w - 1; x++){
			float oldRed = data[index(x, y)];
			float oldGreen = data[index(x, y) + 1];
			float oldBlue = data[index(x, y) + 2];

			float newRed = (round((FACTOR * oldRed) / 255) * (255 / FACTOR));
			float newGreen = (round((FACTOR * oldGreen) / 255) * (255 / FACTOR));
			float newBlue = (round((FACTOR * oldBlue) / 255) * (255 / FACTOR));
		
			float quantErrorR = oldRed - newRed;
			float quantErrorG = oldGreen - newGreen;
			float quantErrorB = oldBlue - newBlue;

			data[index(x, y)] = newRed;
			data[index(x, y) + 1] = newGreen;
			data[index(x, y) + 2] = newBlue;			

			data[index(x + 1, y)] += quantErrorR * (1/16.0);
			data[index(x + 1, y) + 1] += quantErrorG * (1/16.0);
			data[index(x + 1, y) + 2] += quantErrorB * (1/16.0);
		
			data[index(x - 1, y + 1)] += quantErrorR * (1/16.0);
			data[index(x - 1, y + 1) + 1] += quantErrorG * (1/16.0);
			data[index(x - 1, y + 1) + 2] += quantErrorB * (1/16.0);
			

			data[index(x, y + 1)] += quantErrorR * (1/16.0);
			data[index(x, y + 1) + 1] += quantErrorG * (1/16.0);
			data[index(x, y + 1) + 2] += quantErrorB * (1/16.0);


			data[index(x + 1, y + 1)] += quantErrorR * (1/16.0);
			data[index(x + 1, y + 1) + 1] += quantErrorG * (1/16.0);
			data[index(x + 1, y + 1) + 2] += quantErrorB * (1/16.0);
		}
	}
	return *this;
}

Image& Image::averageGroupOfPixels(int resolution){
	int FACTOR = 1;
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

			for(int c_y = (y-(resolution/2)); c_y < (y + (resolution/2));c_y++){
				for(int c_x = (x-(resolution/2)); c_x < (x + (resolution/2)); c_x++){
					if ((index(c_x,c_y) + 2) > size){continue;}
					data[index(c_x, c_y)] = (round((FACTOR * avgR) / 255) * (255 / FACTOR));
					data[index(c_x, c_y) + 1] = (round((FACTOR * avgG) / 255) * (255 / FACTOR));
					data[index(c_x, c_y) + 2] = (round((FACTOR * avgB) / 255) * (255 / FACTOR));
				}
			}
		}
	}
	printf("Picture averaged.\n");
	return *this;
}
int Image::index(int x, int y){
	return (channels*(x + (y * w)));
}

Image& Image::cyanHalftoneIntensity(int resolution){
	for (int y = (resolution/2); y<h; y+=(resolution)){
		for (int x = (resolution/2); x<w; x+=(resolution)){
			
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

			float cyan = 0;


			if (key < 1.0 - 1e-6){

				cyan = (1.0 - red - key) / (1.0 - key);
				cyan *= 100.0;

			} else {
				/* Avoiding division by zero */
				cyan = 0.0;
			}

			if (cyan < CMYK_THRESHOLD){cyan = 0;}

			int radius = ((int)((cyan/100.0) * (float)resolution)) % resolution;

			for(int c_y = (y-(resolution/2)); c_y < (y + (resolution/2));c_y++){
				for(int c_x = (x-(resolution/2)); c_x < (x + (resolution/2)); c_x++){
					if (index(c_x, c_y) > size){continue;}
					if  (isInCircle(c_x, (x + 1), c_y, y, (radius * WHITE_SPACE_MULTIPLIER))){
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


			float magenta = 0;

			if ((key < 1.0 - 1e-6)){

				magenta = (1.0 - green - key) / (1.0 - key);
				magenta *= 100.0;

			} else {
				/* Avoiding division by zero */
				magenta = 0.0;
			}		

			if (magenta < CMYK_THRESHOLD){magenta = 0.0;}

			int radius = ((int)((magenta/100.0) * (float)resolution)) % resolution;

			for(int c_y = (y-(resolution/2)); c_y < (y + (resolution/2));c_y++){
				for(int c_x = (x-(resolution/2)); c_x < (x + (resolution/2)); c_x++){
					if (index(c_x, c_y) > size){continue;}				
					if  (isInCircle(c_x, x, c_y, (y + 1), (radius * WHITE_SPACE_MULTIPLIER))){
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

	for (int y = (resolution/2); y<h; y+=(resolution)){
		for (int x = (resolution/2); x<w; x+=(resolution)){
			
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
			float yellow = 0;


			if ((key < 1.0 - 1e-6)){
				yellow = (1.0 - blue - key) / (1.0 - key);
				yellow *= 100.0;

			} else {
				/* Avoiding division by zero */
				yellow = 0.0;
			}
		
			/*Filter lower values out*/
			if (yellow < CMYK_THRESHOLD){yellow = 0;}

			int radius = ((int)((yellow/100.0) * (float)resolution)) % resolution;

			for(int c_y = (y-(resolution/2)); c_y < (y + (resolution/2));c_y++){
				for(int c_x = (x-(resolution/2)); c_x < (x + (resolution/2)); c_x++){
					if (index(c_x, c_y) > size){continue;}
					if  (isInCircle(c_x, (x + 1), c_y, (y + 1), (radius * WHITE_SPACE_MULTIPLIER))){
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

			if (key < CMYK_THRESHOLD){key = 0;}

			int radius = ((int)((key/100.0) * (float)resolution)) % resolution;

			for(int c_y = (y-(resolution/2)); c_y < (y + (resolution/2));c_y++){
				for(int c_x = (x-(resolution/2)); c_x < (x + (resolution/2)); c_x++){
					if  (isInCircle(c_x, x, c_y, y, (radius * WHITE_SPACE_MULTIPLIER))){
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

Image& Image::halftone(int resolution){
	

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


			int greyValue = (avgR + avgG + avgB) / 3;

			int radius = ((int)((greyValue/255.0) * (float)resolution)) % resolution;

			for(int c_y = (y-(resolution/2)); c_y < (y + (resolution/2));c_y++){
				for(int c_x = (x-(resolution/2)); c_x < (x + (resolution/2)); c_x++){
					if  (isInCircle(c_x, x, c_y, y, radius)){
						data[index(c_x, c_y)] =  avgR;
						data[index(c_x, c_y) + 1] = avgG;
						data[index(c_x, c_y) + 2] = avgB;
					} else {
						data[index(c_x, c_y)] = 0;
						data[index(c_x, c_y) + 1] = 0;
						data[index(c_x, c_y) + 2] = 0;
					}
				}
			}
		}
	}

	return *this;
}

Image& Image::convertToCYMK(int resolution){
	for(int y = (resolution/2); y<h; y+=resolution){	
		for(int x = (resolution/2); x<w; x+=resolution){
			float avgR = 0;
			float avgG = 0;
			float avgB = 0;

			for(int c_y = (y-(resolution/2)); c_y < (y + (resolution/2));c_y++){
				for(int c_x = (x-(resolution/2)); c_x < (x + (resolution/2)); c_x++){
					avgR += data[index(c_x, c_y)]; 
					avgG += data[index(c_x, c_y) + 1];
					avgB +=	data[index(c_x, c_y) + 2];
				}
			}

			avgR = avgR/3.0;
			avgG = avgG/3.0;
			avgB = avgB/3.0;

			float red = avgR / 255.0;
			float green = avgG / 255.0;
			float blue = avgB / 255.0;

			float key = 1.0 - std::max(std::max(red,green), blue);

			float cyan = 0;
			float magenta = 0;
			float yellow = 0;


			if (key < 1.0 - 1e-6){

				cyan = (1.0 - red - key) / (1.0 - key);
				magenta = (1.0 - green - key) / (1.0 - key);
				yellow = (1.0 - blue - key) / (1.0 - key);

				cyan *= 100.0;
				magenta *= 100.0;
				yellow *= 100.0;
				key *= 100.0;

			} else {
				/* Avoiding division by zero */
				cyan = 0.0;
				magenta = 0.0;
				yellow = 0.0;
				key = 100.0;
			}
			

			float max_color = std::max(std::max(std::max(cyan, magenta), yellow), key);	

			for(int c_y = (y-(resolution/2)); c_y < (y + (resolution/2));c_y++){
				for(int c_x = (x-(resolution/2)); c_x < (x + (resolution/2)); c_x++){
					if (cyan == max_color){
						data[index(c_x, c_y)] = CYAN[0];
						data[index(c_x, c_y) + 1] = CYAN[1];
						data[index(c_x, c_y) + 2] = CYAN[2];
					} else if (magenta == max_color) {
							data[index(c_x, c_y)] = MAGENTA[0];
						data[index(c_x, c_y) + 1] = MAGENTA[1];
						data[index(c_x, c_y) + 2] = MAGENTA[2];				
					} else if(yellow == max_color) {
							data[index(c_x, c_y)] = YELLOW[0];
						data[index(c_x, c_y) + 1] = YELLOW[1];
						data[index(c_x, c_y) + 2] = YELLOW[2];				
					} else {
						data[index(c_x, c_y)] = BLACK[0];
						data[index(c_x, c_y) + 1] = BLACK[1];
						data[index(c_x, c_y) + 2] = BLACK[2];				
					}
		
				}
			}		
					
		}
	}
	return *this;
}


bool Image::isInCircle(int x1, int x2, int y1, int y2, float radius){
	return ( radius >= sqrt(pow(x2 - x1, 2) + pow(y2-y1, 2)));

}
