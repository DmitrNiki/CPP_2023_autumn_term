#include <iostream>
#include <cassert>

template<typename T> 
void array_copy(T * const ds_begin, T const * const src_begin, T const * const src_end){
    for (unsigned idx = 0; src_begin + idx != src_end; ++idx){
        *(ds_begin + idx) = *(src_begin + idx);
    }
}



int main() {
    int arr_src[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int arr_dst[10];
    array_copy(arr_dst, arr_src, arr_src + 10);
    for (unsigned idx = 0; idx != 10; ++idx)
  	    assert(arr_src[idx] == arr_dst[idx]);
    return 0;
}
