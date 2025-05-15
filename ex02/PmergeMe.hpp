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
		bool	_right;
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
			if (elementSize != 0 && index % elementSize == 0)
				std::cout << "\033[32m | \033[0m";
			std::cout << *it << " ";
		}
		std::cout << std::endl;;
	}

	//	0 1 1 3 5 11
	void	updateJacobsthal(int& previous, int& current)
	{
		_jacobsthal = 2 * previous + current;
		previous = current;
		current = _jacobsthal;
	}


		// identify insertion point
		// now determine the middle point between mainChain.begin() and end
		// maybe recursively call a function that shrinks the scanning range
		//	find the middle of start and end in main Chain (has to be scaled with elementSize)
		//	compare the Value from pend (startPend) to the middle Value
		//	if smaller: range is to the left of target value
		//		mainChain end becomes middle Value - elementSize
		//		mainChain start remains the same
		//	if larger: range is to the right of the target value
		//		main Chain end remains the same
		//		main Chain Start is middle Value + elementSize
		//	if the same insert it after the middle Value
	template <typename Iterator>
	auto identifyInsertionPoint(Iterator mainStart, Iterator mainEnd, Iterator currentPend, unsigned long elementSize)
	{
		// std::cout << "\033[36m\n--------- identifyInsertionPoint ---------\033[0m" << std::endl;
		// std::cout << "mainStart: " << *mainStart << std::endl;
		// std::cout << "mainEnd: " << *mainEnd << std::endl;
		auto mainMiddle = mainStart + (mainEnd - mainStart) / 2;
		while (mainStart <= mainEnd)
		{
			auto dist = mainEnd - mainStart;

			mainMiddle = mainStart + (dist / 2 / elementSize) * elementSize;
			// std::cout << "mainMiddle: " << *mainMiddle << std::endl;
			// std::cout << "currentPend: " << *currentPend << std::endl;

			if (*currentPend < *mainMiddle)
			{
				// std::cout << "smaller: " << *currentPend << " < " << *mainMiddle << std::endl;
				_right = false;
				// for (unsigned long i = 0; i < elementSize; i++)
				// 	mainMiddle--;
				// mainEnd = mainMiddle;
				if (mainMiddle == mainStart)
					break ;
				mainEnd = mainMiddle - elementSize;
				// std::cout << "mainEnd: " << *mainEnd << std::endl;
			}
			else if (*currentPend > *mainMiddle)
			{
				_right = true;
				// std::cout << "larger: " << *currentPend << " > " << *mainMiddle <<std::endl;
				// for (unsigned long i = 0; i < elementSize; i++)
				// 	mainMiddle++;
				if (mainMiddle == mainEnd)
					break ;
				mainStart = mainMiddle + elementSize;
				// std::cout << "mainStart: " << *mainEnd << std::endl;
			}
			else
			{
				_right = true;
				// std::cout << "middle" << std::endl;
				return (mainMiddle);
			}
		}
		return (mainStart);
	}

	// binary search
	//	find the end of mainChain
	//		if there is a bound b3 a3 then the end of searching span is one before a3
	// the bound element is the current Jacobsthal - number of insertions
	//		if not compare if with the enire main
	template <typename T, typename Iterator>
	void	binarySearch(T& mainChain, T& pend, unsigned long elementSize, Iterator currentPend, size_t &insertions)
	{
		// std::cout << "\033[34m\n--------- binary Search ---------\033[0m" << std::endl;
		// std::cout << "number of insertions:\t" << insertions << std::endl;
		// std::cout << "(jacobsthal + insertions) * elementSize - 1" << "\n" << "(" << _jacobsthal << "+" << insertions << ") * " << elementSize << "- 1" << std::endl;
		// std::cout << "=> " << (_jacobsthal + insertions) * elementSize - 1 << std::endl;
		auto mainStart = mainChain.begin() + elementSize - 1;
		// the jacobsthal provides the index for b and a, after instering we need to account for this difference in mainChain, multiply by elementSize for scaling, -1 for last element of the block
		auto mainEnd = mainChain.begin() + (_jacobsthal + insertions) * elementSize - 1;
		if ((_jacobsthal + insertions) * elementSize - 1 >= mainChain.size())
			mainEnd = mainChain.end() - 1;
		// size_t maxIndex;

		// std::cout << "start main chain: " << *mainStart << std::endl;
		// std::cout << "end main chain: " << *mainEnd << std::endl;
		auto insertionPoint = identifyInsertionPoint(mainStart, mainEnd, currentPend, elementSize);
		// std::cout << "\033[34mmainMiddle chain: \033[0m" << *insertionPoint << std::endl;

		if (_right)
			mainChain.insert(insertionPoint + 1, currentPend - elementSize + 1, currentPend + 1);
		else
			mainChain.insert(insertionPoint - elementSize + 1, currentPend - elementSize + 1, currentPend + 1);
		pend.erase(currentPend - elementSize + 1, currentPend + 1);
		insertions++;
		// std::cout << "mainChain after Insetion:\t";
		// printContainer(mainChain, elementSize);
		// std::cout << "pend after Insetion:\t";
		// printContainer(pend, elementSize);
	}

	//	use Jacobsthal as loop condition
	//	if its the matching index is not in the pend
	//	subtract the starting position until it reaches 0
	//	then increase to the next one

	// binary insertion sort
	//	select correct element from pend based on Jacobsthal starting from 3
	//	-> call function to retrive current Jacobsthal number
	//	if there is not matching bx for the Jacobsthal then we start from the back
	template <typename T>
	void	binaryInsertionSort(std::pair<T, T> &chains, unsigned long elementSize, T& container)
	{
		// std::cout << "\033[32m\n============== Step 3 ==============\033[0m" << std::endl;
		T& mainChain = chains.first;
		T& pend = chains.second;

		int current = 1;
		int	previous = 1;
		if (pend.size() < 1)
			return ;
		updateJacobsthal(previous, current);

		// std::cout << "start index is:\n\t(" << _jacobsthal << " - " << previous << ") * " << elementSize << " - 1" << std::endl;  
		// std::cout << (_jacobsthal - previous) * elementSize - 1 << std::endl;
		auto start = pend.begin() + (_jacobsthal - previous) * elementSize - 1;
		if ((_jacobsthal - previous) * elementSize - 1 >= pend.size())
			start = pend.end() - 1;
		size_t insertions = 0;
		for (auto end = pend.begin() + elementSize - 1; start >= end;)
		{
			// std::cout << "startPend: " << *start << std::endl;
			// std::cout << "endPend: " << *end << std::endl;
			binarySearch(mainChain, pend, elementSize, start, insertions);
			if (start == end)
			{
				updateJacobsthal(previous, current);
				// std::cout << "updating jacobsthal to: " << _jacobsthal << std::endl;
				// std::cout << "updating previous jacobsthal to: " << previous << std::endl;
				// start = pend.begin() + (_jacobsthal - previous) * elementSize - 1;
				auto newStart = (_jacobsthal - previous) * elementSize - 1;
				if (newStart >= pend.size())
					newStart = pend.size() -1;
				start = pend.begin() + newStart;
				// std::cout << "startPend is at: " << *start << std::endl;
				// std::cout << "endPend is at: " << *end << std::endl;
			}
			else{
				for (unsigned long i = 0; i < elementSize; i++) // decrements in elementSize
					start--;
			}
		}
		container = mainChain;
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
		// std::cout << "\033[33m\n============== Step 2 ==============\033[0m" << std::endl;
		T	mainChain;
		T	pend;

		// std::cout << "container:\t";
		// printContainer(container, elementSize);
		// unsigned long	nbrOfElements = container.size() / elementSize;
		// std::cout << "\033[36mnumber of Elements: " << nbrOfElements << " // elementSize [" << elementSize << "]\033[0m" << std::endl;
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
		// std::cout << "container:\t";
		// printContainer(container, elementSize);
		// std::cout << "mainChain:\t";
		// printContainer(mainChain, elementSize);
		// std::cout << "the pend:\t";
		// printContainer(pend, elementSize);
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
	T	FordJohnson(T& container, unsigned long elementSize, int counter)
	{
		// std::cout << "\033[35\nm============== Step 1 ==============\033[0m" << std::endl;
		// unsigned long	nbrOfElements = container.size() / elementSize;
		// std::cout << "\033[35mnumber of Elements: " << nbrOfElements << " // elementSize [" << elementSize << "]\033[0m" << std::endl;

		// std::cout << "unsorted container:\t";
		// printContainer(container, elementSize);

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
		_insertions = 0;
		// std::cout << "sorted container:\t";
		printContainer(container, elementSize);
		std::pair<T, T> chains;
		if (elementSize * 2 <= container.size())
		{
			FordJohnson(container, elementSize * 2, counter + 1); // the problem is the first argument of calling FordJohnson
			// std::cout << "\033\n[31mrecursion level [" << counter << "]\033[0m" << std::endl;

			/* std::pair<T, T>  */chains = createChains(container, elementSize);
			binaryInsertionSort(chains, elementSize, container);
		
		}
		return chains.first;
		// container = chains.first;
	};

	void	parsing(int argc, char **argv);
	void	execute(int argc, char **argv);
	static void	err(/* t_errors err,  */std::string msg);
};

// 15 numbers
//	first pend insertion pendEnd is last number in element but pendStart is 0
//		should be the same, probably needs to wrap when jacobsthal is bigger than pend
#endif