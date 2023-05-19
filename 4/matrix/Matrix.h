#pragma once
#include <memory>
#include <initializer_list>
#include <type_traits>
#include <array>
#include <utility>

using namespace std;

template <typename T, size_t M, size_t N>
class Matrix {
private:
  array<array<T, N>, M> cont_;

public:
  Matrix() {
    cont_ = {};
  }

  Matrix(initializer_list<initializer_list<T>>);
  Matrix(const Matrix<T, M, N>& other);
  Matrix(Matrix<T, M, N>&& other);

  array<T, N>& operator[](size_t);
  const array<T, N>& operator[](size_t) const;

  Matrix<T, M, N>& operator+=(const Matrix<T, M, N>&);
  Matrix<T, M, N>& operator-=(const Matrix<T, M, N>&);
  Matrix<T, M, N>& operator=(const Matrix<T, M, N>&);
  Matrix<T, M, N>& operator=(Matrix<T, M, N>&&);

  /*~Matrix() {
    cont_.~array<array<T, N>, M>();
  }*/
};

template <typename T, size_t M, size_t N>
Matrix<T, M, N>::Matrix(initializer_list<initializer_list<T>> lst) {
  if (lst.size() != M) {
    delete this;
    throw(1);
  }

  auto row_it = lst.begin();
  for (size_t i = 0; i < M; i++, row_it++) {
    if (row_it->size() != N) {
      delete this;
      throw(1);
    }

    auto col_it = row_it->begin();
    for (size_t j = 0; j < N; j++, col_it++) {
      cont_[i][j] = *col_it;
    }
  }
}

template <typename T, size_t M, size_t N>
Matrix<T, M, N>::Matrix(const Matrix<T, M, N>& other) {
  //for (size_t i = 0; i < M; i++) {
  //  for (size_t j = 0; j < N; j++) {
  //    cont_[i][j] = other[i][j];
  //  }
  //}
  cont_ = other.cont_;
}

template <typename T, size_t M, size_t N>
Matrix<T, M, N>::Matrix(Matrix<T, M, N>&& other) {
  //for (size_t i = 0; i < M; i++) {
  //  for (size_t j = 0; j < N; j++) {
  //    cont_[i][j] = move(other[i][j]);
  //  }
  //}
  cont_ = move(other.cont_);
}

template <typename T, size_t M, size_t N>
array<T, N>& Matrix<T, M, N>::operator[](size_t col) {
  return cont_[col];
}

template <typename T, size_t M, size_t N>
const array<T, N>& Matrix<T, M, N>::operator[](size_t col) const {
  return cont_[col];
}

// Addition operator
template <typename T, size_t M, size_t N>
Matrix<T, M, N>&& operator+(const Matrix<T, M, N>& m1, const Matrix<T, M, N>& m2) {
  Matrix<T, M, N> result;
  for (size_t i = 0; i < M; i++) {
    for (size_t j = 0; j < N; j++) {
      result[i][j] = m1[i][j] + m2[i][j];
    }
  }
  return move(result);
}

// Subtraction operator
template <typename T, size_t M, size_t N>
Matrix<T, M, N>&& operator-(const Matrix<T, M, N>& m1, const Matrix<T, M, N>& m2) {
  Matrix<T, M, N> result;
  for (size_t i = 0; i < M; i++) {
    for (size_t j = 0; j < N; j++) {
      result[i][j] = m1[i][j] - m2[i][j];
    }
  }
  return move(result);
}

// Multiplication operator
template <typename T, size_t M, size_t N, size_t K>
Matrix<T, M, N>&& operator*(const Matrix<T, M, K>& m1, const Matrix<T, K, N>& m2) {
  Matrix<T, M, N> result;
  for (size_t i = 0; i < M; i++) {
    for (size_t j = 0; j < N; j++) {
      for (size_t k = 0; k < K; k++) {
        result[i][j] += m1[i][k] * m2[k][j];
      }
    }
  }
  return move(result);
}

// Addition operator
template <typename T, size_t M, size_t N>
Matrix<T, M, N>& Matrix<T, M, N>::operator+=(const Matrix<T, M, N>& other) {
  for (size_t i = 0; i < M; i++) {
    for (size_t j = 0; j < N; j++) {
      cont_[i][j] += other[i][j];
    }
  }
  return *this;
}

// Subtraction operator
template <typename T, size_t M, size_t N>
Matrix<T, M, N>& Matrix<T, M, N>::operator-=(const Matrix<T, M, N>& other) {
  for (size_t i = 0; i < M; i++) {
    for (size_t j = 0; j < N; j++) {
      cont_[i][j] -= other[i][j];
    }
  }
  return *this;
}

// Copy assignment operator
template <typename T, size_t M, size_t N>
Matrix<T, M, N>& Matrix<T, M, N>::operator=(const Matrix<T, M, N>& other) {
  cont_ = other.cont_;
  return *this;
}

// Move assignment operator
template <typename T, size_t M, size_t N>
Matrix<T, M, N>& Matrix<T, M, N>::operator=(Matrix<T, M, N>&& other) {
  cont_ = move(other.cont_);
  return *this;
}

template <typename>
struct MatrixShape {};

template <typename T, size_t M, size_t N>
struct MatrixShape<Matrix<T, M, N>> {
  using type = typename T;
  static constexpr size_t cols = N;
  static constexpr size_t rows = M;
};

template <typename T, size_t M, size_t N>
struct MatrixShape<Matrix<T, M, N>&> {
  using type = typename T;
  static constexpr size_t cols = N;
  static constexpr size_t rows = M;
};

template <typename T, size_t M, size_t N>
struct MatrixShape<Matrix<T, M, N>&&> {
  using type = typename T;
  static constexpr size_t cols = N;
  static constexpr size_t rows = M;
};

template <typename Mat>
Matrix<typename MatrixShape<Mat>::type, MatrixShape<Mat>::cols, MatrixShape<Mat>::rows>&& Transpose(Mat&& m) {
  Matrix<typename MatrixShape<Mat>::type, MatrixShape<Mat>::cols, MatrixShape<Mat>::rows> result;
  for (size_t i = 0; i < MatrixShape<Mat>::cols; i++) {
    for (size_t j = 0; j < MatrixShape<Mat>::rows; j++) {
      result[i][j] = m[j][i];
    }
  }
  return move(result);
}
