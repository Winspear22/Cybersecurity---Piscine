#include "includes.hpp"

int main(int argc, char **argv)
{
	std::vector<std::string> vec(argv + 1, argv + argc);
	for (const auto& it : vec)
		std::cout << it << std::endl;
	return 0;
}