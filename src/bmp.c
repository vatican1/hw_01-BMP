#include "bmp.h"

#define im_wight file_info.info_header.width 
#define im_height file_info.info_header.height

const int pixel_size = 3;

void free_img(img_t* img){
    free(img->pixels[0]);
    free(img->pixels);
}


int get_shift(img_t* img){
    return (4 - (img->im_wight * pixel_size) % 4) % 4;
}

int init_pixels(img_t* img) {
  img->pixels = malloc(sizeof(pixel_t*) * img->im_height);
  if (img->pixels == NULL)
    return -1;
  
  pixel_t* buffer = malloc(sizeof(pixel_t) * img->im_wight * img->im_height);
  if (buffer == NULL) {
    free (img->pixels);
    return -1;
  }

  for (int i = 0; i < (int) img->im_height; i++) {
    img->pixels[i] = buffer + i * img->im_wight;
  }

  return 0;
}


int load_bmp(FILE* file, img_t* img){
    fread(&img->file_info, sizeof(file_info_t), 1, file);

    fseek(file, img->file_info.header.data_offset, SEEK_SET);

    if(init_pixels(img) == -1) return -1;

    for (int i = img->im_height-1; i >= 0; i--){
        fread(img->pixels[i], sizeof(pixel_t), img->im_wight, file);
        fseek(file, get_shift(img), SEEK_CUR);
    }
    return 0;
}

void recalc_size(img_t* img, int w, int h){
    img->im_wight = w;
    img->im_height = h;
    int size = (w * pixel_size + get_shift(img)) * h;
    img->file_info.info_header.image_size = size;
    img->file_info.header.file_size = img->file_info.info_header.image_size + sizeof(file_info_t);
}

int crop(const img_t* orig_img, img_t* crop_img, int x, int y, int w, int h){
    crop_img->file_info = orig_img->file_info;
    recalc_size(crop_img, w, h);
    
    if (init_pixels(crop_img) == -1) return -1;

    for(int i = 0 ; i < h; i++){
        for(int j = 0 ; j < w; j++){
            crop_img->pixels[i][j] = orig_img->pixels[y + i][x + j];
        }
    }

    return 0;
}

int rotate(const img_t* img, img_t* rotate_img){
    rotate_img->file_info = img->file_info;
    recalc_size(rotate_img, img->im_height, img->im_wight);

    if(init_pixels(rotate_img) == -1) return -1;

    for( int i = 0; i < (int) rotate_img->im_height; i++){
        for( int j = 0; j < (int) rotate_img->im_wight; j++){
            rotate_img->pixels[i][j] = img->pixels[img->im_height - 1 - j][i];
        }
    }
    return 0;
}

void save_bmp(img_t* img,  FILE* file){
    fseek(file, 0 , SEEK_SET);
    fwrite(&img->file_info, sizeof(file_info_t), 1, file);

    fseek(file, img->file_info.header.data_offset, SEEK_SET);

    int shift = get_shift(img);
    for( int  i = img->im_height - 1 ; i >= 0; i--){
        fwrite(img->pixels[i], sizeof(pixel_t), img->im_wight, file);
        for (int j = 0; j < shift; j++){
            putc('\0', file);
        }
    }
}