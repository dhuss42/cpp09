#include "PmergeMe.hpp"

PmergeMe::PmergeMe()
{
}

PmergeMe::~PmergeMe()
{
}


const	char* PmergeMe::NotPositiveException::what() const noexcept
{
	return ("not a Positive value");
}

void	PmergeMe::parsing(int argc, char **argv)
{
	try{
		for (int i = 1; i < argc; ++i)
		{
			int n = std::stoi(argv[i]);
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

	std::vector<int> sorted(_vector);
	std::stable_sort(sorted.begin(), sorted.end());
	std::cout << "\nAfter:\t";
	std::copy(sorted.begin(), sorted.end(), std::ostream_iterator<int>(std::cout, " ")); // probaly print the actual sorted result
	std::cout << std::endl;
	auto	start = std::chrono::high_resolution_clock::now();
	FordJohnson(_vector, 1);
	auto	end = std::chrono::high_resolution_clock::now();
	auto	duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

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
}