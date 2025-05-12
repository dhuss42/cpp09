/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhuss <dhuss@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:51:43 by dhuss             #+#    #+#             */
/*   Updated: 2025/05/12 13:55:02 by dhuss            ###   ########.fr       */
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
	E_WRONGHEADER,
} t_errors;

class BitcoinExchange
{
	private:
		std::string	_filename;
		std::string _date;
		std::map<std::string, float> _dataBase;
	public:
		BitcoinExchange();
		BitcoinExchange(std::string filename);
		BitcoinExchange(const BitcoinExchange& src);
		BitcoinExchange operator=(const BitcoinExchange& other);
		~BitcoinExchange();

		std::string	todaysDate();
		bool	dateSpan(int year, int month, int day);
		bool	validateDate(std::string str);
		bool	validateValue(std::string str, bool exiting);
		void	matchDates(std::string date, std::string value);
		void	extractInput();
		void	mapDataBase();

		static void	err(t_errors err, std::string msg, bool exiting);
};

#endif