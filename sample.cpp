#define PIMP_IMPLEMENTATION
#include "pimp.h"
#include <string.h>

int main()
{
	Image image = create_image(1920, 1080);
	
	memset(image.pixels, 0x18, get_image_size(&image));
	
	for(int y = 300;  y < 1000; ++y) {
		for(int x = 100; x < 1900; ++x) {
			image.pixels[y * image.width + x] = 0xff0000;
		}
	}
	
	save_image(&image, "image.bmp");
	return 0;
}
