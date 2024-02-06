#include "Image.h"
#include <stdio.h>
#include <string>
#include <cstdlib>

using namespace std;

int main(){

	return 0;
}
extern "C"{
	void processImage(const int numberCalls, const char* fileEXT, const char* image_path){

		Image img(image_path);

		std::string tempHold = std::to_string(numberCalls);
		char const* uniqID = tempHold.c_str();
		const char* mfileEXT = fileEXT;	

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
		
			const char* rmFiles = "rm *.png *.jpg";
			system(rmFiles);

			/*Write buffers*/
			char keyFile[50];
			char yellowFile[50];
			char cyanFile[50];
			char magentaFile[50];
			char finalFile[50];

			sprintf(keyFile, "key_values%s.%s", uniqID, mfileEXT);
			sprintf(yellowFile,"yellow_values%s.%s", uniqID, mfileEXT);
			sprintf(cyanFile, "cyan_values%s.%s", uniqID, mfileEXT);
			sprintf(magentaFile, "magenta_values%s.%s", uniqID, mfileEXT);
			sprintf(finalFile, "Final_IMG%s.%s", uniqID, mfileEXT);
			
			key.write(keyFile);
			yellow.write(yellowFile);
			cyan.write(cyanFile);
			magenta.write(magentaFile);
			img.write(finalFile);
		}
	}
}
