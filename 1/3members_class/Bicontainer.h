#pragma once
#include <vector>

template <typename T>
class Bicontainer {
  T* arr;
  size_t sz;
  std::vector<T> v;

public:
//  Bicontainer(std::initializer_list<T> array, size_t size, std::vector<T> vec) : arr(array), sz(size), v(vec) {}
//  // assignment constructor with initializer list for simple types

  Bicontainer(T* array = nullptr, size_t size = 0, std::vector<T> vec = {}) : arr(array), sz(size), v(vec) {}
    // assignment constructor + defaut constructor

  size_t Size() { return sz; }

  Bicontainer(const Bicontainer<T>& other);

  Bicontainer& operator=(const Bicontainer<T>& other);

  ~Bicontainer();
};

template <typename T>
Bicontainer<T>::Bicontainer(const Bicontainer<T>& other) {
  sz = other.sz;

  arr = reinterpret_cast<T*>(new uint8_t(sz * sizeof(T))); // allocate memory for arr
  for (size_t i = 0; i < sz; i++)
    arr[i] = T(other.arr[i]); // create arr members using T copy constructor

  v = other.v;
}

template <typename T>
Bicontainer<T>& Bicontainer<T>::operator=(const Bicontainer<T>& other) {
  for (size_t i = 0; i < sz; i++)
    arr[i].~T(); // explicitly delete arr members

  delete[] arr; // free memory

  sz = other.sz;

  arr = reinterpret_cast<T*>(new uint8_t[sz * sizeof(T)]); // allocate memory for arr
  for (size_t i = 0; i < sz; i++)
    arr[i] = T(other.arr[i]); // create arr members using T copy constructor

  v = other.v;

  return *this;
}

template <typename T>
Bicontainer<T>::~Bicontainer() {
  for (size_t i = 0; i < sz; i++)
    arr[i].~T(); // explicitly delete arr members

  delete[] arr; // free memory
  v.~vector(); // delete v
}

