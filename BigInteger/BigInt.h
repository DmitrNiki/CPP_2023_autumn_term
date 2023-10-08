#include "iostream"
#include <string.h>
#include <algorithm>
#include "head.h"

using u32 = uint32_t;
using u16 = uint16_t;

struct BigInt{
    std::string s;

    BigInt() = default;
    BigInt(int x):s(std::to_string(x)){};
    BigInt(const std::string& S):s(S){};
    BigInt(const BigInt& A): s(A.s){};
};


// BigInt operator""_bi(unsigned char* x, size_t size) {
//     BigInt x();
//     for (size_t i = 0; i < size; i++){
//     }
// } 

bool operator==(const BigInt &a, const BigInt &b){
    if (a.s == b.s) return 1;
    return 0;
}

bool operator!=(const BigInt &a, const BigInt &b){
    if (a.s != b.s) return 1;
    return 0;
}

BigInt operator--(const BigInt& a){
    std::string abs_a;
    if (a.s[0] == '-'){
        for(size_t i = 1; i < a.s.length(); i++){
            abs_a += a.s[i]; 
        }
        return BigInt(abs_a);
    }
    return BigInt('-' + a.s);
}

bool operator>(const BigInt &a, const BigInt &b){
    int flag = 0;
    if (a.s[0] == '-' && b.s[0] == '-'){
        flag = 1;
    }
    if (a.s[0] == '-' && b.s[0] != '-') return 0;
    if (a.s[0] != '-' && b.s[0] == '-') return 1;
    if ((a.s.length() > b.s.length() && !flag) || (a.s.length() < b.s.length() && flag)) return 1;
    else if ((a.s.length() > b.s.length() && flag) || (a.s.length() < b.s.length() && !flag)) return 0;
    for(size_t i = 0; i < a.s.length(); i++){
        if (((--a).s[i] > (--b).s[i] && !flag)||((--a).s[i] < (--b).s[i] && flag)) return 1;
        if (((--a).s[i] < (--b).s[i] && !flag)||((--a).s[i] > (--b).s[i] && flag)) return 0;
    }
    return 0;
}

bool operator<(const BigInt &a, const BigInt &b){
    return (b > a);
}

BigInt operator+(BigInt &a, BigInt &b){
    BigInt res;
    int next_dis = 0;
    int flag = 0;
    if(a.s[0] == '-' && b.s[0] == '-'){
        a = --a;
        b = --b;
        flag = 1;
    }else if(a.s[0] == '-' && b.s[0] != '-'){
        a = --a;
        res = b - a;
        return res;
    }else if(a.s[0] != '-' && b.s[0] == '-'){
        b = --b;
        res = a - b;
        return res;
    }

    for (size_t i = 1; i <= std::max(a.s.length(),b.s.length()); ++i){
        std::string sa(1, a.s[a.s.length() - i]);
        std::string sb(1, b.s[b.s.length() - i]);
        int nxt = next_dis;
        int x = atoi(sa.c_str()) + atoi(sb.c_str()) + nxt;
        next_dis = x / 10;
        x %= 10;
        res.s += std::to_string(x);
        if (i == std::max(a.s.length(),b.s.length()) && next_dis != 0) res.s += std::to_string(next_dis );
    }
    if (flag) res.s += '-';
    reverse(res.s.begin(), res.s.end());
    
    return res;
}

BigInt Subtraction(const BigInt a, const BigInt b){
    BigInt res;
    int next_dis = 0;
    std::string tmp;
    for (size_t i = 1; i <= std::max(a.s.length(),b.s.length()); ++i){
        std::string sa(1, a.s[a.s.length() - i]);
        std::string sb(1, b.s[b.s.length() - i]);
        int x = 10*(i!= a.s.length()) + atoi(sa.c_str()) - atoi(sb.c_str()) + next_dis - (i != 1);
        next_dis = x / 10;
        x %= 10;
        if (x == 0) {
            tmp += std::to_string(x);
        }else {
            res.s = tmp;
            res.s += std::to_string(x);
            tmp = res.s;
        }
        if (i == std::max(a.s.length(),b.s.length()) && next_dis != 0) res.s += std::to_string(next_dis);
    }
    reverse(res.s.begin(), res.s.end());
    
    return res;
}

BigInt operator-(BigInt &a, BigInt &b){
    BigInt res;
    
    if(a.s[0] == '-' && b.s[0] == '-'){
        a = --a;
        b = --b;
        if (a > b) res.s = '-' + Subtraction(a, b).s;
        else if (b > a) res = Subtraction(b, a);
        else res.s = '0';
    }else if(a.s[0] == '-' && b.s[0] != '-'){
        a = --a;
        res.s = '-' + (a + b).s;
    }else if(a.s[0] != '-' && b.s[0] == '-'){
        b = --b;
        res.s = (a + b).s;
    }else if(a.s[0] != '-' && b.s[0] != '-'){
        if (a > b) res = Subtraction(a, b);
        else if (b > a) res.s = '-' + Subtraction(b, a).s;
        else res.s = '0';
    }
    return res;
}

BigInt operator*(BigInt &a, BigInt &b){
    BigInt res;
    int flag = 0;

    if(a.s[0] == '-' && b.s[0] != '-'){
        flag = 1;
        a = --a;
    }else if(a.s[0] != '-' && b.s[0] == '-'){
        flag = 1;
        b = --b;
    }else if(a.s[0] == '-' && b.s[0] == '-'){
        b = --b;
        a = --a;
    }
    int M[b.s.length()][a.s.length()];
    BigInt N[b.s.length()];
    for (size_t i = 0; i < b.s.length(); ++i){
        for(size_t j = 0; j < a.s.length(); ++j){
            std::string sa(1, a.s[a.s.length() - 1 - i]);
            std::string sb(1, b.s[b.s.length() - 1 - j]);
            M[i][j] = atoi(sa.c_str())* atoi(sb.c_str());
        }
        for(size_t j = 0; j < a.s.length(); ++j){
            BigInt cur;
            cur.s = std::to_string(M[i][j]);
            for(size_t k = 0; k < j; k++){
                cur.s += '0';
            }
            N[i] = N[i] + cur;
        }
        for(size_t k = 0; k < i; k++){
            N[i].s += '0';
        }
        res = res + N[i]; 
    }
    if (flag) res.s = '-' + res.s;
    return res;
}

// int main(){
//     BigInt a, b;
//     std::cin >> a.s >> b.s;
//     std::string tmp_a = a.s;
//     std::string tmp_b = b.s;

//     if(a.s[0] == ' ' || b.s[0] == ' '){
//         std::cout << "Unavalable input\n";
//         return 0;
//     }
//     std::cout << "a + b = " << (a + b).s <<"\n";
//     a.s = tmp_a;
//     b.s = tmp_b; 
//     std::cout << "a - b = " << (a - b).s <<"\n";
//     a.s = tmp_a;
//     b.s = tmp_b;
//     std::cout << "a * b = " << (a*b).s << "\n";
//     return 0;
// }
