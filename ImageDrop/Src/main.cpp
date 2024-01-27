#include "Image.h"

using namespace std;

int main(int argc, char* argz[]){
	Image img_out(argz[1]);
	
	if (img_out.fileRead){
		img_out.convertToCYMK();
		img_out.halftone(32);	
	}

	img_out.write("halftone.jpg");
	return 0;
}


