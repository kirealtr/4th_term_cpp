#include <iostream>
#include <initializer_list>
#include "Matrix.h"

int main() {
  Matrix<int, 2, 2> m;
  m[0][1] = 5;
  std::cout << m[0][1] << std::endl;

  Matrix<int, 2, 2> m1 = {
    {1, 2},
    {3, 4}
  };

  Matrix<int, 2, 2> m2;
  m2 = m + m1;
  m = m1 * m2;
  m = Transpose(std::move(m2));
  std::cout << m[1][0] << std::endl;
  m = Transpose(m1);
  std::cout << m[1][0] << std::endl;

  Matrix<int, 3, 1> m3 = {
    {1},
    {3},
    {4}
  };

  Matrix<int, 1, 3> m4 = {
    {1, 1, 1 }
  };

  std::cout << (m4 * m3)[0][0] << std::endl;
  //std::cout << (m4 * m3)[0][1] << std::endl; // RE: array index out of range
  //std::cout << (m4 * m2)[0][0] << std::endl; // CE: no acceptable operator defined
  //std::cout << (m4 + m3)[0][0] << std::endl; // CE: no acceptable operator defined

  Matrix<int, 1, 3> m5(m4);
  Matrix<int, 1, 3> m6(std::move(m4));
  std::cout << m6[0][2] << std::endl;
  std::cout << m4[0][2] << std::endl;

}