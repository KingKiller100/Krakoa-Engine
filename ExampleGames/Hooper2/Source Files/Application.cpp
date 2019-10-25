
#include <iostream>

namespace krakoa::test
{
	extern void PrintTest();
}

int main(int argv, char** argc)
{
	krakoa::test::PrintTest();
	 
	std::cin.get();

	return 0;
}