#include "Image.h"

using namespace std;

int main(int argc, char* argz[]){
	Image test("itachi.png");

	Image dither_img(test);
	dither_img.dither();
	dither_img.write("dithered.png");
	return 0;
}


