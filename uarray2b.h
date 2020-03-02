#ifndef UARRAY2B_INCLUDED
#define UARRAY2B_INCLUDED

#define BLOCK64K 65536

#include <math.h>
#include <stdio.h>
#include "uarray2.h"

typedef struct UArray2b_T UArray2b_T;


// blocksize = sqrt(num cells in block)
UArray2b_T* UArray2b_new(int width, int height, int size, int blocksize);

// creates new array with block size of 64 kb  
UArray2b_T* UArray2b_new_64K_block(int width, int height, int size);

// frees the entire array
void UArray2b_free(UArray2b_T* arr);

// returns the width of array
int UArray2b_width(UArray2b_T* arr);

// returns the height of the array
int UArray2b_height(UArray2b_T* arr);

// returns the size of the elements stored at the lowest level
int UArray2b_size(UArray2b_T* arr);

// returns the blocksize
// block size is n where a block is nxn
int UArray2b_blocksize(UArray2b_T* arr);

// returns element at a certain coordinate
// takes pixel coordinates
void* UArray2b_get(UArray2b_T* arr, int column, int row);

// applies function to elements in a block major pattern
void UArray2b_map(UArray2b_T* arr, 
        void apply(int i, int j, UArray2b_T* array2b, void *elem, void *cl), void* cl);

#endif
