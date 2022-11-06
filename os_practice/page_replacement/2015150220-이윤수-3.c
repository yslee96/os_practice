#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EMPTY_FRAME -1

int* generate_ref_arr(size_t sz, size_t page_max);
int lru(int* ref_arr, size_t ref_arr_sz, size_t frame_sz, size_t page_max);

int _contains(int* arr, size_t sz, int target);
// Driver code
//  Usage: ./program {ref_arr_size} {page_max} {frame_size}
int main(int argc, char** argv) {
    if (argc < 4) return -1;
    
    int ref_arr_sz = atoi(argv[1]);
    int page_max = atoi(argv[2]);
    int frame_sz = atoi(argv[3]);

    int* ref = generate_ref_arr(ref_arr_sz, page_max);
    int page_faults = lru(ref, ref_arr_sz, frame_sz, page_max+1);
    printf("%d\n", page_faults);
    free(ref);

    return 0;
}

int* generate_ref_arr(size_t sz, size_t page_max) {
    int* ref_arr = (int*) malloc(sizeof(int) * sz);
    
    // Generate Random Refernece String
    if(ref_arr){
        srand((unsigned)time(NULL));
        for(int i=0; i<sz; i++){
            ref_arr[i] = rand() % (page_max+1);
        }
    }
    return ref_arr;
}

int lru(int* ref_arr, size_t ref_arr_sz, size_t frame_sz, size_t page_max) {
    int i, j;
    int page_faults = 0;
    int is_fault = 0;
    
    // Initializing frames
    int* frames = (int*) malloc(sizeof(int) * frame_sz);
    for (i=0; i<frame_sz; i++) frames[i] = -1;

    // TODO
    unsigned char* ref_bits = (unsigned char*) malloc(sizeof(unsigned char) * page_max);
    for (i=0; i<page_max; i++) ref_bits[i] = 0x00;

    for (i=0; i<ref_arr_sz; i++){

        for (j=0; j<page_max; j++){
             ref_bits[j] = (ref_bits[j] >> 1);
        }

        is_fault = _contains(frames, frame_sz, ref_arr[i]);

        if(is_fault == -1){ 
            int empty_idx = _contains(frames, frame_sz, EMPTY_FRAME);
            
            if(empty_idx != -1){
                frames[empty_idx] = ref_arr[i];
            }
            else{ // frame is full
                unsigned char min = 0xFF;
                int min_idx;
                for(j=0; j<frame_sz; j++){
                    if(ref_bits[frames[j]] < min) {
                        min = ref_bits[frames[j]];
                        min_idx = j;
                    }
                }
                frames[min_idx] = ref_arr[i];
            }
            page_faults++;
        }
        ref_bits[ref_arr[i]] |= 0x80;
    }

    free(frames);
    free(ref_bits);
   
    return page_faults;
}

int _contains(int* arr, size_t sz, int target) {
    int i = 0;
    for (i=0; i<sz; i++) {
        if (arr[i] == target) return i;
    }
    return -1;
}


