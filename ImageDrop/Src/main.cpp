#include "Image.h"

using namespace std;

int main(int argc, char* argz[]){
	Image img(argz[1]);
	
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

		key.write("key_values.jpg");
		yellow.write("yellow_values.jpg");
		cyan.write("cyan_values.jpg");
		magenta.write("magenta_values.jpg");
		img.write("Final_IMG.jpg");
	}
	return 0;
}


