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
		size_t	_jacobsthal;
		size_t	_insertions;
		size_t	_comparisons;
		bool	_right;
	public:
		PmergeMe();
		PmergeMe(const PmergeMe& src);
		PmergeMe& operator=(const PmergeMe& other);
		~PmergeMe();
		void	updateJacobsthal(int& previous, int& current);

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

	/*----------------------------------------------------------*/
	/* prints container content									*/
	/* - when elementSize is not 0 it adds element boundaries	*/
	/*----------------------------------------------------------*/
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

		// identify insertion point
		// now determine the middle point between mainChain.begin() and end
		// loop
		//	find the middle of start and end in main Chain (has to be scaled with elementSize)
		//	compare the Value from pend (currentPend) to the middle Value
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
		std::cout << "\033[36m\n--------- identifyInsertionPoint ---------\033[0m" << std::endl;
		std::cout << "mainStart: " << *mainStart << std::endl;
		std::cout << "mainEnd: " << *mainEnd << std::endl;
		// maybe save the endpoint so that the
		while (std::distance(mainStart, mainEnd) >= 0)
		{
			long dist = std::distance(mainStart, mainEnd);
			long distToMiddle = (dist / 2 / elementSize) * elementSize;
			auto mainMiddle = std::next(mainStart, distToMiddle);
			std::cout << "mainMiddle: " << *mainMiddle << std::endl;
			std::cout << "currentPend: " << *currentPend << std::endl;
			std::cout << "dist: " << dist << std::endl;

			if (*currentPend < *mainMiddle)
			{
				_comparisons++;
				std::cout << "\033[34mnumber of comparisons: \033[0m" << _comparisons << std::endl;
				std::cout << "smaller: " << *currentPend << " < " << *mainMiddle << std::endl;
				_right = false;
				if (mainMiddle == mainStart)
					break ;
				mainEnd = std::prev(mainMiddle, elementSize);
				std::cout << "mainStart: " << *mainEnd << std::endl;
				std::cout << "mainEnd: " << *mainEnd << std::endl;
			}
			else if (*currentPend > *mainMiddle)
			{
				_comparisons++;
				std::cout << "\033[34mnumber of comparisons: \033[0m" << _comparisons << std::endl;
				_right = true;
				std::cout << "larger: " << *currentPend << " > " << *mainMiddle <<std::endl;
				if (mainMiddle == mainEnd)
					break ;
				mainStart = std::next(mainMiddle, elementSize);
				std::cout << "mainStart: " << *mainEnd << std::endl;
				std::cout << "mainEnd: " << *mainEnd << std::endl;
			}
			else
			{
				_right = true;
				_comparisons++;
				std::cout << "\033[34mnumber of comparisons: \033[0m" << _comparisons << std::endl;
				std::cout << "middle" << std::endl;
				return (mainMiddle++);
			}
		}
		return (mainStart);
	}

	template <typename T, typename Iterator>
	void	setStartingPoint(T& container, Iterator& start, int previous, unsigned long elementSize)
	{
		long iterations = (_jacobsthal - previous) * elementSize - 1;
		if (iterations >= static_cast<long>(container.size()))
			start = std::prev(container.end());
		else if (iterations < 0)
			start = container.begin();
		else
			start = std::next(container.begin(), iterations);
	}

	// binary search
	//	find the end of mainChain
	// the bound element ax / bx is the current Jacobsthal + number of insertions scaled by elementSize
	//		if no bound element compare if with the enire mainChain
	// identify the Insertion point and insert the element from the pend into the mainChain
	template <typename T, typename Iterator>
	void	binarySearch(T& mainChain, T& pend, unsigned long elementSize, Iterator currentPend)
	{
		std::cout << "\033[34m\n--------- binary Search ---------\033[0m" << std::endl;
		std::cout << "number of insertions:\t" << _insertions << std::endl;
		std::cout << "(jacobsthal + insertions) * elementSize - 1" << "\n" << "(" << _jacobsthal << "+" << _insertions << ") * " << elementSize << "- 1" << std::endl;
		std::cout << "=> " << (_jacobsthal + _insertions) * elementSize - 1 << std::endl;
		auto mainStart = std::next(mainChain.begin(), elementSize - 1);
		// the jacobsthal provides the index for b and a, after instering we need to account for this difference in mainChain, multiply by elementSize for scaling, -1 for last element of the block
		typename T::iterator mainEnd;
		setStartingPoint(mainChain, mainEnd, _insertions * (-1), elementSize);
		std::cout << "\033[34mdifference: [" << std::distance(mainStart, mainEnd) << "]\033[0m" << std::endl;
		if (std::distance(mainStart, mainEnd) % 2 != 0)
			mainEnd = std::prev(mainEnd, elementSize);
		// when mainStart until mainEnd is an even distance than move one elementSize back

		std::cout << "start main chain: " << *mainStart << std::endl;
		std::cout << "end main chain: " << *mainEnd << std::endl;
		auto insertionPoint = identifyInsertionPoint(mainStart, mainEnd, currentPend, elementSize);
		std::cout << "\033[34mmainMiddle chain: \033[0m" << *insertionPoint << std::endl;

		auto insertStart = std::prev(currentPend, elementSize -1);
		auto insertEnd = std::next(currentPend);

		typename T::iterator insertionTarget;
		if (_right)
			insertionTarget = std::next(insertionPoint);
		else
			insertionTarget = std::prev(insertionPoint, elementSize - 1);
		mainChain.insert(insertionTarget, insertStart, insertEnd);
		pend.erase(insertStart, insertEnd);
		_insertions++;
		std::cout << "mainChain after Insetion:\t";
		printContainer(mainChain, elementSize);
		std::cout << "pend after Insetion:\t";
		printContainer(pend, elementSize);
	}

	/*------------------------------------------------------------------------------------------*/
	/* Method that locates the index of the value-to-insert based on Jacobsthal					*/
	/* 		-> if the index exceeds pend.size it becomes pend.end()								*/
	/* loop																						*/
	/* - calls binarySearch to identify insertion point in mainChain							*/
	/* - updates the index of the value-to-insert by iterating backwards scaled to elementSize	*/
	/* - when iterator reaches the begining of the pend (here end) updates Jacobsthal			*/
	/* - then updates the index of the value-to-insert based on Jacobsthal						*/
	/* => b3 -> b2 -> b5 -> b4 -> b11 -> b10 -> b9 etc.											*/
	/* Inserts the Rest of container at the end of mainChain									*/
	/*------------------------------------------------------------------------------------------*/
	template <typename T>
	void	binaryInsertionSort(std::pair<T, T> &chains, unsigned long elementSize, T& container)
	{
		std::cout << "\033[32m\n============== Step 3 ==============\033[0m" << std::endl;
		T& mainChain = chains.first;
		T& pend = chains.second;

		int current = 1;
		int	previous = 1;
		if (pend.size() < 1)
			return ;
		updateJacobsthal(previous, current);
		typename T::iterator start;
		std::cout << "start index is:\n\t(" << _jacobsthal << " - " << previous << ") * " << elementSize << " - 1" << std::endl;
		std::cout << "=> " << (_jacobsthal - previous) * elementSize - 1 << std::endl;
		setStartingPoint(pend, start, previous, elementSize);
		for (auto end = std::next(pend.begin(), elementSize - 1); start >= end && pend.size() > 0;)
		{
			std::cout << "\033[32mpend.size(): \033[0m" << pend.size() << std::endl;
			std::cout << "startPend: " << *start << std::endl;
			std::cout << "endPend: " << *end << std::endl;
			binarySearch(mainChain, pend, elementSize, start);
			if (start == end)
			{
				updateJacobsthal(previous, current);
				std::cout << "updating jacobsthal to: " << _jacobsthal << std::endl;
				std::cout << "updating previous jacobsthal to: " << previous << std::endl;
				setStartingPoint(pend, start, previous, elementSize);
			}
			else
				start = std::prev(start, elementSize);
		}
		std::cout << "\033[32mcontainer.size(): \033[0m" << container.size() << std::endl;
		std::cout << "\033[32mmainChain.size(): \033[0m" << mainChain.size() << std::endl;
		auto restStart = container.end() - (container.size() - mainChain.size());
		std::cout << "\033[33mrestStart value: \033[0m" << *restStart << std::endl;

		mainChain.insert(mainChain.end(), restStart, container.end());
		container = mainChain;
		_insertions = 0;
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

		std::cout << "container:\t";
		printContainer(container, elementSize);
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

	/*------------------------------------------------------------------------------------------------------*/
	/* Recursive template Method for Sorting the container 													*/
	/* - creates pairs based on elementSize																	*/
	/* - compares last digits inside pairs if B > A the pairs are swapped									*/
	/* - calls FordJohnson recursively with elementSize * 2 if at least 2 Elements can be formed			*/
	/* - when highest recursion level is reached it calls createChain and binaryInsertionSort alternately	*/
	/* 	-> createChain creates the mainChain and the pend													*/
	/* 	-> binaryInsertionSort inserts elements from the pend into the mainChain							*/
	/* - when recursion is resolved it returns the sorted container											*/
	/*------------------------------------------------------------------------------------------------------*/
	template <typename T>
	T	FordJohnson(T& container, long elementSize, int counter)
	{
		std::cout << "\033[35\nm============== Step 1 ==============\033[0m" << std::endl;
		unsigned long	nbrOfElements = container.size() / elementSize;
		std::cout << "\033[35mnumber of Elements: " << nbrOfElements << " // elementSize [" << elementSize << "]\033[0m" << std::endl;

		std::cout << "unsorted container:\t";
		printContainer(container, elementSize);

		for (auto it = container.begin(); std::distance(it, container.end()) >= 2 * elementSize; it += 2 * elementSize)
		{
			auto itA = it;
			auto itB = it + elementSize;
			auto& lastA = *(itA + elementSize - 1);
			auto& lastB = *(itB + elementSize - 1);
			if (lastB < lastA)
			{
				std::swap_ranges(itA, itA + elementSize, itB);
			}
			_comparisons++;
			std::cout << "\033[34mnumber of comparisons: \033[0m" << _comparisons << std::endl;
		}
		_insertions = 0;
		std::cout << "sorted container:\t";
		printContainer(container, elementSize);
		std::pair<T, T> chains;
		if (static_cast<unsigned long>(elementSize) * 2 <= container.size())
		{
			FordJohnson(container, elementSize * 2, counter + 1);
			std::cout << "\033\n[31mrecursion level [" << counter << "]\033[0m" << std::endl;
			chains = createChains(container, elementSize);
			binaryInsertionSort(chains, elementSize, container);
		}
		return (chains.first);
	};

	void	parsing(int argc, char **argv);
	void	execute(int argc, char **argv);
	static void	err(std::string msg);
};

#endif