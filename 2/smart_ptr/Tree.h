#pragma once
#include <memory>
#include <iostream>

template <typename T>
class Tree {

  struct Node {
    std::weak_ptr<Node> parent;
    std::shared_ptr<Node> child_left;
    std::shared_ptr<Node> child_right;
    T value;

    Node(const T& val) : value(val) {}

    ~Node() {
      std::cout << "Node with value " << value << " is deleted" << std::endl;
      value.~T();
    }
  };

  std::shared_ptr<Node> head_;
  std::shared_ptr<Node> iter_;
public:
  Tree(const T& val) {
    head_ = std::make_shared<Node>(val);
    iter_ = head_;
  }

  T& GetValue() { return iter_->value; }

  void GoLeft() { 
    /*if (iter_->child_left == nullptr)
      throw std::runtime_error("Node doesn't exist");*/

    iter_ = iter_->child_left;
  }

  void GoRight() {
    /*if (iter_->child_right == nullptr)
      throw std::runtime_error("Node doesn't exist");*/

    iter_ = iter_->child_right;
  }

  void GoUp() {
    /*if (iter_->parent.expired())
      throw std::runtime_error("Node doesn't exist");*/

    iter_ = (iter_->parent).lock();
  }

  void AppendLeft(const T& val) {
    //delete iter_->child_left;
    iter_->child_left = std::make_shared<Node>(val); // if left child already exist, it will be deleted
    iter_->child_left->parent = std::weak_ptr<Node>(iter_);
  }

  void AppendRight(const T& val) {
    //delete iter_->child_left;
    iter_->child_right = std::make_shared<Node>(val);
    iter_->child_right->parent = std::weak_ptr<Node>(iter_);
  }

  void ReturnToHead() {
    iter_ = head_;
  }
};