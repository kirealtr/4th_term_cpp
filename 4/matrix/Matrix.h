#pragma once
#include <memory>
#include <initializer_list>
#include <type_traits>
#include <array>
#include <utility>
#include <vector>
#include <deque>
#define  CONT_TYPE (UnPackCont<Cont, T, M, N>::name)

using namespace std;

template <class Cont, typename T, size_t M, size_t N>
struct UnPackCont {
  using type = array<array<T, N>, M>;
  static constexpr char name = 'a';
};

template <typename T, size_t M, size_t N>
struct UnPackCont<vector<T>, T, M, N> {
  using type = vector<vector<T>>;
  static constexpr char name = 'v';
};

template <typename T, size_t M, size_t N>
struct UnPackCont<deque<T>, T, M, N> {
  using type = deque<deque<T>>;
  static constexpr char name = 'd';
};

template <class Cont, typename T, size_t M, size_t N>
class Matrix {
private:
  //Cont cont_;
  /*if constexpr (is_same_v(Cont, vector<T>)) {
     cont_;
    const char CONT_TYPE = 'v';
  }
  else if constexpr (is_same_v(Cont, deque<T>)) {
    deque<deque<T>> cont_;
    const char CONT_TYPE = 'd';
  }
  else {
    array<array<T, N>, M> cont_;
    const char CONT_TYPE = 'a';
  }*/
  typename UnPackCont<Cont, T, M, N>::type cont_;

public:
  Matrix() {
    if constexpr (CONT_TYPE == 'v') {
      cont_ = vector<vector<T>>(M);
    }
    else if constexpr (CONT_TYPE == 'd') {
      cont_ = deque<deque<T>>(M);
    }
    else 
      cont_ = {};


    for (size_t i = 0; i < M; i++) {
      if constexpr (CONT_TYPE == 'v') {
        cont_[i] = vector<T>(N);
      }
      else if constexpr (CONT_TYPE == 'd') {
        cont_[i] = deque<T>(N);
      }
    }
  }

  Matrix(initializer_list<initializer_list<T>>);
  Matrix(const Matrix<Cont, T, M, N>& other);
  Matrix(Matrix<Cont, T, M, N>&& other);

  Cont& operator[](size_t);
  const Cont& operator[](size_t) const;

  Matrix<Cont, T, M, N>& operator+=(const Matrix<Cont, T, M, N>&);
  Matrix<Cont, T, M, N>& operator-=(const Matrix<Cont, T, M, N>&);
  Matrix<Cont, T, M, N>& operator=(const Matrix<Cont, T, M, N>&);
  Matrix<Cont, T, M, N>& operator=(Matrix<Cont, T, M, N>&&);

  /*~Matrix() {
    cont_.~array<array<T, N>, M>();
  }*/
};

template <class Cont, typename T, size_t M, size_t N>
Matrix<Cont, T, M, N>::Matrix(initializer_list<initializer_list<T>> lst) : Matrix() {
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

template <class Cont, typename T, size_t M, size_t N>
Matrix<Cont, T, M, N>::Matrix(const Matrix<Cont, T, M, N>& other) {
  //for (size_t i = 0; i < M; i++) {
  //  for (size_t j = 0; j < N; j++) {
  //    cont_[i][j] = other[i][j];
  //  }
  //}
  cont_ = other.cont_;
}

template <class Cont, typename T, size_t M, size_t N>
Matrix<Cont, T, M, N>::Matrix(Matrix<Cont, T, M, N>&& other) {
  //for (size_t i = 0; i < M; i++) {
  //  for (size_t j = 0; j < N; j++) {
  //    cont_[i][j] = move(other[i][j]);
  //  }
  //}
  cont_ = move(other.cont_);
}

template <class Cont, typename T, size_t M, size_t N>
Cont& Matrix<Cont, T, M, N>::operator[](size_t col) {
  return cont_[col];
}

template <class Cont, typename T, size_t M, size_t N>
const Cont& Matrix<Cont, T, M, N>::operator[](size_t col) const {
  return cont_[col];
}

// Addition operator
template <class Cont, typename T, size_t M, size_t N>
Matrix<Cont, T, M, N> operator+(const Matrix<Cont, T, M, N>& m1, const Matrix<Cont, T, M, N>& m2) {
  Matrix<Cont, T, M, N> result;
  for (size_t i = 0; i < M; i++) {
    for (size_t j = 0; j < N; j++) {
      result[i][j] = m1[i][j] + m2[i][j];
    }
  }
  return (result);
}

// Subtraction operator
template <class Cont, typename T, size_t M, size_t N>
Matrix<Cont, T, M, N> operator-(const Matrix<Cont, T, M, N>& m1, const Matrix<Cont, T, M, N>& m2) {
  Matrix<Cont, T, M, N> result;
  for (size_t i = 0; i < M; i++) {
    for (size_t j = 0; j < N; j++) {
      result[i][j] = m1[i][j] - m2[i][j];
    }
  }
  return (result);
}

// Multiplication operator
template <class Cont, typename T, size_t M, size_t N, size_t K>
Matrix<Cont, T, M, N> operator*(const Matrix<Cont, T, M, K>& m1, const Matrix<Cont, T, K, N>& m2) {
  Matrix<Cont, T, M, N> result;
  for (size_t i = 0; i < M; i++) {
    for (size_t j = 0; j < N; j++) {
      for (size_t k = 0; k < K; k++) {
        result[i][j] += m1[i][k] * m2[k][j];
      }
    }
  }
  return (result);
}

// Addition operator
template <class Cont, typename T, size_t M, size_t N>
Matrix<Cont, T, M, N>& Matrix<Cont, T, M, N>::operator+=(const Matrix<Cont, T, M, N>& other) {
  for (size_t i = 0; i < M; i++) {
    for (size_t j = 0; j < N; j++) {
      cont_[i][j] += other[i][j];
    }
  }
  return *this;
}

// Subtraction operator
template <class Cont, typename T, size_t M, size_t N>
Matrix<Cont, T, M, N>& Matrix<Cont, T, M, N>::operator-=(const Matrix<Cont, T, M, N>& other) {
  for (size_t i = 0; i < M; i++) {
    for (size_t j = 0; j < N; j++) {
      cont_[i][j] -= other[i][j];
    }
  }
  return *this;
}

// Copy assignment operator
template <class Cont, typename T, size_t M, size_t N>
Matrix<Cont, T, M, N>& Matrix<Cont, T, M, N>::operator=(const Matrix<Cont, T, M, N>& other) {
  cont_ = other.cont_;
  return *this;
}

// Move assignment operator
template <class Cont, typename T, size_t M, size_t N>
Matrix<Cont, T, M, N>& Matrix<Cont, T, M, N>::operator=(Matrix<Cont, T, M, N>&& other) {
  cont_ = move(other.cont_);
  return *this;
}

template <typename>
struct MatrixShape {};

template <class Cont, typename T, size_t M, size_t N>
struct MatrixShape<Matrix<Cont, T, M, N>> {
  using data_type = typename T;
  using cont_type = typename Cont;
  static constexpr size_t cols = N;
  static constexpr size_t rows = M;
};

template <class Cont, typename T, size_t M, size_t N>
struct MatrixShape<Matrix<Cont, T, M, N>&> {
  using data_type = typename T;
  using cont_type = typename Cont;
  static constexpr size_t cols = N;
  static constexpr size_t rows = M;
};

template <class Cont, typename T, size_t M, size_t N>
struct MatrixShape<Matrix<Cont, T, M, N>&&> {
  using data_type = typename T;
  using cont_type = typename Cont;
  static constexpr size_t cols = N;
  static constexpr size_t rows = M;
};

template <typename Mat>
Matrix<typename MatrixShape<Mat>::cont_type, typename MatrixShape<Mat>::data_type, MatrixShape<Mat>::cols, MatrixShape<Mat>::rows> Transpose(Mat&& m) {
  Matrix<typename MatrixShape<Mat>::cont_type, typename MatrixShape<Mat>::data_type, MatrixShape<Mat>::cols, MatrixShape<Mat>::rows> result;
  //auto result;
  for (size_t i = 0; i < MatrixShape<Mat>::cols; i++) {
    for (size_t j = 0; j < MatrixShape<Mat>::rows; j++) {
      result[i][j] = m[j][i];
    }
  }
  return (result);
}
