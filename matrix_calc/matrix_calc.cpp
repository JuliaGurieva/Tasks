#include <iostream>
#include <vector>
#include <iomanip>
#include <stdexcept>

double eps = 1.0e-4;

class Matrix {
public:
    Matrix() : a_() {
        rows_ = 0;
        cols_ = 0;
    }
    Matrix(size_t rows, size_t cols) : rows_(rows), cols_(cols), a_() {
        for (size_t i = 0; i < rows; ++i) {
            std::vector<double> zero(cols, 0.0);
            a_.push_back(zero);
        }
    }

    Matrix(size_t rows) : Matrix(rows, rows) {}

    Matrix(const std::vector<std::vector<double>> a) : rows_(a.size()), cols_(a[0].size()), a_(a) {}

    Matrix(const Matrix& rhs) : rows_(rhs.rows_), cols_(rhs.cols_), a_(rhs.a_) {}

    size_t Rows() const {
        return rows_;
    }

    size_t Columns() const {
        return cols_;
    }
    double& Rank() {
        return rank_;
    }
    double& Det() {
        if (rows_ != cols_) {
            throw std::runtime_error("Not a square matrix ");
        }
        return det_;
    }
    const double& operator()(size_t i, size_t j) const {
        return a_[i][j];
    }

    double& operator()(size_t i, size_t j) {
        return a_[i][j];
    }

    Matrix& operator=(const Matrix& rhs) {
        for (size_t i = 0; i < rhs.Rows(); ++i) {
            for (size_t j = 0; j < rhs.Columns(); ++j) {
                a_[i][j] = rhs(i, j);
            }
        }
        rows_ = rhs.Rows();
        cols_ = rhs.Columns();
        return *this;
    }

    Matrix& operator+=(const Matrix& rhs) {
        for (size_t i = 0; i < rhs.Rows(); ++i) {
            for (size_t j = 0; j < rhs.Columns(); ++j) {
                a_[i][j] += rhs(i, j);
            }
        }
        return *this;
    }

    Matrix& operator-=(const Matrix& rhs) {
        for (size_t i = 0; i < rhs.Rows(); ++i) {
            for (size_t j = 0; j < rhs.Columns(); ++j) {
                a_[i][j] -= rhs(i, j);
            }
        }
        return *this;
    }

    friend Matrix operator*(const Matrix& lhs, const Matrix& rhs);

    Matrix& operator*=(const Matrix& rhs) {
        return *this = *this * rhs;
    }

private:
    size_t rows_;
    size_t cols_;
    std::vector<std::vector<double>> a_;
    double det_ = 0;
    double rank_ = 0;
};

//Types of linear system solutions
enum class LinearSystem {
    NoSolutions, Solution, InfSolutions
};

Matrix Transpose(const Matrix& a) {
    Matrix c(a.Columns(), a.Rows());
    for (size_t i = 0; i < a.Columns(); ++i) {
        for (size_t j = 0; j < a.Rows(); ++j) {
            c(i, j) = a(j, i);
        }
    }
    return c;
}

Matrix Identity(size_t n) {
    Matrix e(n, n);
    for (size_t i = 0; i < n; ++i) {
        e(i, i) = 1;
    }
    return e;
}

Matrix operator+(const Matrix& lhs, const Matrix& rhs) {
    Matrix c(lhs.Rows(), lhs.Columns());
    for (size_t i = 0; i < lhs.Rows(); ++i) {
        for (size_t j = 0; j < lhs.Columns(); ++j) {
            c(i, j) = lhs(i, j) + rhs(i, j);
        }
    }
    return c;
}

Matrix operator-(const Matrix& lhs, const Matrix& rhs) {
    Matrix c(lhs.Rows(), lhs.Columns());
    for (size_t i = 0; i < lhs.Rows(); ++i) {
        for (size_t j = 0; j < lhs.Columns(); ++j) {
            c(i, j) = lhs(i, j) - rhs(i, j);
        }
    }
    return c;
}

Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
    Matrix c(lhs.Rows(), rhs.Columns());
    for (size_t i = 0; i < lhs.Rows(); ++i) {
        for (size_t j = 0; j < rhs.Columns(); ++j) {
            for (size_t k = 0; k < lhs.Columns(); ++k) {
                c(i, j) += lhs(i, k) * rhs(k, j);
            }
        }
    }
    return c;
}

Matrix& ChangeRows(Matrix& a, size_t i, size_t j) {
    double tmp;
    size_t len = a.Columns();
    for (size_t k = 0; k < len; k++) {
        tmp = a(i, k);
        a(i, k) = a(j, k);
        a(j, k) = tmp;
    }
    return a;
}
//Gaussian elimination
// upper triangular form
Matrix Triangle(Matrix& a, std::vector<double>& right) { // O(n^3)
    Matrix res = a;
    int i = 0, j;
    size_t n = a.Rows();
    a.Det() = 1;
    while (i < n) {
        int p;
        for (p = i; p < n; ++p) {
            j = i;
            while(j < n && std::abs(res(j, p)) < eps) {
                ++j;
            }
            if (j!=n) {
                break;
            }
        }
        if (j < n && p < n) {
            // forward steps. Getting triangular matrix
            if (i < n-1 && j<n && i != j) {
                ChangeRows(res, i, j);
                if ((j-i)% 2)
                    a.Det() *= -1;
                std::swap(right[i], right[j]);
            }
            for (int l = i + 1; l < n; l++) {
                double k = -(res(l, p) / res(i, p));
                for (int col = p; col < res.Columns(); ++col) {
                    res(l, col) += k * res(i, col);
                }
                right[l] += k * right[i];
            }
        }
        a.Det() *= res(i, i);
        ++i;
    }
    return res;
}

// Solution Ax=b with Gaussian elimination(upper triangular form)
std::pair<LinearSystem, std::vector<double>> Gauss(Matrix& a, std::vector<double>& b) { // O(n^3)
    std::vector<double> right = b;
    size_t n = a.Rows();
    Matrix res = Triangle(a, right);
    std::vector<double> ans = {};
    std::vector<double> tmp(n, 0);
    std::vector<double> decision(n, 0);
    bool NoSolutions = false;
    bool InfSolutions = false;
    // reverse steps. Getting solution
    for (int ind = static_cast<int>(n) - 1; ind >= 0; --ind) {
        int l = ind;
        while(l<n && std::abs(res(ind, l)) < eps) {
            l++;
        }
        if (l==n) {
            if (std::abs(right[ind] - tmp[ind]) < eps) {
                InfSolutions = true;
            } else {
                NoSolutions = true;
            }
            decision[ind] = 0;
        } else {
            decision[ind] = (right[ind] - tmp[ind]) / res(ind, l);
        }
        for (int c = ind - 1; c >= 0; --c) {
            tmp[c] += res(c, ind) * decision[ind];
        }
    }
    if (NoSolutions) {
        return std::make_pair(LinearSystem::NoSolutions, ans);
    } else if (InfSolutions) {
        return std::make_pair(LinearSystem::InfSolutions, ans);
    }
    return std::make_pair(LinearSystem::Solution, decision);
}

// Getting rank of A using upper triangular form
int MatrixRank(Matrix& a) {
    std::vector<double> right (a.Rows(), 0);
    Matrix tmp = Triangle(a, right);
    int zero_rows = 0;
    for (int i = 0; i < static_cast<int>(tmp.Rows()); ++i) {
        bool zero = true;
        for (int j = 0; j < static_cast<int>(tmp.Columns()); ++j) {
            if (std::abs(tmp(i, j)) > eps) {
                zero = false;
                break;
            }
        }
        if (zero) {
            ++zero_rows;
        }
    }
    int zero_cols = 0;
    for (int i = 0; i < static_cast<int>(tmp.Columns()); ++i) {
        bool zero = true;
        for (int j = 0; j < static_cast<int>(tmp.Rows()); ++j) {
            if (std::abs(tmp(j, i)) > eps) {
                zero = false;
                break;
            }
        }
        if (zero) {
            ++zero_cols;
        }
    }
    a.Rank() = std::min(tmp.Rows() - zero_rows, tmp.Columns() - zero_cols);
    return a.Rank();
}

// Inverse matrix
Matrix Inverse (Matrix& a, double det) {
    if (a.Rows() != a.Columns()) {
        throw std::runtime_error("Not a square matrix ");
    }
    if (std::abs(det) < eps) {
        throw std::runtime_error("Singular matrix ");
    }
    Matrix inv(a.Rows());
    double col;
    std::vector<double> id (a.Rows(), 0);
    // solve Ax=(0...1...0) for each column of Identity matrix
    for (int i = 0; i < a.Rows(); ++i) {
        if (i > 0) {
            id[i-1] = 0;
        }
        id[i] = 1;
        auto x = Gauss(a, id);
        for (int j = 0; j < a.Rows(); ++j) {
            inv(j, i) = x.second[j];
        }
    }
    return inv;
}

void RandInput(Matrix& a, size_t n) {
    srand(time(NULL));
    for (size_t i = 0; i < a.Rows(); ++i) {
        for (size_t j=0; j < a.Columns(); ++j) {
            rand();
            a(i, j) = rand() % n;
            int b = rand();
            int c = rand();
            if (b > c) a(i, j) *= -1;
        }
    }
}

void Input(Matrix& a) {
    for (size_t i = 0; i < a.Rows(); ++i) {
        for (size_t j=0; j < a.Columns();++j) {
            std::cin >> a(i, j) ;
        }
    }
}

void Print(Matrix& a) { // print A
    for (size_t i = 0; i < a.Rows(); ++i) {
        for(size_t j=0;j < a.Columns();++j) {
            if (std::abs(a(i, j)) < eps)  {
                a(i, j) = 0;
            }
            std::cout<<std::setw(9)<<std::setprecision(6)<<a(i,j)<<' ';
        }
        std::cout<<'\n';
    }
}

void PrintExtended(Matrix& a , std::vector<double>& b) { // print A|b
    for (size_t i = 0; i < a.Rows(); ++i) {
        for(size_t j=0;j < a.Columns();++j) {
            if (std::abs(a(i, j)) < eps)  {
                a(i, j) = 0;
            }
            std::cout<<std::setw(9)<<std::setprecision(6)<<a(i,j)<<' ';
        }
        std::cout<<"  "<<std::setw(9)<<std::setprecision(6)<<b[i]<<'\n';
    }
}
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
