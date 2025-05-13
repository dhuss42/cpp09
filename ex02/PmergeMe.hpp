#ifndef PMERGEME_HPP
# define PMERGEME_HPP

#include <iostream>
#include <deque>
#include <vector>
#include <algorithm>
#include <exception>
#include <chrono>

class PmergeMe
{
	private:
		std::vector<int> _vector;
		std::deque<int>	_deque;
	public:
		PmergeMe();
		~PmergeMe();

	class NotPositiveException: public std::exception
	{
		public:
				const char* what() const noexcept;
	};

	// Ford-Johnson (template)
	//	call with &container and elements = 1
	//	get_size of sequence
	//	recursion must continue until a single pair size is bigger than half of the container size
	//		recursively call Ford-Johnso with elements * 2
	//
	typedef enum e_errors
	{
		E_ARGS,
	} t_errors;

	// elements need to be paired somehow
	// maybe use iterator ranges to track positions
	// std::swap_ranges(first1, last1, first2)
	//	swaps range between first1 and last1 with same range starting from first2
	template <typename T>
	void	FordJohnson(T& container, unsigned long elementSize)
	{
		unsigned long	nbrOfElements = container.size() / elementSize;
		std::cout << "elementSize[" << elementSize << "]: " << "number of Elements: " << nbrOfElements << std::endl;
		for (int i = 0; i + 2 * elementSize <= container.size(); i += 2 * elementSize) // not correct iteration
		{
			T& lastA = container[i + elementSize -1];
			T& lastB = container[i + 2 * elementSize -1];
			std::cout << lastA << std::endl;
			std::cout << lastB << std::endl;
			// plan is to compare the two neighbouring ending iterators
		}

		// if (!(elementSize > container.size() / 2))
		// 	FordJohnson(container, elementSize * 2);
	};

	void	parsing(int argc, char **argv);
	void	execute(int argc, char **argv);
	static void	err(/* t_errors err,  */std::string msg);
};


#endif