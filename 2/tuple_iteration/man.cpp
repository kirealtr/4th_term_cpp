#include <iostream>
#include <tuple>

//template <typename T, class Func>
//void TupleApplyToEach(std::tuple<T>& t, Func& func) {
//  func(std::get<0>(t));
//}
//
//template <size_t N, class Func>
//void TupleApplyToEach(std::tuple<>& t, Func& func) {
//  Head head = std::get<0>(t);
//  std::tuple<Head...> head = std::get<std::tuple_size(t) - 1>(t);
//  //std::tie(head, tail) = t;
//  func(head);
//  TupleApplyToEach(std::get<std::tuple_size(t) - 1>(t), func);
//}

template <class Tuple, class Func, size_t N>
struct TupleIterator {
  static void ApplyToEach(Tuple& t, Func& func) {
    TupleIterator<Tuple, Func, N - 1>::ApplyToEach(t, func);
    func(std::get<N - 1>(t));
  }
};

template <class Tuple, class Func>
struct TupleIterator<Tuple, Func, 1> {
  static void ApplyToEach(Tuple& t, Func& func) {
    func(std::get<0>(t));
  }
};

template<typename Tuple, class Func>
void TupleApplyToEach(Tuple& t, Func& func) {
  TupleIterator<Tuple, Func, std::tuple_size_v<Tuple>>::ApplyToEach(t, func);
}

double sum = 0;
double multiplication = 1;

template <typename T>
void Add(const T& obj) {
  sum += obj;
}

void Multiply(const int& obj) {
  multiplication *= obj;
}

void Square(double& obj) {
  obj = obj * obj;
}

void Print(double& obj) {
  std::cout << obj << " ";
}

int main() {
  std::tuple<double, double, double> t(1, 2.4, 5.8);
  TupleApplyToEach(t, Add<double>);
  std::cout << sum << std::endl;
  TupleApplyToEach(t, Multiply);
  std::cout << multiplication << std::endl;

  std::cout << std::endl;
  TupleApplyToEach(t, Square);
  TupleApplyToEach(t, Print);
  std::cout << std::endl;
}