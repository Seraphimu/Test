#include <iostream>
#define ADD_TO_SUM(sum, value) \
	sum##num += value

int main(void) {
	int sum = 0;

	std::cout << ADD_TO_SUM(5, 10);	

	return 0;
}
