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

void rotate90(int i, int j, A2 a, void* elem, void* cl);

void rotate180(int i, int j, A2 a, void* elem, void* cl);

void rotate270(int i, int j, A2 a, void* elem, void* cl);

int main(int argc, char *argv[]) {
    printf("start\n");
    int rotation = 0;
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
    printf("before read\n");
    Pnm_ppm img = Pnm_ppmread(stdin, methods);
    printf("after read\n");
    A2 arr_copy;
    int w = img->width;
    int h = img->height;
    int s = sizeof(Pnm_rgb);
    int bs = img->methods->blocksize(img->pixels);

    switch(rotation){
        case 90:
            arr_copy = methods->new_with_blocksize(h, w, s, bs);
            map(img->pixels, rotate90, arr_copy);
            img->width = h;
            img->height = w;
            break;

        case 180:
            arr_copy = methods->new_with_blocksize(w, h, s, bs);
            map(img->pixels, rotate180, arr_copy);
            break;

        case 270:
            arr_copy = methods->new_with_blocksize(h, w, s, bs);
            map(img->pixels,rotate270, arr_copy);
            img->width = h;
            img->height = w;
            break;

        default:
            break;
    }
    if (rotation != 0){
        img->pixels = arr_copy;
    }
    printf("yes\n");
    Pnm_ppmwrite(stdout, img);
    Pnm_ppmfree(&img);

    return 0;
    //assert(0); // the rest of this function is not yet implemented
}

void rotate90(int i, int j, A2 a, void* elem, void* cl){
    int new_i = methods->height(a) - j -1;
    int new_j = i;
    Pnm_rgb new_p = methods->at(cl, new_j, new_i);
    *new_p = *((Pnm_rgb) elem);
}

void rotate180(int i, int j, A2 a, void* elem, void* cl){
    int new_i = methods->width(a) - i - 1;
    int new_j = methods->height(a) - j - 1;
    Pnm_rgb new_p = methods->at(cl, new_j, new_i);
    *new_p = *((Pnm_rgb) elem);
}

void rotate270(int i, int j, A2 a, void* elem, void* cl){
    int new_i = j;
    int new_j = methods->width(a) - i - 1;
    Pnm_rgb new_p = methods->at(cl, new_j, new_i);
    *new_p = *((Pnm_rgb) elem);
}



















































