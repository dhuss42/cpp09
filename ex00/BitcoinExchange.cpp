/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhuss <dhuss@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:51:38 by dhuss             #+#    #+#             */
/*   Updated: 2025/05/12 14:14:19 by dhuss            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

//--------Constructors----------//

/*------------------------------*/
/* Default Constructor			*/
/*------------------------------*/
BitcoinExchange::BitcoinExchange() : _filename(""), _date("")
{
	std::cout << std::fixed << std::setprecision(2);
}

/*------------------------------*/
/* Constructor					*/
/*------------------------------*/
BitcoinExchange::BitcoinExchange(std::string filename) : _filename(filename), _date(todaysDate())
{
	std::cout << std::fixed << std::setprecision(2);
}

/*------------------------------*/
/* Copy Constructor				*/
/*------------------------------*/
BitcoinExchange::BitcoinExchange(const BitcoinExchange& src) : _filename(src._filename), _date(src._date), _dataBase(src._dataBase)
{
}

/*----------------------------------*/
/* Overloaded Assignment Operator	*/
/*----------------------------------*/
BitcoinExchange BitcoinExchange::operator=(const BitcoinExchange& other)
{
	if (this != &other)
	{
		_filename = other._filename;
		_date = other._date;
		_dataBase = other._dataBase;
	}
	return (*this);
}

/*------------------------------*/
/* Destructor					*/
/*------------------------------*/
BitcoinExchange::~BitcoinExchange()
{
}

//--------helper----------//

/*------------------------------------------*/
/* checks whether the year is a leap year	*/
/*------------------------------------------*/
bool	isLeapYear(int year)
{
	if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
		return (true);
	return (false);
}

//--------public methods----------//

/*--------------------------------------*/
/* Returns today's date as a string		*/
/*--------------------------------------*/
std::string	BitcoinExchange::todaysDate()
{
	std::time_t	now;
	char	time_str[18];
	now = std::time(nullptr);
	std::strftime(time_str, sizeof(time_str), "%Y-%m-%d", std::localtime(&now));
	return (time_str);
}

/*----------------------------------------------*/
/*	Checks if the time span is valid			*/
/*	-	from epoch until current day			*/
/*----------------------------------------------*/
bool	BitcoinExchange::dateSpan(int year, int month, int day)
{
	std::smatch matches;
	std::regex pattern("^(\\d{4})-(\\d{2})-(\\d{2})$");
	if (!std::regex_match(_date, matches, pattern))
		return (false);
	if (year >= 1970 && year < std::stoi(matches[1].str()))
		return (true);
	else if (year == std::stoi(matches[1].str()))
	{
		if (month <= std::stoi(matches[2].str()))
		{
			if (day <= std::stoi(matches[3].str()))
				return (true);
		}
	}
	return (false);
}

/*------------------------------------------------------*/
/*	validates the Date format && whether date exists	*/
/*------------------------------------------------------*/
bool	BitcoinExchange::validateDate(std::string str)
{
	std::smatch matches;
	std::regex pattern("^(\\d{4})-(\\d{2})-(\\d{2})$");
	if (!std::regex_match(str, matches, pattern))
		return (false);
	int year = std::stoi(matches[1].str());
	int month = std::stoi(matches[2].str());
	int day = std::stoi(matches[3].str());
	const int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30 ,31};
	if (month < 1 || month > 12)
		return (false);
	int maxDays = daysInMonth[month];
	if (month == 2 && isLeapYear(year))
		maxDays = 29;
	if (day < 1 || day > maxDays)
		return (false);
	if (!dateSpan(year, month, day))
		return (false);
	return (true);
}

/*------------------------------------------------------*/
/*	validates the Value									*/
/*	- catches overflows									*/
/*	- no negative values								*/
/*	- if input value not above 1000						*/
/*	- if data.csv value not above 1e20					*/
/*		-> prevents the final result from being inf		*/
/*------------------------------------------------------*/
bool	BitcoinExchange::validateValue(std::string str, bool exiting)
{
	float	num = -1;
	try
	{
		num = std::stof(str);
	}
	catch(const std::exception& e)
	{
		err(E_INPUT, str.c_str(), exiting);
		return (false);
	}
	if (num < 0)
	{
		err(E_VALUE, "not a positive number.", exiting);
		return (false);
	}
	if (!exiting && num > 1000)
	{
		err(E_VALUE, "too large a number.", exiting);
		return (false);
	}
	if (exiting && num > 1e20)
		err(E_VALUE, "Exchange rate too high.", exiting);
	return (true);
}

/*----------------------------------------------------------------------------------*/
/* finds matching dates and multiples exchange rate with value						*/
/*	-	returns the first iterator where the key is greater than the searched key	*/
/*	-	subtracts one from iterator to get matching date or closest lower date		*/
/*	-	prints the multiplied value at date in question								*/
/*----------------------------------------------------------------------------------*/
void	BitcoinExchange::matchDates(std::string date, std::string value)
{
	std::map<std::string, float>::iterator it = _dataBase.upper_bound(date);
	if (it != _dataBase.begin())
		it--;
	std::cout << date << " => " << std::stof(value) << " = " << std::stof(value) * it->second << std::endl;
}

/*----------------------------------------------------------------------*/
/* parses input file													*/
/* - opens input file													*/
/* - extracts one line at a time										*/
/* - matches line to regex pattern and splits it into dates and values	*/
/* - split dates and values are valdiated								*/
/*----------------------------------------------------------------------*/
void	BitcoinExchange::extractInput()
{
	std::string	line;
	std::ifstream file(_filename);
	std::smatch	split;
	std::regex	pattern("^(\\d{4}-\\d{2}-\\d{2})( \\| )([+-]?\\d+[.]?\\d*)$");

	if (!file)
		err(E_FILE, _filename, true);
	getline(file, line);
	if (line.compare("date | value") != 0)
		err(E_WRONGHEADER, "for input file", true);
	while (getline(file, line))
	{
		if (std::regex_match(line, split, pattern))
		{
			if (validateDate(split[1]) && validateValue(split[3], false))
				matchDates(split[1], split[3]);
			else
				err(E_INPUT, line, false);
		}
		else if (line.empty())
			err(E_EMPTY, line, false);
		else
			err(E_INPUT, line, false);
	}
	file.close();
}

/*--------------------------------------------------------------*/
/* - opens data.csv												*/
/* - extracts line from file									*/
/* - splits data and exchange rate with regex					*/
/* - populate multimap container with date and exchange rate	*/
/*--------------------------------------------------------------*/
void	BitcoinExchange::mapDataBase()
{
	std::string	line;
	std::ifstream	file("data.csv");
	std::regex pattern("^(\\d{4}-\\d{2}-\\d{2}),([+-]?\\d+\\.?\\d*)$");
	std::smatch split;

	if (!file)
		err(E_FILE, "data.csv!", true);
	getline(file, line);
	if (line.compare("date,exchange_rate") != 0)
		file.close(), err(E_WRONGHEADER, "for data.csv.", true);
	while (getline(file, line))
	{
		if (std::regex_match(line, split, pattern))
		{
			if (validateDate(split[1]) && validateValue(split[2], true))
				_dataBase[split[1]] = std::stof(split[2]);
		}
		else if (line.empty())
			file.close(), err(E_EMPTY, line, true);
		else
			file.close(), err(E_INPUT, line, true);
	}
	file.close();
}

/*--------------------------*/
/* handles errors			*/
/*--------------------------*/
void	BitcoinExchange::err(t_errors err, std::string msg, bool exiting)
{
	std::cerr << "\033[31mError: ";
	if (err == E_ARGS)
		std::cerr << "invalid number of arguments.";
	else if (err == E_FILE)
		std::cerr << "could not open " << msg;
	else if (err == E_INPUT)
		std::cerr << "bad input => " << msg;
	else if (err == E_VALUE)
		std::cerr << msg;
	else if (err == E_EMPTY)
		std::cerr << "line is empty!";
	else if (err == E_WRONGHEADER)
		std::cerr << "wrong header " << msg;
	if (exiting)
	{
		std::cerr << " Exiting...\033[0m" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cerr << "\033[0m" << std::endl;
}
