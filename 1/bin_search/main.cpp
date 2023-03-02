#include "Timer.h"
#include "MyIterators.h"
#include <vector>
#include <iterator>
#include <iostream>

#define BIG_SIZE      1e7
#define ITEM_TO_FIND  1e5

template <typename T, typename Iterator, typename IterCategory>
void MoveIteratorHelper(std::vector<T>& v, Iterator& iter, int& iter_pos, int new_pos, IterCategory) {
  if (iter_pos > new_pos) {
    iter = v.begin();
    iter_pos = 0;
  }

  for (; iter_pos < new_pos; iter_pos++, ++iter);
}

template <typename T, typename Iterator>
void MoveIteratorHelper(std::vector<T>& v, Iterator& iter, int& iter_pos, int new_pos, std::bidirectional_iterator_tag) {
  if (iter_pos > new_pos)
    for (; iter_pos > new_pos; iter_pos--, --iter);
  else
    for (; iter_pos < new_pos; iter_pos++, ++iter);
}

template <typename T, typename Iterator>
void MoveIteratorHelper(std::vector<T>& v, Iterator& iter, int& iter_pos, int new_pos, std::random_access_iterator_tag) {
  iter += new_pos - iter_pos;
  iter_pos = new_pos;
}

template <typename T, typename Iterator>
void MoveIterator(std::vector<T>& v, Iterator& iter, int& iter_pos, int new_pos) {
  MoveIteratorHelper<T, Iterator>(v, iter, iter_pos, new_pos, typename std::iterator_traits<Iterator>::iterator_category());
}

/*
template <typename T, typename Iterator, typename IterCategory>
int BinSearchHelper(std::vector<T>& v, const T& obj, IterCategory) {
  Timer t("Forward iterator binsearch");

  int n = v.size();
  Iterator iter = v.begin();
  int iter_pos = 0;
  int left_bound = 0, right_bound = n;

  MoveIterator<T, Iterator>(v, iter, iter_pos, (right_bound + left_bound) / 2);
  while (left_bound < right_bound) {
    if (*iter >= obj) {
      right_bound = (right_bound + left_bound) / 2;
    }
    else {
      left_bound = (right_bound + left_bound) / 2 + 1;
      if (left_bound >= n)
        return -1;
    }
    MoveIterator<T, Iterator>(v, iter, iter_pos, (right_bound + left_bound) / 2);
  }
  if (*iter == obj)
    return iter_pos;
  else
    return -1;
}

template <typename T, typename Iterator>
int BinSearchHelper(std::vector<T>& v, const T& obj, std::bidirectional_iterator_tag) {
  Timer t("Bidirectional iterator binsearch");

  int n = v.size();
  Iterator iter = v.begin();
  int iter_pos = 0;
  int left_bound = 0, right_bound = n;

  MoveIterator<T, Iterator>(v, iter, iter_pos, (right_bound + left_bound) / 2);
  while (left_bound < right_bound) {
    if (*iter >= obj) {
      right_bound = (right_bound + left_bound) / 2;
    }
    else {
      left_bound = (right_bound + left_bound) / 2 + 1;
      if (left_bound >= n)
        return -1;
    }
    MoveIterator<T, Iterator>(v, iter, iter_pos, (right_bound + left_bound) / 2);
  }
  if (*iter == obj)
    return iter_pos;
  else
    return -1;
}

template <typename T, typename Iterator>
int BinSearchHelper(std::vector<T>& v, const T& obj, std::random_access_iterator_tag) {
  Timer t("Random access iterator binsearch");

  int n = v.size();
  Iterator iter = v.begin();
  int iter_pos = 0;
  int left_bound = 0, right_bound = n;

  MoveIterator<T, Iterator>(v, iter, iter_pos, (right_bound + left_bound) / 2);
  while (left_bound < right_bound) {
    if (*iter >= obj) {
      right_bound = (right_bound + left_bound) / 2;
    }
    else {
      left_bound = (right_bound + left_bound) / 2 + 1;
      if (left_bound >= n)
        return -1;
    }
    MoveIterator<T, Iterator>(v, iter, iter_pos, (right_bound + left_bound) / 2);
  }

  if (*iter == obj)
    return iter_pos;
  else
    return -1;
}*/

template <typename T, typename Iterator>
int BinSearch(std::vector<T>& v, const T& obj) {
  int n = v.size();
  Iterator iter = v.begin();
  int iter_pos = 0;
  int left_bound = 0, right_bound = n;

  MoveIterator<T, Iterator>(v, iter, iter_pos, (right_bound + left_bound) / 2);
  while (left_bound < right_bound) {
    if (*iter >= obj) {
      right_bound = (right_bound + left_bound) / 2;
    }
    else {
      left_bound = (right_bound + left_bound) / 2 + 1;
      if (left_bound >= n)
        return -1;
    }
    MoveIterator<T, Iterator>(v, iter, iter_pos, (right_bound + left_bound) / 2);
  }

  //iter_pos++, ++iter; // now iter_pos = left_bound = right_bound
  if (*iter == obj)
    return iter_pos;
  else
    return -1;
}

int main() {
  std::vector<int> v(BIG_SIZE, 0);
  for (int i = 0; i < BIG_SIZE; v[i] = i++);

  {
    Timer t("Forward iterator binsearch");
    std::cout << BinSearch<int, ForwardIterator<int>>(v, ITEM_TO_FIND) << std::endl;
  }
  {
    Timer t("Bidirectional iterator binsearch");
    std::cout << BinSearch<int, BidirectionalIterator<int>>(v, ITEM_TO_FIND) << std::endl;
  }
  {
    Timer t("Random access iterator binsearch");
    std::cout << BinSearch<int, std::vector<int>::iterator>(v, ITEM_TO_FIND) << std::endl;
  }
}