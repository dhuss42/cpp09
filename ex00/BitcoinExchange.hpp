#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

#include "iostream"
#include "fstream"
#include <map>
#include <regex>

class BitcoinExchange
{
	private:
		std::string	_filename;
	public:
		BitcoinExchange();
		BitcoinExchange(std::string filename);
		// BitcoinExchange(const BitcoinExchange& src);
		// BitcoinExchange operator=(const BitcoinExchange& other);
		~BitcoinExchange();

		bool	validateDate();
};

#endif