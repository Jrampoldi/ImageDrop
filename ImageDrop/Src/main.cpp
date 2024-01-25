#include "Image.h"

using namespace std;

int main(int argc, char* argz[]){
	Image test("SampleImages/Lion.jpg");
	
	Image dither_img(test);
	dither_img.dither();
	dither_img.averageGroupOfPixels(64);	
	dither_img.write("dithered.jpg");
	return 0;
}


