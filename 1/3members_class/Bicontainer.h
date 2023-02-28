#pragma once
#include <vector>
#include "Timer.h"

template <typename T>
class Bicontainer {
  T* arr_;
  size_t sz_;
  std::vector<T> v_;

public:
//  Bicontainer(std::initializer_list<T> array, size_t size, std::vector<T> vec) : arr(array), sz(size), v(vec) {}
//  // assignment constructor with initializer list for simple types

  Bicontainer(T* array = nullptr, size_t size = 0, const std::vector<T>& vec = {}) : arr_(array), sz_(size), v_(vec) {}
    // assignment constructor + defaut constructor (size should be equal to array size!)

  Bicontainer(T* array, size_t size, std::vector<T>&& vec) : arr_(array), sz_(size), v_(std::move(vec)) {}

  size_t Size() { return sz_; }

  Bicontainer(const Bicontainer<T>& other);
  Bicontainer(Bicontainer<T>&& other);

  Bicontainer& operator=(const Bicontainer<T>& other);
  Bicontainer& operator=(Bicontainer<T>&& other);

  ~Bicontainer();
};

template <typename T>
Bicontainer<T>::Bicontainer(const Bicontainer<T>& other) {
  Timer t("Copy constructor");

  sz_ = other.sz_;

  arr_ = reinterpret_cast<T*>(new int8_t[sz_ * sizeof(T)]); // allocate memory for arr
  for (size_t i = 0; i < sz_; i++)
    new(arr_ + i) T(other.arr_[i]); // create arr members using T copy constructor

  v_ = other.v_;
}

template <typename T>
Bicontainer<T>::Bicontainer(Bicontainer<T>&& other) {
  Timer t("Move constructor");

  sz_ = other.sz_;

  arr_ = reinterpret_cast<T*>(new int8_t[sz_ * sizeof(T)]); // allocate memory for arr
  for (size_t i = 0; i < sz_; i++)
    new(arr_ + i) T(std::move(other.arr_[i])); // create arr members using T move constructor

  v_ = std::move(other.v_);
}

template <typename T>
Bicontainer<T>& Bicontainer<T>::operator=(const Bicontainer<T>& other) {
  Timer t("Copy assignment");

  if (sz_ > 0) {
    for (size_t i = 0; i < sz_; i++)
      (arr_ + i)->~T(); // explicitly delete arr members

    delete[] reinterpret_cast<int8_t*>(arr_); // free memory
  }

  sz_ = other.sz_;

  arr_ = reinterpret_cast<T*>(new int8_t[sz_ * sizeof(T)]); // allocate memory for arr
  for (size_t i = 0; i < sz_; i++)
    new(arr_ + i) T(other.arr_[i]); // create arr members using T copy constructor

  v_ = other.v_;

  return *this;
}

template <typename T>
Bicontainer<T>& Bicontainer<T>::operator=(Bicontainer<T>&& other) {
  Timer t("Move assignment");

  if (sz_ > 0) {
    for (size_t i = 0; i < sz_; i++)
      (arr_ + i)->~T(); // explicitly delete arr members

    delete[] reinterpret_cast<int8_t*>(arr_); // free memory
  }

  sz_ = other.sz_;

  arr_ = reinterpret_cast<T*>(new int8_t[sz_ * sizeof(T)]); // allocate memory for arr
  for (size_t i = 0; i < sz_; i++)
    new(arr_ + i) T(std::move(other.arr_[i])); // create arr members using T move constructor

  v_ = std::move(other.v_);

  return *this;
}

template <typename T>
Bicontainer<T>::~Bicontainer() {
  if (sz_ > 0) {
    for (size_t i = 0; i < sz_; i++)
      (arr_ + i)->~T(); // explicitly delete arr members

    delete[] reinterpret_cast<int8_t*>(arr_); // free memory
  }

  v_.~vector(); // delete v
  sz_ = 0;
}

