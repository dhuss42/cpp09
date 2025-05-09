#include "BitcoinExchange.hpp"

void	exchanger(const std::string& filename)
{
	BitcoinExchange btc(filename);
	// btc.parsing();
	btc.mapDataBase();
}

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Error could not opn file." << std::endl;
		return (-1);
	}
	else
	{
		exchanger(argv[1]);
	}
	return (0);
}