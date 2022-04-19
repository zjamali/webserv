
#include "../headers/_UltimateHeader_.hpp"

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		try
		{
			configParser config(argv[1]);
			webServer	ws(config);
		}
		catch(const std::exception& e)
		{
			std::cerr << RED << e.what() << '\n';
		}
	}
	else
		std::cout << "Error, you should pass a config file" << std::endl;
	return (0);
}