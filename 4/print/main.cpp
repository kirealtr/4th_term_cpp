#include <iostream>

void Print() {}

template<typename Head, typename... Tail>
void Print(Head&& head, Tail&&... tail) {
	std::cout << head << std::endl;
	Print(std::forward<Tail>(tail)...);
}

int main() {
	std::string str = "eeeeeeeeee";
	int a = 5245;
	Print(a, str, std::move(str), "Hello world");
}