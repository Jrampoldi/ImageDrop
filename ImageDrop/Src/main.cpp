#include "Image.h"
#include <iostream>
#include <cstring>

using namespace std;

int main(int argc, char** argv){

	return 0;
}
extern "C"{
	void processImage(const char* image_path){
		Image img(image_path);
	
		if (img.fileRead){
			Image key(img);
			Image yellow(img);
			Image cyan(img);
			Image magenta(img);


			key.keyHalftoneIntensity(8);
			yellow.yellowHalftoneIntensity(16);
			magenta.magentaHalftoneIntensity(16);
			cyan.cyanHalftoneIntensity(16);
			img.convertToCMYKHalftone(cyan, magenta, yellow, key);
			
			key.write("key_values.png");
			yellow.write("yellow_values.png");
			cyan.write("cyan_values.png");
			magenta.write("magenta_values.png");
			img.write("Final_IMG.png");
		}
	}
}
