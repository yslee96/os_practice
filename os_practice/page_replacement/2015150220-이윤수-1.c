#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int* generate_ref_arr(size_t sz, size_t page_max);
int lru(int* ref_arr, size_t ref_arr_sz, size_t frame_sz);

int _contains(int* stack, size_t sz, int top, int target);

// Driver code
//  Usage: ./program {ref_arr_size} {page_max} {frame_size}
int main(int argc, char** argv) {
    if (argc < 4) return -1;
    
    int ref_arr_sz = atoi(argv[1]);
    int page_max = atoi(argv[2]);
    int frame_sz = atoi(argv[3]);

    int* ref = generate_ref_arr(ref_arr_sz, page_max);
    int page_faults = lru(ref, ref_arr_sz, frame_sz);
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

int lru(int* ref_arr, size_t ref_arr_sz, size_t frame_sz) {
    int i, j, page_loc;
    int page_faults = 0;

    // Initializing frames
    int* frames = (int*) malloc(sizeof(int) * frame_sz);
    for (i=0; i<frame_sz; i++) frames[i] = -1;

    // TODO
    int top = -1;
    for(i=0; i<ref_arr_sz; i++){
        page_loc = _contains(frames, frame_sz, top, ref_arr[i]);
        if(page_loc == -1){
            if(top == frame_sz-1){      //stack is full
                for(j=1; j<=top; j++)
                    frames[j-1] = frames[j];
                frames[top] = ref_arr[i];
            }
            else{
                frames[++top] = ref_arr[i];
            }
            page_faults++;
        }
        else{
            for(j=page_loc+1; j<=top; j++)
                frames[j-1] = frames[j];
            frames[top] = ref_arr[i];  
        }
    }
    free(frames);

    return page_faults;
}


int _contains(int* stack, size_t sz, int top, int target) {

    int i =0;
    if(top >= 0){
        for(i=0; i<sz; i++){
            if(stack[i] == target) return i;
        }
    }

    return -1;
}