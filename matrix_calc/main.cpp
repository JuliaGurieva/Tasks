#include <iostream>
#include <vector>
#include "matrix.h"

int main() {
    size_t rows, cols;
    std::cout<<" Input number of rows and columns : \n";
    std::cin >> rows >> cols;
    Matrix a(rows, cols);
    Input(a);
    //RandInput(a, 20);
    std::cout << "Rank = " << MatrixRank(a) << '\n';
    std::vector<double> b;
    std::vector<double> copy;
    for (int i = 0; i < rows; ++i) {
        //b.push_back(rand() % 20);
        double x;
        std::cin >> x;
        b.push_back(x);
    }
    copy = b;
    PrintExtended( a, b);
    std::cout<<'\n';
    Matrix res = Triangle(a, copy);
    Print(res);
    std::cout << '\n';
    auto ans = Gauss(a, b);
    if (ans.first == LinearSystem::InfSolutions) {
        std::cout<<"Infinite solutions \n";
    } else if (ans.first == LinearSystem::NoSolutions) {
        std::cout<<"No solutions \n";
    }
    if (ans.first == LinearSystem::Solution) {
        for (double el : ans.second) {
         std::cout << el << ' ';
        }
    }
    std::cout << '\n' << "Determinant = " << a.Det();
    std::cout << '\n' << "Inverse matrix : \n";
    Matrix inv = Inverse(a, a.Det());
    Print(inv);
    return 0;
}
