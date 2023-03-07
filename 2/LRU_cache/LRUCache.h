#pragma once
#include <list>
#include <unordered_map>
#include <exception>

//using namespace std;

template <typename K, typename T>
class LRUCache {
  using List = std::list<std::pair<K, T>>;
  List list_;
  //unordered_map<K, int> map_;
  const int cap_;
public:
  LRUCache(const int& capacity) : cap_(capacity) {}

  const T Get(const K& key) {
    typename List::iterator it = list_.begin();
    for (int i = 0; i < list_.size(); i++, ++it) { // O(n) - find
      if (it->first == key) {
        std::pair<K, T> elem = *it;
        list_.erase(it); // O(1)
        //list_.push_front(std::make_pair(key, value));
        list_.push_front(std::move(elem)); // O(1)
        return elem.second;
      }
    }
    throw std::runtime_error("Element not found");
  } // totally - O(n)

  void Put(const K& key, const T& value) {
    if (list_.size() == cap_)
      list_.pop_back(); // O(1)

    list_.push_front(std::make_pair<K, T>(key, value)); // O(1)
  } // totally - O(1)

  void Put(K&& key, T&& value) {
    if (list_.size() == cap_)
      list_.pop_back();

    list_.push_front(std::make_pair<K, T>(std::move(key), std::move(value)));
  }
};
