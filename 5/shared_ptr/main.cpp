#include <iostream>
#include <memory>
#include <exception>
#include <vector>

template <typename T>//, class Deleter>
struct ControlBlock{
private:
  /*struct BaseDeleterHolder {
    virtual ~BaseDeleterHolder() {}
    virtual const std::type_info& TypeInfo() const = 0;
  };*/

  /*template <class Deleter>
  struct DeleterHolder : BaseDeleterHolder {
    Deleter deleter;
    DeleterHolder(const Deleter& del) : deleter(del) {}
    const std::type_info& TypeInfo() const {
      return typeid(Deleter);
    }
  };*/

public:
  T* object;
  size_t counter;
  //BaseDeleterHolder* del_holder;
  //Deleter* deleter;

 /* ControlBlock(T* ptr) : counter(1), object(ptr), 
    deleter(&(new std::default_delete<T>)) {};*/

  //ControlBlock(T* ptr, Deleter* del) : counter(1), object(ptr), deleter(del) {};
  ControlBlock(T* ptr) : counter(1), object(ptr) {};

  /*template <class Deleter>
  const Deleter& GetDeleter() {
    if (typeid(Deleter) != del_holder->TypeInfo())
      throw std::runtime_error("Bad cast");
    return static_cast<DeleterHolder<Deleter>*>(del_holder)->deleter;
  }*/

  ~ControlBlock() {
    //*deleter(object);
    delete object;
    //delete deleter;
  }
};

template <typename T>
class SharedPtr {
  T* ptr_ = nullptr;
  //size_t* counter_ = nullptr;
  ControlBlock<T>* cb_ = nullptr;

  SharedPtr(ControlBlock<T>* cb) : ptr_(cb->object), cb_(cb) {}

  template <typename U, typename... Args>
  friend SharedPtr<U> MakeShared(Args&&...);

public:
  SharedPtr() {}
  //SharedPtr(T* ptr) : ptr_(ptr), cb_(new ControlBlock<T, std::default_delete<T>>(ptr, new std::default_delete<T>)) {}
  SharedPtr(T* ptr) : ptr_(ptr), cb_(new ControlBlock<T>(ptr)) {}
  //template <class Deleter>
  //SharedPtr(T* ptr, const Deleter& d) : ptr_(ptr), cb_(new ControlBlock<T, Deleter>(ptr, &d)) {}


  SharedPtr(SharedPtr<T>&& other) : ptr_(std::move(other.ptr_)), cb_(std::move(other.cb_)) {}

  const SharedPtr& operator=(SharedPtr<T>&& other) {
    this->~SharedPtr();
    ptr_ = std::move(other.ptr_);
    cb_ = std::move(other.cb_);
    return *this;
  }
  
  SharedPtr(const SharedPtr<T>& other) : ptr_(other.ptr_), cb_(other.cb_) {
    cb_->counter++;
  }

  const SharedPtr& operator=(const SharedPtr<T>& other) {
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
SharedPtr<T> MakeShared(Args&&... args) {
  try {
    auto cb = new ControlBlock<T>(new T(std::forward<Args>(args)...));
    return SharedPtr<T>(cb);
  } catch (const std::bad_alloc& e) {
    std::cout << "Allocation failed" << std::endl;
    throw e;
  } catch (const std::length_error& e) {
    std::cout << "Allocation failed" << std::endl;
    throw e;
  }
}

int main() {
  auto p1 = MakeShared<int>(5);
  auto p2 = MakeShared<int>(6);
  SharedPtr<int> p3(p2);

  std::cout << p3.UseCount() << std::endl;
  p2 = p1;
  std::cout << *p2 << std::endl;
  std::cout << p3.UseCount() << std::endl;

  SharedPtr<int> p4(std::move(p3));
  std::cout << p3.UseCount() << std::endl;
  std::cout << *p4 << std::endl;

  SharedPtr<int> p5;
  try {
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
}