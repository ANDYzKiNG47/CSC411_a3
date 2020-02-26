#include "uarray2.h"

struct UArray2_T {
    int width;
    int height;
    int size;
    Array_T matrix;
};

UArray2_T* Uarray2_new(int width, int height, int size){

    UArray2_T* uarr2 = malloc(sizeof(UArray2_T));
    uarr2->width = width;
    uarr2->height = height;
    uarr2->size = size;
    uarr2->matrix = Array_new(height, sizeof(Array_T*));
    for (int i = 0; i < height; ++i){
        Array_T* p = Array_get(uarr2->matrix, i);
        *p = Array_new(width, size);
    }
    return uarr2;
}

void UArray2_free(UArray2_T* arr){

    for (int i = 0; i < arr->height; ++i){
        Array_T* p = Array_get(arr->matrix, i);
        Array_free(p);
    }
    Array_free(&arr->matrix);
    free(arr);

}

int UArray2_width(UArray2_T* arr){
    return arr->width;
}

int UArray2_height(UArray2_T* arr){
    return arr->height;
}

int UArray2_size(UArray2_T* arr){
    return arr->size;
}

void* UArray2_get(UArray2_T* arr, int row, int column){

    Array_T* p = Array_get(arr->matrix, row);
    void* elem = Array_get(*p, column);
    return elem;
}

void UArray2_map_row_major(UArray2_T* arr, apply_func apply, void* cl){
    
    for (int i = 0; i < arr->height; ++i){
        for (int j = 0; j < arr->width; ++j){
            void* e = UArray2_get(arr, i, j);
            apply(i, j, arr, e, cl);
        }
    }
}

void UArray2_map_col_major(UArray2_T* arr, apply_func apply, void* cl){
    
    for (int j = 0; j < arr->width; ++j){
        for (int i = 0; i < arr->height; ++i){
            void* e = UArray2_get(arr, i, j);
            apply(i, j, arr, e, cl);
        }
    }
}
/*
void UArray2_map_block_major(UArray2_T* arr, apply_func apply, void* cl){
    
    if (arr->height != 9 || arr->width != 9)
        return;

    int j_idx = 0;
    for (int k = 0; k < 3; ++k){
        for (int i = 0; i < 3; ++i){
            for (int j = j_idx; j < j_idx + 3; ++j){
                void* e = UArray2_get(arr, i, j);
                apply(arr, i, j, e, cl);
            }
        }
        j_idx += 3;
    }

    j_idx = 0;
    for (int k = 0; k < 3; ++k){
        for (int i = 3; i < 6; ++i){
            for (int j = j_idx; j < j_idx + 3; ++j){
                void* e = UArray2_get(arr, i, j);
                apply(arr, i, j, e, cl);
            }
        }
        j_idx += 3;
    }

    j_idx = 0;
    for (int k = 0; k < 3; ++k){
        for (int i = 6; i < 9; ++i){
            for (int j = j_idx; j < j_idx + 3; ++j){
                void* e = UArray2_get(arr, i, j);
                apply(arr, i, j, e, cl);
            }
        }
        j_idx += 3;
    }
}
*/

UArray2_T* UArray2_readPGM(FILE* input){
    
    if (input == NULL){
        fprintf(stderr, "Error: Invalid file\n");
        exit(1);
    }

    Pnmrdr_T pgmReader = Pnmrdr_new(input);
    Pnmrdr_mapdata pgmData = Pnmrdr_data(pgmReader);
    
    if (pgmData.type != Pnmrdr_gray){
        fprintf(stderr, "Error: Invalid image type\n");
        Pnmrdr_free(&pgmReader);
        exit(1);
    }
    
    UArray2_T* arr = Uarray2_new(pgmData.width, pgmData.height,  sizeof(int));
    int len = arr->width * arr->height;
    for (int i = 0; i < len; ++i){
        int r = i / pgmData.height;
        int c = i % pgmData.width;
        int pixel = (int) Pnmrdr_get(pgmReader);
        int* e = UArray2_get(arr, r, c);
        *e = pixel;
    }
    Pnmrdr_free(&pgmReader);

    return arr;
}


















