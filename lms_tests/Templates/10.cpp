#include <iostream>
#include <cassert>

template <typename T>
T error_tpl(T const &a) {
    assert(false);
    return 1; // unreachable code
}

template<>
int error_tpl<int>(int const &a) {
    assert(true);
    return 1; 
}

int main(){
    bool a = true;
    assert(a == error_tpl(static_cast<int>(a)));
    return 0;
}