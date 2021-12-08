#ifndef PIMP_H
#define PIMP_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef uint8_t b8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;


#pragma pack(push, 1)

typedef struct {
	u16 type;
	u32 file_size;
	u16 reserved1;
	u16 reserved2;
	u32 offset;
	u32 size;
	u32 width;
	u32 height;
	u16 planes;
	u16 bitcount;
	u32 compression;
	u32 size_image;
	u32 x_pels_per_meter;
	u32 y_pels_per_meter;
	u32 clr_used;
	u32 clr_important;
} BitmapHeader;

#pragma pack(pop)

typedef struct {
	BitmapHeader header;
	u32 width;
	u32 height;
	u32* pixels;
} Image;

void save_image(Image* image, char* file_name);
u32 get_image_size(Image* image);
Image create_image(u32 width, u32 height);
b8 bmp_write(const char *filename, u32 *pixels, i32 w, i32 h, i32 channels);
b8 bmp_write_file(FILE *fp, u32 *pixels, i32 w, i32 h, i32 channels);

#ifdef PIMP_IMPLEMENTATION

u32 get_image_size(Image* image)
{
	return image->width * image->height * sizeof(u32);
}

Image create_image(u32 width, u32 height)
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
	
	image.pixels = (u32*) malloc(get_image_size(&image));
	
	return image;
}

void save_image(Image* image, char* file_name)
{
	FILE* file = fopen(file_name, "wb");
	fwrite(&image->header, sizeof(BitmapHeader), 1, file);
	fwrite(image->pixels, get_image_size(image), 1, file);
}

b8 bmp_write(const char *filename, u32 *pixels, i32 w, i32 h, i32 channels)
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


b8 bmp_write_file(FILE *fp, u32 *pixels, i32 w, i32 h, i32 channels)
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
