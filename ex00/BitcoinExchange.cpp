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

void	err(std::string msg, std::ifstream* file)
{
	if (file)
		file->close();
	std::cerr << msg << std::endl;
	exit(EXIT_FAILURE);
}

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
bool	BitcoinExchange::validateDate()
{
	std::ifstream file(_filename);
	if (!file)
		err("Error opening file", NULL);
	std::string content;
	while (getline(file, content))
	{
		std::cout << content << std::endl;
		std::smatch matches;
		std::regex pattern("^(\\d{4})-(\\d{2})-(\\d{2})$");
		if (!std::regex_match(content, matches, pattern))
			err("Invalid Date format", &file);	
		int year = std::stoi(matches[1].str());
		int month = std::stoi(matches[2].str());
		int day = std::stoi(matches[3].str());
		const int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30 ,31};
		if (month < 1 || month > 12 || day < 1 || day > daysInMonth[month])
			err("Invalid Date formant", &file);
		if (month == 2)
		{
			if (isLeapYear(year) && day > 29)
				err("Invalid Date formant", &file);
			else
				err("Invalid Date formant", &file);
		}
	}
	file.close();
	return (true);
}

// validate input
// 	Each line in this file must use the following format: "date | value".
//		regex
//	A valid date will always be in the following format: Year-Month-Day.
//	Date must exist
//		Gap year Every 4 years, not every 100 years, but every 400 years
//		30 Days: April, Jun, Sep, Nov
//		31 Days: Jan, March, May, July, Aug, Oct, Dec
//		28 Days: Feb
//	A valid value must be either a float or a positive integer, between 0 and 1000.
//		
//	if the date does not exist in DB then use the next closest lower date