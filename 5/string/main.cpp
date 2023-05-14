#include <cstring>
#include <algorithm>
#include <iostream>

class String {
private:
  static const size_t SMALL_SIZE = 16;

  union {
    char* ptr_ = nullptr;
    char buff_[SMALL_SIZE];
  } data;
  size_t size_ = 0;
  size_t capacity_ = SMALL_SIZE;

  void Copy(const char* str, size_t offset = 0) {
    size_t len = std::strlen(str) + 1;
    Reserve(len + offset);
    if (IsShort()) {
      std::copy(str, str + len, data.buff_ + offset);
    }
    else {
      std::copy(str, str + len, data.ptr_ + offset);
    }
    size_ = len + offset;
  }

public:
  bool IsShort() const {
    return (capacity_ <= SMALL_SIZE);
  }

  String(){}

  String(const char* str) {
    Copy(str);
  }

  String(const String& other) {
    //Reserve(other.size_);
    if (other.IsShort()) {
      //std::copy(other.data, other.data + other.size_, data.ptr_);
      Copy(other.data.buff_);
    }
    else {
      Copy(other.data.ptr_);
    }
 
  }

  String(String&& other) noexcept {
    data = std::move(other.data);
    capacity_ = other.capacity_;
    size_ = other.size_;

    other.data.ptr_ = nullptr;
    other.capacity_ = other.size_ = 0;
  }

  String& operator=(const String& other) {
    if (other.IsShort()) {
      //std::copy(other.data, other.data + other.size_, data.ptr_);
      Copy(other.data.buff_);
    }
    else {
      Copy(other.data.ptr_);
    }
    return *this;
  }

  String& operator=(String&& other) noexcept {
    data = std::move(other.data);
    capacity_ = other.capacity_;
    size_ = other.size_;

    other.data.ptr_ = nullptr;
    other.capacity_ = other.size_ = 0;
    return *this;
  }

  char& operator[](size_t index) {
    if (IsShort()) {
      return data.buff_[index];
    }
    else {
      return data.ptr_[index];
    }
  }

  size_t Size() const {
    return size_;
  }

  size_t Capacity() const {
    return capacity_;
  }

  void Reserve(size_t new_capacity) {
    if ((new_capacity <= capacity_) or (new_capacity < SMALL_SIZE)) {
      return;
    }

    new_capacity *= 2;

    char* new_data = new char[new_capacity];

    if (IsShort()) {
      std::copy(data.buff_, data.buff_ + size_, new_data);
    }    
    else {
      std::copy(data.ptr_, data.ptr_ + size_, new_data);
      delete[] data.ptr_;
    }

    data.ptr_ = new_data;
    capacity_ = new_capacity;
  }

  void Append(const char* str) {
    Copy(str, size_);
  }

  void Clear() {
    if (!IsShort()) {
      delete[] data.ptr_;
      data.ptr_ = nullptr;
    }
    size_ = 0;
    capacity_ = 0;
  }

  friend std::ostream& operator<<(std::ostream& os, const String& s);

  ~String() {
    if (!IsShort()) {
      delete[] data.ptr_;
    }
  }
};

std::ostream& operator<<(std::ostream& os, const String& s) {
  //char* str = new char[s.size_];
  //std::cout << std::strlen(str) << '\n';
  //if (s.IsShort()) {
  //  std::copy(s.data.buff_, s.data.buff_ + s.size_, str);
  //}
  //else {
  //  std::copy(s.data.ptr_, s.data.ptr_ + s.size_, str);
  //}
  ////os << str;
  //os << s.size_;
  //delete[] str;
  //return os;
  for (size_t i = 0; i < s.size_; i++) {
    os << (s.IsShort() ? s.data.buff_[i] : s.data.ptr_[i]);
  }
  return os;
}

int main() {
  String s = "Hello world";
  //std::cout << s.Size() << '\n';
  std::cout << s << ' ' << s.IsShort() << '\n';
  s.Append("eeeeeee");
  std::cout << s << ' ' << s.IsShort() << '\n';

  auto s2 = s;
  std::cout << s2;
}

