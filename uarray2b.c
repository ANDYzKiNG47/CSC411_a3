#include "uarray2b.h"

struct UArray2b_T{
    int width;
    int height;
    int size;
    int blocksize;
    UArray2* matrix;
};

UArray2b_T* UArray2b_new(int width, int height, int size, int blocksize){
    
    UArray2b_T* arr = malloc(sizeof(UArray2b));
    arr->width = width;
    arr->height = height;
    arr->size = size;
    arr->blocksize = blocksize;

    int nRows = (int) ceil( (double) (height / blocksize) );
    int nCols = (int) ceil( (double) (width / blocksize) );
    arr->matrix = Uarray2_new(nCols, nRows, sizeof(Array_T*));
    for (int i = 0; i < nRows; ++i){
        for (int j = 0; j < nCols; ++j){
            Array_T* p = UArray2_get(arr->matrix, i, j);
            int blockLength = (int) pow( (double) blocksize, 2.0);
            *p = Array_new(blockLength, size);
        }
    }
    return arr;
}

UArray2b_T* UArray2b_new_64k_block(int width, int height, int size){
    
    UArray2b_T* arr = malloc(sizeof(UArray2b));
    arr->width = width;
    arr->height = height;
    arr->size = size;
    arr->blocksize = 64k;

    int nRows = (int) ceil( (double) (height / arr->blocksize) );
    int nCols = (int) ceil( (double) (width / arr->blocksize) );
    arr->matrix = Uarray2_new(nCols, nRows, sizeof(Array_T*));
    for (int i = 0; i < nRows; ++i){
        for (int j = 0; j < nCols; ++j){
            Array_T* p = UArray2_get(arr->matrix, i, j);
            int blockLength = (int) pow( (double) 64k, 2.0);
            *p = Array_new(blockLength, size);
        }
    }
    return arr;
}

void UArray2b_free(UArray2b_T* arr){
    
    int nRows = UArray2_height(arr->matrix);
    int nCols = UArray2_width(arr->matrix);
    for (int i = 0; i < nRows; ++i){
        for (int j = 0; j < nCols; ++j){
            Array_T* p = UArray2_get(arr->matrix, i, j);
            Array_free(p);
        }
    }
    UArray2_free(arr->matrix);
    free(arr);
}

int UArray2b_width(UArray2b_T* arr){

}

int UArray2b_height(UArray2b_T* arr){

}

int UArray2b_size(UArray2b_T* arr){

}

int UArray2b_blocksize(UArray2b_T* arr){

}

void* UArray2b_get(UArray2b_T* arr, int row, int column){

}

void UArray2b_map(UArray2b_T* arr, apply_func, void* cl){

}



