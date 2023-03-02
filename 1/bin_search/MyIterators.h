#pragma once
#include <vector>
#include <iterator>

template <typename T>
class ForwardIterator : public std::iterator<std::forward_iterator_tag, T> {
protected:  // inheritance from std::iterator to set iterator_category
  using Iter_t = typename std::vector<T>::iterator;
  Iter_t iter_;
public:
  ForwardIterator(const Iter_t& std_iter) : iter_(std_iter) {}
  ForwardIterator(Iter_t&& std_iter) : iter_(std::move(std_iter)) {}

  ForwardIterator<T> operator=(const Iter_t& std_iter) {
    iter_ = std_iter;
    return *this;
  }
  ForwardIterator<T> operator=(Iter_t&& std_iter) {
    iter_ = std_iter;
    return *this;
  }

  ForwardIterator<T> operator++() {
    ++iter_;
    return *this;
  }

  T& operator*() {
    return *iter_;
  }
};

template <typename T>
class BidirectionalIterator : public ForwardIterator<T> {
public:
  using ForwardIterator<T>::ForwardIterator;

  ForwardIterator<T> operator--() {
    --this->iter_;
    return *this;
  }
};

namespace std
{
  template <typename T>
  struct iterator_traits<BidirectionalIterator<T>>
  {
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef void value_type;
    typedef void difference_type;
  };
} // other way to set iterator_category because double inheritance doesn't work
