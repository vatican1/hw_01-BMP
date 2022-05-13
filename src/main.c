#include <stdio.h>
#include <string.h>
#include "bmp.h"


int main(int argc, char** argv){

    if (argc < 2) return -1;
    if (strcmp(argv[1], "crop-rotate") == 0){
        if (argc != 8 )return -1;
        
        FILE* bmp_load = fopen(argv[2], "rb");
        if ( bmp_load == NULL) return -1;
    

        int x, y, w, h;
        x = atoi(argv[4]);
        y = atoi(argv[5]);
        w = atoi(argv[6]);
        h = atoi(argv[7]);

        img_t img;
        if (load_bmp(bmp_load, &img) == -1) return -1;

        if(w < 0 || h < 0 || x < 0 || y < 0 || x + w > (int) img.file_info.info_header.width || y + h > (int) img.file_info.info_header.height){
            fclose(bmp_load);
            free_img(&img);
            return -1;
        }
        FILE* bmp_save = fopen(argv[3], "wb");
        if (bmp_save == NULL) {
            fclose(bmp_load);
            return -1;
        }

        img_t img_crop;
        if (crop(&img, &img_crop, x, y ,w, h) == -1){
            fclose(bmp_load);
            fclose(bmp_save);
            free_img(&img);
            free_img(&img_crop);
            return -1;
        }
        img_t img_rotate;
        if (rotate(&img_crop, &img_rotate) == -1){
            fclose(bmp_load);
            fclose(bmp_save);
            free_img(&img);
            free_img(&img_crop);
            free_img(&img_rotate);
            return -1;
        }

        save_bmp(&img_rotate, bmp_save);
        free_img(&img);
        free_img(&img_crop);
        free_img(&img_rotate);

        fclose(bmp_load);
        fclose(bmp_save);
        return 0;
    }
    return -1;
}