#include <fstream>
#include <vector>
#include <iostream>

#define BIG_SIZE  1e5

int main() {
  std::ofstream file;
  file.open("data1.csv");
  std::vector<int> v;
  v.reserve(2e4);
  for (int i = 0; i < BIG_SIZE; i++) {
    v.push_back(i);
    file << v.size() << ", " << v.capacity() << std::endl;
  }

  
  /*int i = 0;
  try {
    while (true) {
      v.push_back(i);
      file << v.size() << ", " << v.capacity() << std::endl;
      i++;
    }
  }
  catch (std::bad_alloc) {
    std::cout << "Max vector size: " << i << std::endl;
  }*/
}

/*
�� �������� �����, ��� ������� ������������� �� � 2, � �������� � ������� ���� ������
��� ��� ���������� �������. ���� ������� ������ ������� �������, �� ��� ������ 
������������ ��������� ������� ������, ����� � ����� �� ���� ������� ��, ��� ����
�� reserve �� ��������.
*/