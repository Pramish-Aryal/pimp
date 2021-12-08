#define PIMP_IMPLEMENTATION
#include "pimp.h"
#include <string.h>
#include <math.h>

void draw_circle(Image* image, int x0, int y0, int r, unsigned int color) {
	for(int y = y0 - r; y < y0 + r; ++y) {
		for(int x = x0 - r; x < x0 + r; ++x) {
			if((x - x0) * (x - x0) + (y - y0) * (y - y0) < r * r)
				if(x >= 0 && x < image->width && y >= 0 && y < image->height)
				image->pixels[y * image->width + x] = color;
		}
	}
}

void draw_squircle(Image* image, int x0, int y0, int r, float n, unsigned int color) {
	for(int y = y0 - r; y < y0 + r; ++y) {
		for(int x = x0 - r; x < x0 + r; ++x) {
			if(pow((x - x0), n) + pow((y - y0), n) < pow(r, n))
				if(x >= 0 && x < image->width && y >= 0 && y < image->height)
				image->pixels[y * image->width + x] = color;
		}
	}
}

float lerp(float a, float b, float t) {
	return a + (b - a) * t;
}

void draw_line(Image* image, int x0, int y0, int x1, int y1, unsigned int color) {
	for(float t = 0; t < 1.0f; t += 0.00001f) {
		int x = (int)lerp(x0, x1, t);
		int y = (int)lerp(y0, y1, t);
		if(x >= 0 && x < image->width && y >= 0 && y < image->height)
			image->pixels[y * image->width + x] = color;
	}
}

int main()
{
	Image image = create_image(1920, 1080);
	memset(image.pixels, 0x18, get_image_size(&image));
	draw_circle(&image, 600, 600, 100, 0xed4245);
	draw_circle(&image, 1200, 300, 100, 0x402245);
	draw_circle(&image, 100, 800, 300, 0x1ee345);
	draw_circle(&image, 300, 760, 40, 0xed4245);
	draw_squircle(&image, 950, 500, 400, 4, 0xffffff);
	draw_line(&image, 0, 0, 1920, 1080, 0xff9900);
	save_image(&image, "image.bmp");
	
	if(bmp_write("test.bmp", image.pixels, image.width, image.height, 4)) 
		printf("success\n");
	else 
		printf("failed\n");
	
	FILE* fp = fopen("test2.bmp", "wb");
	if(bmp_write_file(fp, image.pixels, image.width, image.height, 4)) 
		printf("success\n");
	else 
		printf("failed\n");
	
	return 0;
}