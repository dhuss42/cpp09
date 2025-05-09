/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhuss <dhuss@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:51:43 by dhuss             #+#    #+#             */
/*   Updated: 2025/05/09 15:51:44 by dhuss            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

#include "iostream"
#include "fstream"
#include <map>
#include <regex>
#include <iomanip>
#include <ctime>

typedef enum e_errors
{
	E_ARGS,
	E_FILE,
	E_INPUT,
	E_DATE,
	E_VALUE,
	E_EMPTY,
} t_errors;

// need to use multimap so that duplicates can be stored inside the container
// map overwrites duplicates
class BitcoinExchange
{
	private:
		std::string	_filename;
		std::string _date;
		std::multimap<std::string, float> _dataBase;
		std::multimap<std::string, float> _input;
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
		void	mapDataBase();
		void	matchDates(std::string date, std::string value);
		std::string	todaysDate();
		static void	err(t_errors err, std::string msg);
};

#endif