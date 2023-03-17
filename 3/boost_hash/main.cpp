#include <iostream>
#include <random>
#include <fstream>
#include <functional>
#include <array>
#include <unordered_set>
#include <string>
#include "StringHashFunctions.h"

#define UNIFORMITY_TEST_SIZE      1e3
#define MAX_RANDOM_STRING_LENGH   1e2
#define HASHES_COUNT              9
#define COLLISIONS_TEST_SIZE      1e4

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

  std::uniform_int_distribution<> distrib(0, MAX_RANDOM_STRING_LENGH);
  size_t len = distrib(gen);
  std::uniform_int_distribution<short> letter_dist(32, 126);
  std::string rand_str = "";

  for (size_t i = 0; i < len; i++) {
    rand_str.push_back(letter_dist(gen));
  }

  return std::move(rand_str);
}

template <typename Func>
void TestHashUniformity(Func& Hash, std::ofstream& file) {
  /*std::ofstream file;
  file.open(file_name);*/

  for (int i = 0; i < UNIFORMITY_TEST_SIZE - 1; i++) {
    std::string rand_str = GenerateRandom<std::string>();
    file << Hash(rand_str.c_str(), rand_str.size()) << ", ";
  }
  std::string rand_str = GenerateRandom<std::string>();
  file << Hash(rand_str.c_str(), rand_str.size()) << std::endl;
}

template <typename Func>
void TestHashCollisions(Func& Hash, std::ofstream& file) {
  std::unordered_set<unsigned int> hash_set;
  //std::array<unsigned int, COLLISIONS_TEST_SIZE> collisions_count;
  unsigned int collision_counter = 0;
  for (int i = 1; i < COLLISIONS_TEST_SIZE; i++) {
    std::string rand_str = GenerateRandom<std::string>();
    collision_counter += !std::get<1>(hash_set.insert(Hash(rand_str.c_str(), rand_str.size())));
    //if hash already exist, false will be returned, so we increment collision counter

    file << i << ", " << collision_counter << std::endl;
  }
}

int main() {
  std::array<std::function<unsigned int(const char*, unsigned int)>, HASHES_COUNT> 
    hashes = {
    RSHash,
    JSHash,
    PJWHash,
    ELFHash,
    BKDRHash,
    SDBMHash,
    DJBHash,
    DEKHash,
    APHash
  };

  std::array<std::string, HASHES_COUNT> hash_labels = {
    "RSHash",
    "JSHash",
    "PJWHash",
    "ELFHash",
    "BKDRHash",
    "SDBMHash",
    "DJBHash",
    "DEKHash",
    "APHash"
  };

  // Uniformity test
  std::ofstream file;
  file.open("data.csv");
  for (int i = 0; i < HASHES_COUNT; i++) {
    file << hash_labels[i] << ", ";
    TestHashUniformity(hashes[i], file);
  }
  file.close();

  for (int i = 0; i < HASHES_COUNT; i++) {
    file.open("collision_test_data/" + std::to_string(i) + ".csv");
    file << hash_labels[i] << "," << std::endl;
    TestHashCollisions(hashes[i], file);
    file.close();
  }
}