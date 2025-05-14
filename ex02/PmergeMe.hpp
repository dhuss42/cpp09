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
		std::vector<size_t> _mainChainIndices;
		size_t	_jacobsthal;
		size_t	_insertions;
	public:
		PmergeMe();
		~PmergeMe();

	class NotPositiveException: public std::exception
	{
		public:
				const char* what() const noexcept;
	};
	class NotOnlyDigits: public std::exception
	{
		public:
				const char* what() const noexcept;
	};
	class StringEmpty: public std::exception
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

	template <typename T>
	void	printContainer(T& container, unsigned long elementSize)
	{
		size_t index = 0;
		for (auto it = container.begin(); it != container.end(); it++, index++)
		{
			if (index % elementSize == 0)
				std::cout << "\033[32m | \033[0m";
			std::cout << *it << " ";
		}
		std::cout << std::endl;;
	}

	//	0 1 1 3 5 11
	void	JacobsthalNumber(int& previous, int& current)
	{
		_jacobsthal = 2 * previous + current;
		previous = current;
		current = _jacobsthal;
	}

	// insertleion logic with upper_bound

	// binary search
	//	find the end of mainChain
	//		if there is a bound b3 a3 then the end of searching span is one before a3
			// the bound element is the current Jacobsthal - number of insertions
	//		if not compare if with the enire main
	template <typename T>
	void	binarySearch(T& mainChain, T& pend, unsigned long elementSize)
	{
		auto end = mainChain.begin() + (_jacobsthal - _insertions);
		std::cout << "end main chain: " << *end << std::endl;
		(void) pend;
		(void) elementSize;
	}


	// binary insertion sort
	//	select correct element from pend based on Jacobsthal starting from 3
	//	-> call function to retrive current Jacobsthal number
	//	if there is not matching bx for the Jacobsthal then we start from the back
	template <typename T>
	void	binaryInsertionSort(std::pair<T, T> &chains, unsigned long elementSize)
	{
		std::cout << "\033[32m\n============== Step 3 ==============\033[0m" << std::endl;
		T mainChain = chains.first;
		T pend = chains.second;
		int current = 1;
		int	previous = 1;
		if (pend.size() < 1)
			return ;
		JacobsthalNumber(previous, current);
		std::cout << "jacobsthalnumber: " << _jacobsthal << std::endl;
		std::cout << "pend.size: " << pend.size() << std::endl;
		int run = 0;

		// I want to find the element Index from which I need to start
		for (size_t i = 0 ; i + elementSize <= pend.size(); i += elementSize)
		{
			std::cout << "run [" << run++ << "]" << std::endl;
			size_t elementIndex = i / elementSize; // using this as condition to check with jacobsthal is wrong, the start in the pend needs to be calculated differently
			// the start of the pend is the current _jacobsthal - the previous one or something similiar
			auto pendToInsert = pend.begin() + elementIndex * elementSize;
			// auto start = pend.begin() + i;
			// auto end = start + elementSize;

			std::cout << "elementIndex [" << elementIndex << "]" << std::endl;
			std::cout << "pendToInsert [" << *pendToInsert << "]" << std::endl;
			if (_jacobsthal == elementIndex)
			{
				// std:: cout << "Pend start: " << *start << std::endl;
				// std:: cout << "Pend end: " << *end << std::endl;
				binarySearch(mainChain, pend, elementSize);
				// insert the element in main Chain
				// then start loop going backwards and inserting the elements until there are non below the current jacobsthal
				// then update the jacbosthal
			}
			// else if (_jacobsthal > elementIndex)
			// {
			// 	std::cout << "start inserting from the back" << std::endl;
			// }
			// else if the Jobsthal is larger
			//	// insert going from the back
			// else if it is larger update to next Jacobsthal
		}
		// std::cout << "\n\n\n";
		// printContainer(mainChain, elementSize);
		// printContainer(pend, elementSize);
	}

	/*----------------------------------------------*/
	/* splits container into main Chain and pend	*/
	/* -	goes over container element by element	*/
	/* -	get the element index for bx and ax		*/
	/* -	sorts b1 and a's into main chain		*/
	/* -	sorts remaining b's into pend			*/
	/* -	returns them as pair					*/
	/*----------------------------------------------*/
	template <typename T>
	std::pair<T, T>	createChains(T& container, unsigned long elementSize)
	{
		std::cout << "\033[33m\n============== Step 2 ==============\033[0m" << std::endl;
		T	mainChain;
		T	pend;

		unsigned long	nbrOfElements = container.size() / elementSize;
		std::cout << "\033[36mnumber of Elements: " << nbrOfElements << " // elementSize [" << elementSize << "]\033[0m" << std::endl;
		for (size_t i = 0 ; i + elementSize <= container.size(); i += elementSize)
		{
			size_t elementIndex = i / elementSize;
			auto start = container.begin() + i;
			auto end = start + elementSize;
			if (elementIndex % 2 != 0 || elementIndex == 0)
				mainChain.insert(mainChain.end(), start, end);
			else
				pend.insert(pend.end(), start, end);
		}
		std::cout << "container:\t";
		printContainer(container, elementSize);
		std::cout << "mainChain:\t";
		printContainer(mainChain, elementSize);
		std::cout << "the pend:\t";
		printContainer(pend, elementSize);
		return (std::make_pair(mainChain, pend));
	}


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
		std::cout << "\033[35\nm============== Step 1 ==============\033[0m" << std::endl;
		unsigned long	nbrOfElements = container.size() / elementSize;
		std::cout << "\033[35mnumber of Elements: " << nbrOfElements << " // elementSize [" << elementSize << "]\033[0m" << std::endl;

		std::cout << "unsorted container:\t";
		printContainer(container, elementSize);

		for (int i = 0; i + 2 * elementSize <= container.size(); i += 2 * elementSize)
		{
			// std::cout << "\033[36mrun [" << i << "]\033[0m" << std::endl;
			auto& lastA = container[i + elementSize - 1];
        	auto& lastB = container[i + 2 * elementSize - 1];
			// std::cout << "lastA: [" << lastA << "]" << std::endl;
			// std::cout << "lastB: [" << lastB << "]" << std::endl;
			if (lastB < lastA)
			{
				// std::cout << "swaped: " << "[" << lastA << "] with [" << lastB << "]" << std::endl;
				std::swap_ranges(container.begin() + i, container.begin() + i + elementSize, container.begin() + i + elementSize);
			}
		}
		std::cout << "sorted container:\t";
		printContainer(container, elementSize);
		if (elementSize * 2 <= container.size())
			FordJohnson(container, elementSize * 2);
		std::pair<T, T> chains = createChains(container, elementSize);
		binaryInsertionSort(chains, elementSize);
	};

	void	parsing(int argc, char **argv);
	void	execute(int argc, char **argv);
	static void	err(/* t_errors err,  */std::string msg);
};


#endif