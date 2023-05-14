#include <iostream>
#include <memory>
#include <exception>
#include <vector>

template <typename T, class Deleter>
struct ControlBlock{
  T* object;
  size_t counter;
  //Deleter* d;

  ControlBlock(T* ptr) : counter(1), object(ptr) /*d(new Deleter)*/ {};

  ~ControlBlock() {
    Deleter()(object);
    //delete object;
    //delete deleter;
  }
};

template <typename T, class Deleter = std::default_delete<T>>
class SharedPtr {
  T* ptr_ = nullptr;
  //size_t* counter_ = nullptr;
  //ControlBlock<T>* cb_ = nullptr;
  ControlBlock<T, Deleter>* cb_ = nullptr;

  SharedPtr(ControlBlock<T, Deleter>* cb) : ptr_(cb->object), cb_(cb) {}

  template <typename U, typename... Args>
  friend SharedPtr<U> MakeShared(Args&&...);

public:
  SharedPtr() {}
  //SharedPtr(T* ptr) : ptr_(ptr), cb_(new ControlBlock<T, std::default_delete<T>>(ptr)) {}

  SharedPtr(T* ptr) : ptr_(ptr), cb_(new ControlBlock<T, Deleter>(ptr)) {}

  SharedPtr(SharedPtr<T, Deleter>&& other) : ptr_(std::move(other.ptr_)), cb_(std::move(other.cb_)) {}

  const SharedPtr& operator=(SharedPtr<T, Deleter>&& other) {
    this->~SharedPtr();
    ptr_ = std::move(other.ptr_);
    cb_ = std::move(other.cb_);
    return *this;
  }
  
  SharedPtr(const SharedPtr<T, Deleter>& other) : ptr_(other.ptr_), cb_(other.cb_) {
    cb_->counter++;
  }

  const SharedPtr& operator=(const SharedPtr<T, Deleter>& other) {
    this->~SharedPtr();
    ptr_ = other.ptr_;
    cb_ = other.cb_;
    cb_->counter++;
    return *this;
  }

  size_t UseCount() {
    return cb_->counter;
  }

  T& operator*() {
    if (!ptr_) // nullptr case
      throw std::runtime_error("Error: attempt to dereference a nullptr");
    return *ptr_;
  }

  T* operator->() {
    if (!ptr_) // nullptr case
      throw std::runtime_error("Error: attempt to dereference a nullptr");
    return ptr_;
  }

  ~SharedPtr() {
    if (!cb_) // nullptr case
      return;

    cb_->counter--;
    if (!cb_->counter) {
      //(*cb_->deleter)(ptr_);
      //delete ptr_;
      delete cb_;
    }
  }
};

template <typename T, typename... Args>
SharedPtr<T, std::default_delete<T>> MakeShared(Args&&... args) {
  try {
    auto cb = new ControlBlock<T, std::default_delete<T>>(new T(std::forward<Args>(args)...));
    return SharedPtr<T, std::default_delete<T>>(cb);
  } catch (const std::bad_alloc& e) {
    std::cout << "Allocation failed" << std::endl;
    throw e;
  } catch (const std::length_error& e) {
    std::cout << "Allocation failed" << std::endl;
    throw e;
  }
}

template <typename T>
struct MyDeleter
{
  void operator()(T* p)
  {
    std::cout << "Variable " << *p << " was deleted with MyDeleter" << std::endl;
    free(p);
  }
};

void operator delete(void* p) {
  std::cout << "Variable " << *static_cast<int*>(p) << " was deleted with std::default_delete" << std::endl;
  free(p);
}

int main() {
  auto p1 = MakeShared<int>(1);
  auto p2 = MakeShared<int>(2);
  SharedPtr<int> p3(p2);

  std::cout << p3.UseCount() << std::endl;
  p2 = p1;
  std::cout << *p2 << std::endl;
  std::cout << p3.UseCount() << std::endl;

  SharedPtr<int> p4(std::move(p3));
  std::cout << p3.UseCount() << std::endl;
  std::cout << *p4 << std::endl;
  
  try {
    SharedPtr<int> p5;
    std::cout << *p5 << std::endl;
  } catch (const std::runtime_error& e) {
    std::cout << e.what() << std::endl;
  }

  try {
    auto p6 = MakeShared<std::vector<int>>(9999999999999999999);
  }
  catch (const std::length_error& e) {
    std::cout << e.what() << std::endl;
  }

  //MyDeleter<int> d;
  SharedPtr<int, MyDeleter<int>> p7(new int(7));
  auto p8 = p7;
  *p8 += 1;
}