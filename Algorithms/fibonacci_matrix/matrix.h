#include <vector>

template<class T>
class Matrix {
public:
    Matrix() : a_() {
        rows_ = 0;
        cols_ = 0;
    }
    Matrix(size_t rows, size_t cols) : rows_(rows), cols_(cols) {
        for (size_t i = 0; i < rows; ++i) {
            std::vector<T> zero(cols, 0.0);
            a_.push_back(zero);
        }
    }

    Matrix(size_t rows) : Matrix(rows, rows) {}

    Matrix(const std::vector<std::vector<T>> a) : rows_(a.size()), cols_(a[0].size()), a_(a) {}

    Matrix(const Matrix& rhs) : rows_(rhs.rows_), cols_(rhs.cols_), a_(rhs.a_) {}

    size_t Rows() const {
        return rows_;
    }

    size_t Columns() const {
        return cols_;
    }

    const T& operator()(size_t i, size_t j) const {
        return a_[i][j];
    }

    T& operator()(size_t i, size_t j) {
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

    Matrix operator*(Matrix<T>& rhs) {
        Matrix c(rows_, rhs.Columns());
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < rhs.Columns(); ++j) {
                for (size_t k = 0; k < cols_; ++k) {
                    c(i, j) += this->a_[i][k] * rhs(k, j);
                }
            }
        }
        return c;
    }

    Matrix& operator*=(Matrix& rhs) {
        return *this = *this * rhs;
    }

private:
    size_t rows_;
    size_t cols_;
    std::vector<std::vector<T>> a_;
};

template<class T>
Matrix<T> Identity(size_t n) {
    Matrix<T> e(n, n);
    for (size_t i = 0; i < n; ++i) {
        e(i, i) = 1;
    }
    return e;
}
