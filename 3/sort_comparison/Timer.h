#pragma once
#include <string>
#include <chrono>
#include <iostream>

class Timer {
  const std::string id_;
  const std::chrono::steady_clock::time_point start_time_ = std::chrono::steady_clock::now();

public:
  Timer(std::string id) : id_(std::move(id)) {}

  ~Timer() {
    const auto end_time = std::chrono::steady_clock::now();
    const auto dur = end_time - start_time_;
    std::cout << id_ << ": ";
    std::cout << "operation time";
    std::cout << ": " << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count()
      << " ms" << std::endl;
  }
};
