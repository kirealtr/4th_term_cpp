#include "Bicontainer.h"
#include <vector>
#include <iostream>

#define REALLY_BIG_SIZE     5e7

int main() {
  std::vector<int> v1(REALLY_BIG_SIZE, 0);
  int* ptr = new int[REALLY_BIG_SIZE]{ 0 };

  Bicontainer<int> bc1(ptr, REALLY_BIG_SIZE, std::move(v1));

  std::cout << "Constructor test: " << std::endl;
  Bicontainer<int> bc2(bc1);
  Bicontainer<int> bc3(std::move(bc2));
  bc2.~Bicontainer();
  bc3.~Bicontainer();

  std::cout << std::endl;

  std::cout << "Assignment test: " << std::endl;
  bc2 = bc1;
  bc3 = std::move(bc2);
  bc2.~Bicontainer();
  bc3.~Bicontainer();

}