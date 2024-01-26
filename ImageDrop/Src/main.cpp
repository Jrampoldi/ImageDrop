#include "Image.h"

using namespace std;

int main(int argc, char* argz[]){
	Image test("SampleImages/cat.jpg");
	
	Image img_out(test);

	img_out.averageGroupOfPixels(2);		
	img_out.averageGroupOfPixels(4);
	img_out.averageGroupOfPixels(8);

	img_out.write("dithered.jpg");
	return 0;
}


