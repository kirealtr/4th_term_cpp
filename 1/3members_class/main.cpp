#include "Bicontainer.h"
#include <vector>
#include <iostream>

int main() {
  std::vector<int> v1({ 3, 4, 5 });
  int* ptr = new int[2];
  ptr[0] = 1; ptr[1] = 2;

  Bicontainer<int> bc(ptr, 2, v1);
  Bicontainer<int> bc2;
  bc2 = bc;

  std::cout << bc2.Size();
}