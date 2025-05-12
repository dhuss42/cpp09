/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhuss <dhuss@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:51:38 by dhuss             #+#    #+#             */
/*   Updated: 2025/05/09 15:51:40 by dhuss            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

//--------Constructors----------//

BitcoinExchange::BitcoinExchange() : _filename("default"), _date("default")
{
}

BitcoinExchange::BitcoinExchange(std::string filename) : _filename(filename)
{
}

BitcoinExchange::~BitcoinExchange()
{
}

//------------------//

std::string	BitcoinExchange::todaysDate()
{
	std::time_t	now;
	char	time_str[18];
	now = std::time(nullptr);
	std::strftime(time_str, sizeof(time_str), "%Y-%m-%d", std::localtime(&now));
	std::cout << "today's date" << time_str << std::endl;
	return (time_str);
}

bool	isLeapYear(int year)
{
	if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
		return (true);
	return (false);
}

//year-month-date |
// matches stores what is grouped together determined by ()
// still possible to input invalid years like 1000, need to find out what the lowest valid date is
//	maybe from the csv file
// something not correct with the gap year
bool	BitcoinExchange::validateDate(std::string str)
{
	std::smatch matches;
	std::regex pattern("^(\\d{4})-(\\d{2})-(\\d{2})$");
	if (!std::regex_match(str, matches, pattern))
		err(E_DATE, "", false);
	int year = std::stoi(matches[1].str());
	int month = std::stoi(matches[2].str());
	int day = std::stoi(matches[3].str());
	const int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30 ,31};
	if (month < 1 || month > 12)
		err(E_DATE, "", false); // maybe return false and then call error from the calling function
	int maxDays = daysInMonth[month];
	if (month == 2 && isLeapYear(year))
		maxDays = 29;
	if (day < 1 || day > maxDays)
		err(E_DATE, "", false);
	// if (str.compare(_date) > 0) // does not work
	// 	return (false);
	return (true);
}

bool	BitcoinExchange::validateValue(std::string str)
{
	float	num = -1;
	try
	{
		num = std::stof(str);
	}
	catch(const std::exception& e)
	{
		err(E_VALUE, str.c_str(), false);
		return (false);
	}
	if (num < 0)
	{
		err(E_VALUE, "not a positive number.", false);
		return (false);
	}
	if (num > 1000)
	{
		err(E_VALUE, "too large a number.", false);
		return (false);
	}
	return (true);
}

// upper boud returns the first iterator where the key is greater than the searched key
void	BitcoinExchange::matchDates(std::string date, std::string value)
{
	std::multimap<std::string, float>::iterator itD = _dataBase.upper_bound(date);
	if (itD != _dataBase.begin())
		itD--;
	std::cout << date << " => " << std::stof(value) << " = " << std::stof(value) * itD->second << std::endl;
}

/*----------------------------------------------------------------------*/
/* parses input file													*/
/* - opens input file													*/
/* - extracts one line at a time										*/
/* - matches line to regex pattern and splits it into dates and values	*/
/* - split dates and values are valdiated								*/
/*----------------------------------------------------------------------*/
void	BitcoinExchange::parsing()
{
	std::ifstream file(_filename);
	if (!file)
		err(E_FILE, "", false);
	std::string	line;
	std::smatch	split;
	std::regex	pattern("^(\\d{4}-\\d{2}-\\d{2})( \\| )([+-]?\\d+[.]?\\d*)$");
	while (getline(file, line))
	{
		if (std::regex_match(line, split, pattern))
		{
			if (validateDate(split[1]) && validateValue(split[3]))
				matchDates(split[1], split[3]);
		}
	}
	file.close();
}

/*----------------------------------------------------------*/
/* - opens data.csv											*/
/* - extracts line from file								*/
/* - splits data and exchange rate with regex				*/
/* - populate multimap container with date and exchange rate		*/
/*----------------------------------------------------------*/
void	BitcoinExchange::mapDataBase()
{
	// add validation to the .csv file
		// check name or extension
		// check if the header in first line is there
		// check valid format for dates
			// check date span ? to today
		// check if exchange rate is valid
	std::ifstream	file("data.csv");
	if (!file)
		err(E_FILE, "data.csv.", true);
	std::string	line;
	getline(file, line);
	if (line.compare("date,exchange_rate") != 0)
	{
		file.close();
		err(E_WRONGHEADER, "data.csv.", true);
	}
	// _date = todaysDate();
	std::regex pattern("^(\\d{4}-\\d{2}-\\d{2}),(\\d+\\.?\\d*)$");
	std::smatch split;
	while (getline(file, line))
	{
		if (std::regex_match(line, split, pattern))
		{
			if (!validateDate(split[1]))
			{
				err(E_INPUT, line, true);
				file.close();
			}
			// validate Value
			_dataBase.insert({split[1], std::stof(split[2])});
		}
		else
		{
			err(E_INPUT, line, true);
		}
	}
	file.close();
}

/*--------------------------*/
/* handles errors			*/
/*--------------------------*/
void	BitcoinExchange::err(t_errors err, std::string msg, bool dataBase)
{
	std::cerr << "\033[31mError: ";
	if (err == E_ARGS)
		std::cerr << "Invalid number of arguments.";
	else if (err == E_FILE)
		std::cerr << "could not open " << msg;
	else if (err == E_INPUT)
		std::cerr << "bad input => " << msg;
	else if (err == E_DATE)
		std::cerr << "Invalid date format!";
	else if (err == E_VALUE)
		std::cerr << msg;
	else if (err == E_EMPTY)
		std::cerr << "Input String is empty!";
	else if (err == E_WRONGHEADER)
		std::cerr << "Wrong header!";
	if (dataBase)
	{
		std::cerr << " Exiting...\033[0m" << std::endl;
		exit(EXIT_FAILURE);	
	}
	std::cerr << "\033[0m" << std::endl;
}
