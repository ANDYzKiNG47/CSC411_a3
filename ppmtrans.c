#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"

static A2Methods_T methods;
typedef A2Methods_Array2 A2;

void rotate90(int j, int i, A2 a, void* elem, void* cl);

void rotate180(int j, int i, A2 a, void* elem, void* cl);

void rotate270(int j, int i, A2 a, void* elem, void* cl);

void flipHorizontal(int j, int i, A2 a, void* elem, void* cl);

void flipVertical(int j, int i, A2 a, void* elem, void* cl);

void transpose(int j, int i, A2 a, void* elem, void* cl);

Pnm_ppm new_Pnm(unsigned w, unsigned h, unsigned d, int size, int blocksize, A2Methods_T methods);

int main(int argc, char *argv[]) {
    int rotation = 0;
    char transformation = '\0';
    methods = array2_methods_plain; // default to UArray2 methods
    assert(methods);
    A2Methods_mapfun *map = methods->map_default; // default to best map
    assert(map);
    #define SET_METHODS(METHODS, MAP, WHAT) do { \
        methods = (METHODS); \
        assert(methods); \
        map = methods->MAP; \
        if (!map) { \
            fprintf(stderr, "%s does not support " WHAT "mapping\n", argv[0]); \
            exit(1); \
        } \
    } while(0)

    int i;
    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-row-major")) {
            SET_METHODS(array2_methods_plain, map_row_major, "row-major");
        } 
        else if (!strcmp(argv[i], "-col-major")) {
            SET_METHODS(array2_methods_plain, map_col_major, "column-major");
        } 
        else if (!strcmp(argv[i], "-block-major")) {
            SET_METHODS(array2_methods_blocked, map_block_major, "block-major");
        } 
        else if (!strcmp(argv[i], "-rotate")) {
            assert(i + 1 < argc);
            char *endptr;
            rotation = strtol(argv[++i], &endptr, 10);
            assert(*endptr == '\0'); // parsed all correctly
            assert(rotation == 0   || rotation == 90
                || rotation == 180 || rotation == 270);
        }
        else if (!strcmp(argv[i], "-transpose")){
            rotation = -1;
            transformation = 't';
        }
        else if (!strcmp(argv[i], "-flip")){
            rotation = -1;
            transformation = argv[i+1][0];
            assert (transformation == 'v' || transformation == 'h');
        } 
        else if (*argv[i] == '-') {
            fprintf(stderr, "%s: unknown option '%s'\n", argv[0], argv[i]);
            exit(1);
        } 
        else if (argc - i > 2) {
            fprintf(stderr, "Usage: %s [-rotate <angle>] "
                "[-{row,col,block}-major] [filename]\n", argv[0]);
            exit(1);
        } else {
            break;
        }
    }
    Pnm_ppm img = Pnm_ppmread(stdin, methods);
    unsigned w = img->width;
    unsigned h = img->height;
    unsigned d = img->denominator; 
    int s = sizeof(struct Pnm_rgb);
    int bs = img->methods->blocksize(img->pixels);
    Pnm_ppm img_copy;
    switch(rotation){
        case 0:
            Pnm_ppmwrite(stdout, img);
            Pnm_ppmfree(&img);
            return 0;
        case 90:
            img_copy = new_Pnm(h, w, d, s, bs, methods);
            map(img->pixels, rotate90, img_copy);
            break;

        case 180:
            img_copy = new_Pnm(w, h, d, s, bs, methods);
            map(img->pixels, rotate180, img_copy);
            break;

        case 270:
            img_copy = new_Pnm(h, w, d, s, bs, methods);
            map(img->pixels, rotate270, img_copy);
            break;

        default:
            if (transformation == 't'){
                img_copy = new_Pnm(h, w, d, s, bs, methods);
                map(img->pixels, transpose, img_copy);
            }
            else if (transformation == 'v'){
                img_copy = new_Pnm(w, h, d, s, bs, methods);
                map(img->pixels, flipVertical, img_copy);
            }
            else if (transformation == 'h'){
                img_copy = new_Pnm(w, h, d, s, bs, methods);
                map(img->pixels, flipHorizontal, img_copy);
            }
            break;
    }

    assert (img_copy != NULL);
    Pnm_ppmwrite(stdout, img_copy);
    Pnm_ppmfree(&img_copy);
    Pnm_ppmfree(&img);

    return 0;
}

void rotate90(int j, int i, A2 a, void* elem, void* cl){
    Pnm_ppm copy = (Pnm_ppm) cl;
    int new_i = j;
    int new_j = methods->height(a) - i - 1;
    Pnm_rgb new_p = methods->at(copy->pixels, new_j, new_i);
    *new_p = *((Pnm_rgb) elem);
}

void rotate180(int j, int i, A2 a, void* elem, void* cl){
    Pnm_ppm copy = (Pnm_ppm) cl;
    int new_i = methods->height(a) - i - 1;
    int new_j = methods->width(a) - j - 1;
    Pnm_rgb new_p = methods->at(copy->pixels, new_j, new_i);
    *new_p = *((Pnm_rgb) elem);
}

void rotate270(int j, int i, A2 a, void* elem, void* cl){
    
    Pnm_ppm copy = (Pnm_ppm) cl;
    int new_i = methods->width(a) - j - 1;
    int new_j = i;
    Pnm_rgb new_p = methods->at(copy->pixels, new_j, new_i);
    *new_p = *((Pnm_rgb) elem);
}

void flipHorizontal(int j, int i, A2 a, void* elem, void* cl){
    Pnm_ppm copy = (Pnm_ppm) cl;
    int new_j = methods->width(a) - j - 1;
    Pnm_rgb new_p = methods->at(copy->pixels, new_j, i);
    *new_p = *((Pnm_rgb) elem);
}

void flipVertical(int j, int i, A2 a, void* elem, void* cl){
    Pnm_ppm copy = (Pnm_ppm) cl;
    int new_i = methods->height(a) - i - 1;
    Pnm_rgb new_p = methods->at(copy->pixels, j, new_i);
    *new_p = *((Pnm_rgb) elem);
}

void transpose(int j, int i, A2 a, void* elem, void* cl){
    (void) a;
    Pnm_ppm copy = (Pnm_ppm) cl;
    Pnm_rgb new_p = methods->at(copy->pixels, i, j);
    *new_p = *((Pnm_rgb) elem);   
}

Pnm_ppm new_Pnm(unsigned w, unsigned h, unsigned d, int size, int blocksize, A2Methods_T methods){
    
    A2 arr = methods->new_with_blocksize((int)w, (int) h, size, blocksize);
    Pnm_ppm img_copy  =  malloc(sizeof(struct Pnm_ppm) * 1);
    img_copy->width = w;
    img_copy->height = h;
    img_copy->denominator = d;
    img_copy->pixels = arr;
    img_copy->methods = methods;
    return img_copy;
}
















































