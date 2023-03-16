#include <iostream>
#include <random>
#include <set>
#include <fstream>
#include <chrono>
#include <vector>
#include <array>

#define MAX_TEST_SIZE     5e3

using test_t = int;

template <typename T>
T GenerateRandom(const T& left_bound, const T& right_bound) {
  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  
  if constexpr (std::is_integral_v<T>) {
    std::uniform_int_distribution<T> distrib(left_bound, right_bound);
    return distrib(gen);
  }
  else if constexpr (std::is_floating_point_v<T>) {
    std::uniform_real_distribution<T> distrib(left_bound, right_bound);
    return distrib(gen);
  }
  // else will be compilation error
};

template <typename T>
T GenerateRandom() {
  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()

  if constexpr (std::is_integral_v<T>) {
    std::uniform_int_distribution<T> distrib(-RAND_MAX, RAND_MAX);
    return distrib(gen);
  }
  else if constexpr (std::is_floating_point_v<T>) {
    std::uniform_real_distribution<T> distrib(-RAND_MAX, RAND_MAX);
    return distrib(gen);
  }
};

template <>
std::string GenerateRandom<std::string>() {
  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<> distrib(0, RAND_MAX);
  size_t len = distrib(gen);
  std::uniform_int_distribution<short> letter_dist(32, 126);
  std::string rand_str = "";
  
  for (size_t i = 0; i < len; i++) {
    rand_str.push_back(letter_dist(gen));
  }

  return std::move(rand_str);
}

template <typename T>
struct ContainerTester {
  static void Test (std::string&& file_name) {
    std::ofstream file;
    file.open(file_name);

    for (size_t j = 10; j < MAX_TEST_SIZE; j++) {
      const auto start_time = std::chrono::steady_clock::now();

      T cont;
      if constexpr (std::is_same_v<std::vector<T::value_type>, T>) {
        cont.resize(j);
      }

      for (size_t i = 0; i < j; i++) {
        cont[i] = (GenerateRandom<T::value_type>());
      }
      std::sort(cont.begin(), cont.end());

      const auto end_time = std::chrono::steady_clock::now();
      const auto dur = end_time - start_time;

      file << j << ", " << std::chrono::duration_cast<std::chrono::
        milliseconds>(dur).count() << std::endl;
    }
  }
};

template <typename T>
struct ContainerTester<std::set<T>> {
  static void Test (std::string&& file_name) {
    std::ofstream file;
    file.open(file_name);

    for (size_t j = 10; j < MAX_TEST_SIZE; j++) {
      const auto start_time = std::chrono::steady_clock::now();

      std::set<T> cont = {0};
      for (size_t i = 0; i < j; i++) {
        cont.insert(GenerateRandom<T>());
      }

      const auto end_time = std::chrono::steady_clock::now();
      const auto dur = end_time - start_time;

      file << j << ", " << std::chrono::duration_cast<std::chrono::
        milliseconds>(dur).count() << std::endl;
    }
  }
};

int main() {
  //std::string str = GenerateRandom<std::string>();
  //int num = GenerateRandom<int>();
  //std::cout << str << std::endl;
  
  //std::set<test_t> s;
  //std::ofstream set_file;
  //set_file.open()

  ContainerTester<std::set<test_t>>::Test("set_data.csv");
  ContainerTester<std::array<test_t, (size_t)MAX_TEST_SIZE>>::Test("array_data.csv");
  ContainerTester<std::vector<test_t>>::Test("vector_data.csv");
}

/*
Вывод: (см. graph.png) судя по графикам, вектор всегда является самым быстрым,
однако, при больших N array его догоняет и, возможно, обгонит при n > 5000. 
Array работает медленне всего при малых N, но затем set начинает сильно отставать
где-то после N = 1000.
*/