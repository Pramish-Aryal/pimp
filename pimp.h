#ifndef PIMP_H
#define PIMP_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef USE_TYPEDEFS
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef uint8_t b8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;
#endif

#pragma pack(push, 1)

typedef struct {
	uint16_t type;
	uint32_t file_size;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t offset;
	uint32_t size;
	uint32_t width;
	uint32_t height;
	uint16_t planes;
	uint16_t bitcount;
	uint32_t compression;
	uint32_t size_image;
	uint32_t x_pels_per_meter;
	uint32_t y_pels_per_meter;
	uint32_t clr_used;
	uint32_t clr_important;
} BitmapHeader;

#pragma pack(pop)

typedef struct {
	BitmapHeader header;
	uint32_t width;
	uint32_t height;
	uint32_t* pixels;
} Image;

void save_image(Image* image, char* file_name);
uint32_t get_image_size(Image* image);
Image create_image(uint32_t width, uint32_t height);
uint8_t bmp_write(const char *filename, uint32_t *pixels, int32_t w, int32_t h, int32_t  channels);
uint8_t bmp_write_file(FILE *fp, uint32_t *pixels, int32_t w, int32_t h, int32_t channels);

#ifdef PIMP_IMPLEMENTATION

uint32_t get_image_size(Image* image)
{
	return image->width * image->height * sizeof(uint32_t);
}

Image create_image(uint32_t width, uint32_t height)
{
	Image image = {0};
	
	image.width = width;
	image.height = height;
	
	image.header.type = 0x4d42;
	image.header.file_size = sizeof(BitmapHeader) + get_image_size(&image);
	image.header.reserved1 = 0;
	image.header.reserved2 = 0;
	image.header.offset = sizeof(BitmapHeader);
	image.header.size = sizeof(BitmapHeader) - 14; // everything above this field in the struct 
	image.header.width = width;
	image.header.height = -height;
	image.header.planes = 1;
	image.header.bitcount = 32;
	image.header.compression = 0;
	image.header.size_image = 0;
	image.header.x_pels_per_meter = 0;
	image.header.y_pels_per_meter = 0;
	image.header.clr_used = 0;
	image.header.clr_important = 0;
	
	image.pixels = (uint32_t*) malloc(get_image_size(&image));
	
	return image;
}

void save_image(Image* image, char* file_name)
{
	FILE* file = fopen(file_name, "wb");
	fwrite(&image->header, sizeof(BitmapHeader), 1, file);
	fwrite(image->pixels, get_image_size(image), 1, file);
}

uint8_t bmp_write(const char *filename, uint32_t *pixels, int32_t w, int32_t h, int32_t channels)
{
	Image image = {0};
	
	image.header.type = 0x4d42;
	image.header.file_size = sizeof(BitmapHeader) + w * h * channels;
	image.header.reserved1 = 0;
	image.header.reserved2 = 0;
	image.header.offset = sizeof(BitmapHeader);
	image.header.size = sizeof(BitmapHeader) - 14; // everything above this field in the struct 
	image.header.width = w;
	image.header.height = -h;
	image.header.planes = 1;
	image.header.bitcount = 32;
	image.header.compression = 0;
	image.header.size_image = 0;
	image.header.x_pels_per_meter = 0;
	image.header.y_pels_per_meter = 0;
	image.header.clr_used = 0;
	image.header.clr_important = 0;
	
	FILE* fp = fopen(filename, "wb");
	if(!fp) return -1;
	if(!fwrite(&image.header, sizeof(BitmapHeader), 1, fp)) {
		fclose(fp);
		return -1;
	}
	
	if(!fwrite(pixels, w * h * channels, 1, fp)) {
		fclose(fp);
		return -1;
	}
	
	return 1;
}


uint8_t bmp_write_file(FILE *fp, uint32_t *pixels, int32_t w, int32_t h, int32_t channels)
{
	Image image = {0};
	
	image.header.type = 0x4d42;
	image.header.file_size = sizeof(BitmapHeader) + w * h * channels;
	image.header.reserved1 = 0;
	image.header.reserved2 = 0;
	image.header.offset = sizeof(BitmapHeader);
	image.header.size = sizeof(BitmapHeader) - 14; // everything above this field in the struct 
	image.header.width = w;
	image.header.height = -h;
	image.header.planes = 1;
	image.header.bitcount = 32;
	image.header.compression = 0;
	image.header.size_image = 0;
	image.header.x_pels_per_meter = 0;
	image.header.y_pels_per_meter = 0;
	image.header.clr_used = 0;
	image.header.clr_important = 0;
	
	if(!fwrite(&image.header, sizeof(BitmapHeader), 1, fp)) return -1;
	if(!fwrite(pixels, w * h * channels, 1, fp)) return -1;
	return 1;
}

#endif // PIMP_IMPLEMENTATION
#endif //PIMP_H
