#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

#include "iostream"
#include "fstream"
#include <map>
#include <regex>
#include <sstream>

typedef enum e_errors
{
	E_ARGS,
	E_FILE,
	E_INPUT,
	E_DATE,
	E_VALUE,
	E_EMPTY,
} t_errors;

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

		bool	validateDate(std::string str);
		bool	validateValue(std::string);
		void	parsing();
		bool	filterOne(std::ifstream *file);
		static void	err(t_errors err, std::ifstream* file);
};

#endif