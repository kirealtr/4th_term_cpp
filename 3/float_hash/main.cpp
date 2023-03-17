#include <string>
#include <iostream>

unsigned int APHash(const char* str, unsigned int length)
{
  unsigned int hash = 0xAAAAAAAA;
  unsigned int i = 0;

  for (i = 0; i < length; ++str, ++i)
  {
    hash ^= ((i & 1) == 0) ? ((hash << 7) ^ (*str) * (hash >> 3)) :
      (~((hash << 11) + ((*str) ^ (hash >> 5))));
  }

  return hash;
}

template <typename T>
unsigned int FloatingPointHash(const T& num) {
  return APHash(std::to_string(num).c_str(), std::to_string(num).size());
}

int main() {
  float pi = 3.14159265358979;
  std::cout << FloatingPointHash(pi) << std::endl;
  std::cout << FloatingPointHash(sqrt(2)) << std::endl;
}