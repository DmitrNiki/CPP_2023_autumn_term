#include <cassert>
#include "BigInt.h"

void test1() {
    BigInt from_empty;
    BigInt from_int(123'456);
    BigInt from_neg_int(-123'456);
    BigInt from_string("0123456789012345678901452523");
    BigInt from_neg_string("-123123123123123123123123132132132131231");
    BigInt copy_constr(from_string);
    assert(copy_constr == from_string);
}

void test2() {
    assert(BigInt("123456789123456789") ==
           BigInt("123456789123456789"));
    assert(BigInt("-12345678901234567890") ==
           BigInt("-12345678901234567890"));
    assert(BigInt("123456789123456789") != BigInt("58943759834759"));
    // BigInt bi = BigInt("2134631238579523625982");  
    // assert(562952925985_bi == BigInt("562952925985"));
    
}

void test3() {
    assert(BigInt(1) < BigInt(10));
    assert(BigInt(-100) < 5);
    assert(!(BigInt(1) > BigInt(10)));
    assert(!(BigInt(-100) > BigInt(5)));
    assert(BigInt("123456789012345678901234567890") <
           BigInt("123456789012545678901234567890"));
    assert(!(BigInt("123456789012345678901234567890") <
             BigInt("123456789012345678901234567890"))); 
    BigInt a2("1234567780947589270491874");
    assert(--a2 == BigInt("-1234567780947589270491874"));
}


int main() {
    test1();
    std::cerr << "Test 1 passed." << std::endl;
    test2();
    std::cerr << "Test 2 passed." << std::endl;
    test3();
    std::cerr << "Test 3 passed." << std::endl;
}