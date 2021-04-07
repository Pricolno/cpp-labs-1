#ifndef LAB_12_MATRIX_H
#define LAB_12_MATRIX_H

#include <cstdio>
#include <cstddef>
#include <iostream>
#include <exception>
#include <fstream>

class Matrix {
public:
    explicit Matrix() noexcept;
    Matrix(std::size_t r, std::size_t c);

    Matrix(const Matrix &other);
    Matrix(Matrix &&other) noexcept;

    Matrix &operator=(const Matrix &other);
    Matrix &operator=(Matrix &&other) noexcept;

    void swap(Matrix &other);

    std::size_t rows() const;
    std::size_t cols() const;

    void set(std::size_t i, std::size_t j, int val);
    int get(std::size_t i, std::size_t j) const;

    int *operator[](std::size_t row) noexcept;
    const int *operator[](std::size_t row) const noexcept;

    friend Matrix operator*(const Matrix &a, const Matrix &b);
    friend Matrix operator+(Matrix a, const Matrix &b);
    friend Matrix operator-(Matrix a, const Matrix &b);

    friend bool operator==(const Matrix &a, const Matrix &b) noexcept;
    friend bool operator!=(const Matrix &a, const Matrix &b) noexcept;

    Matrix &operator+=(const Matrix &other);
    Matrix &operator-=(const Matrix &other);
    Matrix &operator*=(const Matrix &other);

    friend std::ostream &operator<<(std::ostream &out, const Matrix &mat);
    friend std::ifstream &operator>>(std::ifstream &in, Matrix &mat);

    ~Matrix();

private:
    static int **alloc_2d_array(std::size_t r, std::size_t c);

    void validate_indices_bounds(std::size_t i, std::size_t j) const;
    void validate_add_bounds(const Matrix &other) const;
    void validate_mul_bounds(const Matrix &other) const;

    std::size_t _rows = 0;
    std::size_t _cols = 0;
    int **_data = nullptr;
};

/**
 * Отнаследовано от базового класса, потому что
 * MatrixException не относится ни к одному конкретному
 * базовому классу, т.к. содержит все эти типы ошибок
 * одновременно.
 */
class MatrixException : public std::exception {
public:
    explicit MatrixException(std::string reason) : reason(std::move(reason)) {}

    const char *what() const noexcept override;

private:
    std::string reason;
};

#endif //LAB_12_MATRIX_H
