#ifndef BMP_H
#define BMP_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 2)

//http://www.ue.eti.pg.gda.pl/fpgalab/zadania.spartan3/zad_vga_struktura_pliku_bmp_en.html

typedef struct header {
    uint16_t signature;
    uint32_t file_size;
    uint32_t reserved;
    uint32_t data_offset;
} header_t;

typedef struct info_header{
    uint32_t size;
    uint32_t width;
    uint32_t height;

    uint16_t planes;
    uint16_t bit_count;

    uint32_t compression;
    uint32_t image_size;

    uint32_t x_pixels_per_m;
    uint32_t y_pixels_per_m;

    uint32_t colors_used;
    uint32_t important_colors;
} info_header_t;

typedef struct file_info {
    header_t header;
    info_header_t info_header;
} file_info_t;

#pragma pack(pop)

#pragma pack(push, 1)

typedef struct pixel {
  char b, g, r;
} pixel_t;

#pragma pack(pop)

typedef struct img {
    file_info_t file_info;
    pixel_t** pixels;
} img_t;

void free_img(img_t*);
int load_bmp(FILE*, img_t*);
int crop(const img_t*, img_t*, int x, int y, int w, int h);
int rotate(const img_t*, img_t*);
void save_bmp(img_t*, FILE*);

#endif