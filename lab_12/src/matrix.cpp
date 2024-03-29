#include "matrix.h"

#include <algorithm>
#include <cstring>

int **Matrix::alloc_2d_array(std::size_t r, std::size_t c) {
    if (r == 0 || c == 0)
        return nullptr;

    char *raw = static_cast<char *>(operator new(r * sizeof(int *) + r * c * sizeof(int), std::nothrow));
    if (raw == nullptr)
        throw MatrixException("Unable to allocate memory.");

    int *data = reinterpret_cast<int *>(raw + r * sizeof(int *));
    std::fill_n(data, r * c, 0);

    int **pointers = reinterpret_cast<int **>(raw);
    for (std::size_t i = 0; i < r; ++i)
        pointers[i] = data + i * c;
    return pointers;
}

Matrix::Matrix(std::size_t r, std::size_t c) : _rows(r), _cols(c) {
    _data = alloc_2d_array(r, c);
}

Matrix::Matrix() noexcept : Matrix(0u, 0u) {}

Matrix::Matrix(const Matrix &other) {
    *this = other;
}

Matrix::Matrix(Matrix &&other) noexcept {
    *this = std::move(other);
}

Matrix::~Matrix() {
    operator delete(_data);
}

void Matrix::swap(Matrix &other) {
    std::swap(_rows, other._rows);
    std::swap(_cols, other._cols);
    std::swap(_data, other._data);
}

// Я не буду делать swap-trick в копирующем присваивании, даже не надейтесь
Matrix &Matrix::operator=(const Matrix &other) {
    if (this == &other)
        return *this;

    if (_rows != other._rows || _cols != other._cols) {
        int **new_data = alloc_2d_array(other._rows, other._cols);
        operator delete(_data);
        _data = new_data;
    }

    memcpy(_data[0], other._data[0], other._rows * other._cols * sizeof(int));
    _rows = other._rows;
    _cols = other._cols;

    return *this;
}

Matrix &Matrix::operator=(Matrix &&other) noexcept {
    swap(other);
    return *this;
}

std::size_t Matrix::rows() const { return _rows; }

std::size_t Matrix::cols() const { return _cols; }

void Matrix::set(std::size_t i, std::size_t j, int val) {
    validate_indices_bounds(i, j);
    _data[i][j] = val;
}

int Matrix::get(std::size_t i, std::size_t j) const {
    validate_indices_bounds(i, j);
    return _data[i][j];
}

int *Matrix::operator[](std::size_t row) noexcept {
    return _data[row];
}

const int *Matrix::operator[](std::size_t row) const noexcept {
    return _data[row];
}

Matrix &Matrix::operator+=(const Matrix &other) {
    validate_add_bounds(other);
    for (std::size_t i = 0; i < rows(); ++i)
        for (std::size_t j = 0; j < cols(); ++j)
            _data[i][j] += other._data[i][j];
    return *this;
}

Matrix &Matrix::operator-=(const Matrix &other) {
    validate_add_bounds(other);
    for (std::size_t i = 0; i < rows(); ++i)
        for (std::size_t j = 0; j < cols(); ++j)
            _data[i][j] -= other._data[i][j];
    return *this;
}

Matrix &Matrix::operator*=(const Matrix &other) {
    return *this = *this * other;
}

Matrix operator*(const Matrix &a, const Matrix &b) {
    a.validate_mul_bounds(b);
    Matrix c(a.rows(), b.cols());
    for (std::size_t i = 0; i < a.rows(); ++i)
        for (std::size_t j = 0; j < b.cols(); ++j)
            for (std::size_t k = 0; k < a.cols(); ++k)
                c[i][j] += a[i][k] * b[k][j];
    return c;
}

Matrix operator+(Matrix a, const Matrix &b) {
    return a += b;
}

Matrix operator-(Matrix a, const Matrix &b) {
    return a -= b;
}

bool operator==(const Matrix &a, const Matrix &b) noexcept {
    if (a.rows() != b.rows() || a.cols() != b.cols())
        return false;
    for (std::size_t i = 0; i < a.rows(); ++i)
        for (std::size_t j = 0; j < a.cols(); ++j)
            if (a[i][j] != b[i][j])
                return false;
    return true;
}

bool operator!=(const Matrix &a, const Matrix &b) noexcept {
    return !(a == b);
}

std::ostream &operator<<(std::ostream &out, const Matrix &mat) {
    for (std::size_t i = 0; i < mat.rows(); ++i) {
        for (std::size_t j = 0; j < mat.cols(); ++j) {
            out << mat[i][j];
            if (j + 1 < mat.cols())
                out << " ";
        }
        out << std::endl;
    }
    return out;
}

std::ifstream &operator>>(std::ifstream &in, Matrix &mat) {
    if (!in.is_open())
        throw MatrixException("LOAD: unable to open file.");
    std::size_t rows, cols;

    if (!(in >> rows) || !(in >> cols))
        throw MatrixException("LOAD: invalid file format.");

    Matrix res(rows, cols);
    for (std::size_t i = 0; i < rows; ++i)
        for (std::size_t j = 0; j < cols; ++j)
            if (!(in >> res[i][j]))
                throw MatrixException("LOAD: invalid file format.");

    mat = std::move(res);
    return in;
}

void Matrix::validate_indices_bounds(std::size_t i, std::size_t j) const {
    if (i < 0u || j < 0u || i >= _rows || j >= _cols)
        throw MatrixException("ACCESS: bad index.");
}

void Matrix::validate_add_bounds(const Matrix &other) const {
    if (_rows != other.rows() || _cols != other.cols())
        throw MatrixException("ADD: dimensions do not match.");
}

void Matrix::validate_mul_bounds(const Matrix &other) const {
    if (_cols != other.rows())
        throw MatrixException("MUL: #arg1.columns != #arg2.rows.");
}

const char *MatrixException::what() const noexcept {
    return reason.c_str();
}