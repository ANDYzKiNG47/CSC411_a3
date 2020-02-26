#ifndef UARRAY2B_INCLUDED
#define UARRAY2B_INCLUDED

#define BLOCK64K 64000

#include <math.h>
#include <stdio.h>
#include "uarray2.h"

typedef struct UArray2b_T UArray2b_T;


// blocksize = sqrt(num cells in block)
UArray2b_T* UArray2b_new(int width, int height, int size, int blocksize);

UArray2b_T* UArray2b_new_64K_block(int width, int height, int size);

void UArray2b_free(UArray2b_T* arr);

int UArray2b_width(UArray2b_T* arr);

int UArray2b_height(UArray2b_T* arr);

int UArray2b_size(UArray2b_T* arr);

int UArray2b_blocksize(UArray2b_T* arr);

void* UArray2b_get(UArray2b_T* arr, int row, int column);

void UArray2b_map(UArray2b_T* arr, 
        void apply(int i, int j, UArray2b_T array2b, void *elem, void *cl), void* cl);

#endif
