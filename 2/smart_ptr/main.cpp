#include <iostream>
#include "Tree.h"

int main() {
  Tree<int> tree(5);
  tree.AppendLeft(6);
  tree.AppendLeft(7);
  tree.GoLeft();
  tree.AppendRight(8);
  tree.GoRight();
  tree.AppendRight(9);
  tree.ReturnToHead();
  tree.AppendLeft(0);
  std::cout << tree.GetValue() << std::endl;
}