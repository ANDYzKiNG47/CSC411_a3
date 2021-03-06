#include "uarray2b.h"

struct UArray2b_T{
    int width;
    int height;
    int size;
    int blocksize;
    UArray2_T* matrix;
};

UArray2b_T* UArray2b_new(int width, int height, int size, int blocksize){
    
    UArray2b_T* arr = malloc(sizeof(UArray2b_T));
    arr->width = width;
    arr->height = height;
    arr->size = size;
    arr->blocksize = blocksize;
    
    // determine how many blocks
    int nRows = (int) ceil( (double) ((double)height /(double) blocksize) );
    int nCols = (int) ceil( (double) ((double)width / (double)blocksize) );
    // spine of array
    arr->matrix = Uarray2_new(nCols, nRows, sizeof(Array_T*));
    // initialize blocks
    for (int i = 0; i < nRows; ++i){
        for (int j = 0; j < nCols; ++j){
            Array_T* p = UArray2_get(arr->matrix, j, i);
            int blockLength = (int) pow( (double) blocksize, 2.0);
            *p = Array_new(blockLength, size);
        }
    }
    return arr;
}

UArray2b_T* UArray2b_new_64K_block(int width, int height, int size){
    UArray2b_T* arr = UArray2b_new(width, height, size, (int) sqrt(BLOCK64K));
    return arr;
}

void UArray2b_free(UArray2b_T* arr){
    
    int nRows = UArray2_height(arr->matrix);
    int nCols = UArray2_width(arr->matrix);
    for (int i = 0; i < nRows; ++i){
        for (int j = 0; j < nCols; ++j){
            Array_T* p = UArray2_get(arr->matrix, j, i);
            // free the blocks
            Array_free(p);
        }
    }
    // free rows
    UArray2_free(arr->matrix);
    // free the spine
    free(arr);
}

int UArray2b_width(UArray2b_T* arr){
    return arr->width;
}

int UArray2b_height(UArray2b_T* arr){
    return arr->height;
}

int UArray2b_size(UArray2b_T* arr){
    return arr->size;
}

int UArray2b_blocksize(UArray2b_T* arr){
    return arr->blocksize;
}

void* UArray2b_get(UArray2b_T* arr, int column, int row){

    // throw error if out of bounds
    if (row >= arr->height || row < 0 || column >= arr->width || column < 0){
        fprintf(stderr, "Error: Out of bounds\n");
        exit(1);
    }
    //convert pixel to block coordinate
    int block_i = row / arr->blocksize;
    int block_j = column / arr->blocksize;
    int target = arr->blocksize * (row % arr->blocksize) + (column % arr->blocksize);
    Array_T* p = UArray2_get(arr->matrix, block_j, block_i);
    return Array_get(*p, target);
}

// returns 1 if in bounds
// returns 0 if not
int pixelCoord(UArray2b_T* arr, int i, int j, int target, int* column, int* row){
    int r, c;
    
    r = (arr->blocksize * i) + (target / arr->blocksize);
    c = (arr->blocksize * j) + (target % arr->blocksize);
    
    if ( row != NULL && column != NULL ){
        *row = r;
        *column = c;
    }
    
    if ( r < 0 || r >= arr->height || c < 0 || c >= arr->width)
        return 0;
    else
        return 1;
}

void UArray2b_map(UArray2b_T* arr,
    void apply(int i, int j, UArray2b_T* array2b, void *elem, void *cl), void* cl){
    
    int nRows = UArray2_height(arr->matrix);
    int nCols = UArray2_width(arr->matrix);
    int blockLength = (int) pow( (double) arr->blocksize, 2.0);
    int isEdge = 0; // bool to track if block on an edge
    int* row = malloc(sizeof(int));
    int* col = malloc(sizeof(int));
    // invariant: all pixels passed to the apply function will be in bounds
    for (int i = 0; i < nRows; ++i){
        for (int j = 0; j < nCols; ++j){
            // check if edge block
            if ( i == nRows-1 || j == nCols-1)
                isEdge = 1;
            else
                isEdge = 0;
            // access each element in block
            for (int k = 0; k < blockLength; ++k){
                // if block is on the edge and the elem being accessed 
                // is out of bounds continue with loop
                int inBounds = pixelCoord(arr, i, j, k, col, row);
                if ( isEdge && !inBounds ){
                    continue; 
                }
                // DO STUFF
                void* e = UArray2b_get(arr, *col, *row);
                apply( *col, *row, arr, e, cl);
            }//end k
        }//end j
    }//end i
    free(row);
    free(col);
}


















































