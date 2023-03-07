#include <iostream>
#include "LRUCache.h"

int main() {
  LRUCache<char, int> cache(4);

  cache.Put('a', 1);
  cache.Put('b', 2);
  cache.Put('c', 3);
  cache.Put('d', 4);
  std::cout << cache.Get('b') << std::endl;
  cache.Put('e', 5);
  cache.Put('f', 6);
  try {
    std::cout << cache.Get('c') << std::endl;
  }
  catch (std::runtime_error exc) {
    std::cout << exc.what() << std::endl;
  }
}