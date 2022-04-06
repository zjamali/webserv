# include <iostream>
# include <sstream>

int main()
{
	size_t size = 0;
	std::stringstream stream("8");
	stream >> std::hex >> size;
	std::cout << size << std::endl;
}