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

	// elementSize = the size of the element
	// nbrOfElements = how many elements of size elementSize are in container of size()
	// goes over container based on elementSize and locates the values of the neighbouring ending nbrs inside an element
	// compares these values if B > A they are swapped
	// std::swap_ranges(first1, last1, first2)
	//	swaps range between first1 and last1 with same range starting from first2
	// if elementSize has not exceeded
	// calls FordJohnson recursively with elementSize * 2 -> increase the size of the elements
	template <typename T>
	void	FordJohnson(T& container, unsigned long elementSize)
	{
		unsigned long	nbrOfElements = container.size() / elementSize;
		std::cout << "\033[35melementSize[" << elementSize << "]: " << "number of Elements: \033[0m" << nbrOfElements << std::endl;
		for (int i = 0; i + 2 * elementSize <= container.size(); i += 2 * elementSize)
		{
			std::cout << "\033[36mrun [" << i << "]\033[0m" << std::endl;
			auto& lastA = container[i + elementSize - 1];
        	auto& lastB = container[i + 2 * elementSize - 1];
			std::cout << "lastA: [" << lastA << "]" << std::endl;
			std::cout << "lastB: [" << lastB << "]" << std::endl;
			if (lastB < lastA)
			{
				std::cout << "swaped: " << "[" << lastA << "] with [" << lastB << "]" << std::endl;
				std::swap_ranges(container.begin() + i, container.begin() + i + elementSize, container.begin() + i + elementSize);
			}
		}
		std::cout << "new container: ";
		for (auto it = container.begin(); it != container.end(); it++)
			std::cout << *it << " ";
		std::cout << "\n" << std::endl;;	
		if (elementSize * 2 <= container.size())
			FordJohnson(container, elementSize * 2);
	};

	void	parsing(int argc, char **argv);
	void	execute(int argc, char **argv);
	static void	err(/* t_errors err,  */std::string msg);
};


#endif