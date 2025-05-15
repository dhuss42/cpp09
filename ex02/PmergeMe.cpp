#include "PmergeMe.hpp"

PmergeMe::PmergeMe() : _jacobsthal(3), _insertions(0), _right(true)
{
}

PmergeMe::~PmergeMe()
{
}


const	char* PmergeMe::NotPositiveException::what() const noexcept
{
	return ("not a Positive value");
}

const	char* PmergeMe::NotOnlyDigits::what() const noexcept
{
	return ("input string does not consist of only digits");
}

const	char* PmergeMe::StringEmpty::what() const noexcept
{
	return ("string is empty");
}

bool	only_digits(std::string str)
{
	size_t start = 0;
	if (str[0] == '-' || str[0] == '+')
		start = 1;
	if (start == str.length())
		return (false);
	return (std::all_of(str.begin() + start, str.end(), ::isdigit));
}

void	PmergeMe::parsing(int argc, char **argv)
{
	int n = -1;
	try{
		for (int i = 1; i < argc; ++i)
		{
			std::string emptyCheck = static_cast<std::string>(argv[i]);
			if (emptyCheck.empty())
				throw(StringEmpty());
			if (only_digits(argv[i]))
				n = std::stoi(argv[i]);
			else
				throw(NotOnlyDigits());
			if (n >= 0)
			{
				_vector.push_back(n);
				_deque.push_back(n);
			}
			else
				throw(NotPositiveException());
		}
	}
	catch(const std::exception& e)
	{
		 err(e.what());
	}
}

/*------------------------------------------------------------------------------*/
/*	- high_resolution_clock::now												*/
/*		-> most precise clock													*/
/*	- auto instead of															*/
/*		-> std::chrono::time_point<std::chrono::high_resolution_clock> start	*/
/*	- casting ensures that the time is represented in that unit					*/
/* 		-> .count() does nor know units											*/
/* 		-> .count() provides the raw number of time units						*/
/*------------------------------------------------------------------------------*/
void	PmergeMe::execute(int argc, char **argv)
{
	parsing(argc, argv);

	std::cout << "Before:\t";
	std::copy(_vector.begin(), _vector.end(), std::ostream_iterator<int>(std::cout, " "));

	// std::vector<int> sorted(_vector);
	// std::stable_sort(sorted.begin(), sorted.end());
	// std::cout << "\nAfter:\t";
	// std::copy(sorted.begin(), sorted.end(), std::ostream_iterator<int>(std::cout, " ")); // probaly print the actual sorted result
	// std::cout << std::endl;
	auto	start = std::chrono::high_resolution_clock::now();
	FordJohnson(_vector, 1);
	auto	end = std::chrono::high_resolution_clock::now();
	auto	duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

	std::cout << "\nAfter:\t";
	printContainer(_vector, 0);
	std::cout << "Time to process a range of " << _vector.size() << " elements with std::vector : " << duration << " us" << std::endl;
	// std::cout << "Time to process a range of " << _deque.size() << " elements with std::vector : " << "time us" << std::endl;

	//	- do same with deque

}

/*--------------------------*/
/* handles errors			*/
/*--------------------------*/
void	PmergeMe::err(/* t_errors err,  */std::string msg)
{
	std::cerr << "\033[31mError: " << msg;
	// switch (err)
	// {
	// 	case (E_ARGS):
	// 		std::cerr << msg;
	// 		break;
	// 	case (E_NEGATIVE)
	// 		std::cerr << msg;
	// 	case (E_RANGE)
	// 		std::cerr << msg;
	// 	default:
	// 		break;
	// }
	std::cerr << "\033[0m" << std::endl;
	exit(EXIT_FAILURE);
}