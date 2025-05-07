#include "BitcoinExchange.hpp"

//--------Constructors----------//

BitcoinExchange::BitcoinExchange() : _filename("default")
{
}

BitcoinExchange::BitcoinExchange(std::string filename) : _filename(filename)
{
}

BitcoinExchange::~BitcoinExchange()
{
}

//------------------//

bool	isLeapYear(int year)
{
	return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}
// use map

//open file

//year-month-date |
// matches stores what is grouped together determined by ()
// still possible to input invalid years like 1000, need to find out what the lowest valid date is
//	maybe from the csv file
bool	BitcoinExchange::validateDate(std::string str)
{
	std::smatch matches;
	std::regex pattern("^(\\d{4})-(\\d{2})-(\\d{2}) $");
	if (!std::regex_match(str, matches, pattern))
		err(E_DATE, nullptr);
	int year = std::stoi(matches[1].str());
	int month = std::stoi(matches[2].str());
	int day = std::stoi(matches[3].str());
	const int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30 ,31};
	if (month < 1 || month > 12 || day < 1 || day > daysInMonth[month])
		err(E_DATE, nullptr);
	if (month == 2)
	{
		if (isLeapYear(year) && day > 29)
			err(E_DATE, nullptr);
		else
			err(E_DATE, nullptr);
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
		std::cerr << e.what() << '\n';
	}
	std::cout << num << std::endl;
	if (num < 0 || num > 1000)
		err(E_VALUE, nullptr);
	return (true);
}

//	A valid value must be either a float or a positive integer, between 0 and 1000.
//
//	if the date does not exist in DB then use the next closest lower date


bool	BitcoinExchange::filterOne(std::ifstream *file)
{
	std::string	content;
	std::regex	pattern("^(\\d{4})-(\\d{2})-(\\d{2}) \\| (\\d+[.]?\\d+)$");
	while (getline(*file, content))
	{
		std::cout << content << std::endl;
		if (!std::regex_match(content, pattern))
			err(E_INPUT, nullptr);
		std::stringstream	ss(content);
		std::string token;
		while (getline(ss, token, '|'))
		{
			if (isdigit(token[0]))
			{
				std::cout << "\033[34m\tvalue\033[0m" << token << std::endl;
				validateDate(token);
			}
			else if (isspace(token[0]))
			{
				std::cout << "\033[34m\tvalue\033[0m" << token << std::endl;
				validateValue(token);
			}
		}
	}
	return (true);
}

// check with regex date | value
// read from the file line for line
// split the lines into two with delimiter |
void	BitcoinExchange::parsing()
{
	std::string token;
	std::ifstream file(_filename);
	if (!file)
		err(E_FILE, NULL);
	if (!filterOne(&file))
		err(E_DATE, &file);
	file.close();
}

/*--------------------------*/
/* handles errors			*/
/*--------------------------*/
void	BitcoinExchange::err(t_errors err, std::ifstream* file)
{
	std::cerr << "\033[31mError: ";
	if (err == E_ARGS)
		std::cerr << "Invalid number of arguments!";
	else if (err == E_FILE)
		std::cerr << "Cannot open file!";
	else if (err == E_INPUT)
		std::cerr << "Invalid input format!";
	else if (err == E_DATE)
		std::cerr << "Invalid date format!";
	else if (err == E_VALUE)
		std::cerr << "Invalid value!";
	else if (err == E_EMPTY)
		std::cerr << "Input String is empty!";
	std::cerr << "\033[0m" << std::endl;
	if (file)
		file->close();
	exit(EXIT_FAILURE);
}
