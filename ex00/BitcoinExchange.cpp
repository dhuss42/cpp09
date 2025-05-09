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
	_date = time_str;
	return(time_str);
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
		err(E_DATE, nullptr);
	int year = std::stoi(matches[1].str());
	int month = std::stoi(matches[2].str());
	int day = std::stoi(matches[3].str());
	const int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30 ,31};
	if (month < 1 || month > 12)
		err(E_DATE, nullptr);
	int maxDays = daysInMonth[month];
	if (month == 2 && isLeapYear(year))
		maxDays = 29;
	if (day < 1 || day > maxDays)
		err(E_DATE, nullptr);
	// std::cout << _date << std::endl;
	if (!str.compare(_date))
	{
		return (false);
	}
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
		err(E_VALUE, str.c_str());
		return (false);
	}
	if (num < 0)
	{
		err(E_VALUE, "not a positive number.");
		return (false);
	}
	if (num > 1000)
	{
		err(E_VALUE, "too large a number.");
		return (false);
	}
	return (true);
}

// upper boud returns the first iterator where the key is greater than the searched key
void	BitcoinExchange::matchDates(std::string date, std::string value)
{
	// std::cout << "\n-------- match Dates ----------" << std::endl;


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
		err(E_FILE, NULL);
	std::string	line;
	std::smatch	split;
	std::regex	pattern("^(\\d{4}-\\d{2}-\\d{2})( \\| )([+-]?\\d+[.]?\\d*)$");
	while (getline(file, line))
	{
		// read from file
		// split the line
		// validate the date && value
		// use the date to search for the corresponding date in container
		// use the value to multiply

		if (std::regex_match(line, split, pattern))
		{
			if (validateDate(split[1]) && validateValue(split[3]))
				matchDates(split[1], split[3]);
				// _input.insert({split[1], std::stof(split[3])});
		}


		// std::cout << line << std::endl;

		// if (std::regex_match(line, split, pattern))
		// {
		// 	if (validateDate(split[1]) && validateValue(split[3]))
		// 		_input.insert({split[1], std::stof(split[3])});
		// }
		// else
		// {
		// 	err(E_INPUT, line);
		// }
	}
	file.close();
}

// need to change logic slightly
// read everything into container
// when going over container do the validation of the data

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
		err(E_FILE, "data.csv");
	std::string	line;
	getline(file, line);
	if (line.compare("date,exchange_rate") != 0)
	{
		std::cerr << "Error: wrong header for data.csv. Exiting..." << std::endl;
		exit(EXIT_FAILURE);
	}
	_date = todaysDate();
	std::regex pattern("^(\\d{4}-\\d{2}-\\d{2}),(\\d+\\.?\\d*)$");
	std::smatch split;
	while (getline(file, line))
	{
		if (std::regex_match(line, split, pattern))
		{
			if (!validateDate(split[1]))
			{
				std::cerr << "Error: invalid Date for data.csv. Exiting..." << std::endl;
				exit(EXIT_FAILURE);
			}
			// validate Value
			_dataBase.insert({split[1], std::stof(split[2])});
		}
	}
}

/*--------------------------*/
/* handles errors			*/
/*--------------------------*/
void	BitcoinExchange::err(t_errors err, std::string msg)
{
	std::cerr << "\033[31mError: ";
	if (err == E_ARGS)
		std::cerr << "Invalid number of arguments.";
	else if (err == E_FILE)
		std::cerr << "could not open file.";
	else if (err == E_INPUT)
		std::cerr << "bad input => " << msg;
	else if (err == E_DATE)
		std::cerr << "Invalid date format!";
	else if (err == E_VALUE)
		std::cerr << msg;
	else if (err == E_EMPTY)
		std::cerr << "Input String is empty!";
	std::cerr << "\033[0m" << std::endl;
}
