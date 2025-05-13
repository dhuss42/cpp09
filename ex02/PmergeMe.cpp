#include "PmergeMe.hpp"

PmergeMe::PmergeMe()
{
}

PmergeMe::~PmergeMe()
{
}

// execute function that call everything
// parsing
//	- valid ints ranging from 0 to INT_MAX
//	- push_back
// function to execute sorting algorithm
//	- Start timer
//	- call function with vector
//	- print info
//	- do same with deque

// Ford-Johnson (template)
//	call with &container and elements = 1
//	get_size of sequence
//	recursion must continue until a single pair size is bigger than half of the container size
//		recursively call Ford-Johnso with elements * 2
//	

void	PmergeMe::execute(char **argv)
{

}

/*--------------------------*/
/* handles errors			*/
/*--------------------------*/
void	PmergeMe::err(t_errors err)
{
	std::cerr << "\033[31mError: ";
	if (err == E_ARGS)
		std::cerr << "invalid number of arguments.";
	std::cerr << "\033[0m" << std::endl;
}