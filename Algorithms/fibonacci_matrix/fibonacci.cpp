#include <cstdint>
#include "matrix.h"
#include <iostream>

uint64_t check(int n) { // O(n)
    uint64_t f;
    f = (n > 0) ? 1 : 0;
    uint64_t f0 = 0;
    uint64_t f1 = 1;
    for(int i = 2; i < n + 1; i++) {
        f = f0 + f1;
        f0 = f1;
        f1 = f;
    }
    return f;
}

template<class T>
T BinPow(T& a, T& id, int b) { // O(nlog(n))
    if (b <= 0) {
        return id;
    }
    T tmp = a * a;
    if (b % 2 == 0) {
        return BinPow(tmp, id, b / 2);
    }
    if (b % 2 == 1) {
        return (BinPow(tmp, id, (b - 1) / 2) * a) ;
    }
}

int main(){
    Matrix<uint64_t> M({{0, 1}, {1, 1}});//матрица,которую нужно возводить в степень
    int num;
    std::cout << "Input index of Fibonacci number ";//порядковый номер числа Фибоначчи
    std::cin >> num;
    uint64_t ans = 1;
    if (num > 0) {
        Matrix<uint64_t> Id= Identity<uint64_t>(2);
        Matrix<uint64_t> mPow = BinPow(M, Id, num - 1);
        ans = mPow(1, 1);
    } else {
        ans = 0;
    }
    std::cout << "answer = " << ans << '\n'; // O(log(n))
    std::cout << check(num) <<'\n'; // O(n)
    return 0;
}
