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
		size_t	_rightCounter = 0;
		size_t	_leftCounter = 0;
		bool	_rightOfAx = true;
		size_t	_lastRange = 0;
	public:
		PmergeMe();
		PmergeMe(const PmergeMe& src);
		PmergeMe& operator=(const PmergeMe& other);
		~PmergeMe();
		void	updateJacobsthal(int& previous, int& current);
		void	parsing(int argc, char **argv);
		void	execute(int argc, char **argv);
		static void	err(std::string msg);

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

	template <typename T>
	void	printPos(T& container, unsigned long elementSize, int pos)
	{
		size_t index = 0;
		(void) elementSize;
		for (auto it = container.begin(); it != container.end(); it++, index++)
		{
			std::cout << "\033[32m | \033[0m";
			if (pos == 0)
			{
				if (index == 0)
					std::cout << "\033[31mb1" << "\033[0m";
				else
					std::cout << "\033[31ma" << index << "\033[0m";
			}
			else if (pos == 1)
				std::cout << "\033[33mb" << index + 2 << "\033[0m";
			else
			{
				if (index % 2 == 0)
					std::cout << "\033[33mb" << index + 1 << "\033[0m";
				else
					std::cout << "\033[31ma" << index + 1 << "\033[0m";
			}
			for (unsigned long i = 1; i < elementSize; i++)
				std::cout << " ";
			int length = std::to_string(*it).length();
			for (int j = 1; j < length; j++)
				std::cout << " ";
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
		std::cout << "currentPend: " << *currentPend << std::endl;
		// std::cout << "(jacobsthal - leftCounter - 1) * elementsize => (" << _jacobsthal << " - " << _leftCounter << " - 1) *" << elementSize << std::endl;

		while (std::distance(mainStart, mainEnd) >= 0)
		{
			long dist = std::distance(mainStart, mainEnd);
			long distToMiddle = (dist / 2 / elementSize) * elementSize;
			auto mainMiddle = std::next(mainStart, distToMiddle);
			std::cout << "mainMiddle: " << *mainMiddle << std::endl;
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
				// std::cout << "mainStart: " << *mainEnd << std::endl;
				// std::cout << "mainEnd: " << *mainEnd << std::endl;
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
				// std::cout << "mainStart: " << *mainEnd << std::endl;
				// std::cout << "mainEnd: " << *mainEnd << std::endl;
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
	void	setStartingPointPend(T& container, Iterator& start, int previous, unsigned long elementSize)
	{
		std::cout << "\033[33m\n--------- setStartingPoint ---------\033[0m" << std::endl;
		long iterations;

		iterations = (_jacobsthal - previous) * elementSize - 1;
		std::cout << "iterations [" << iterations << "]" << std::endl;
		if (iterations >= static_cast<long>(container.size()))
		{
			std::cout << "iterations are more than container size " << iterations << " > " << container.size() << std::endl;
			start = std::prev(container.end());
		}
		else if (iterations < 0)
			start = container.begin();
		else
		{
			std::cout << "iterations are smaller than container size " << iterations << " < " << container.size() << std::endl;
			start = std::next(container.begin(), iterations);
		}
		std::cout << "the start value is: " << *start << std::endl;
		std::cout << "\033[33m------------------\033[0m\n" << std::endl;
	}

	template <typename T, typename Iterator>
	void	setStartingPointMainChain(T& container, Iterator& start, int previous, unsigned long elementSize)
	{
		std::cout << "\033[33m\n--------- setStartingPoint ---------\033[0m" << std::endl;
		long iterations;
			iterations = (_jacobsthal - previous) * elementSize - 1;
		std::cout << "iterations [" << iterations << "]" << std::endl;
		if (iterations >= static_cast<long>(container.size()))
		{
			std::cout << "iterations are more than container size " << iterations << " > " << container.size() << std::endl;
			start = std::prev(container.end());
		}
		else if (iterations < 0)
			start = container.begin();
		else
		{
			std::cout << "iterations are smaller than container size " << iterations << " < " << container.size() << std::endl;
			start = std::next(container.begin(), iterations);
		}
		std::cout << "the start value is: " << *start << std::endl;
		std::cout << "\033[33m------------------\033[0m\n" << std::endl;
	}

	// binary search
	//	find the end of mainChain
	// the bound element ax / bx is the current Jacobsthal + number of insertions scaled by elementSize
	//		if no bound element compare if with the enire mainChain
	// identify the Insertion point and insert the element from the pend into the mainChain
	template <typename T, typename Iterator>
	void	binarySearch(T& mainChain, T& pend, unsigned long elementSize, Iterator currentPend)
	{
		// std::cout << "\033[34m\n--------- binary Search ---------\033[0m" << std::endl;
		auto mainStart = std::next(mainChain.begin(), elementSize - 1);
		typename T::iterator mainEnd;
		typename T::iterator Ax;
		std::cout << "leftCounter: " << _leftCounter << std::endl;
		std::cout << "RightCounter: " << _rightCounter << std::endl;
		// std::cout << "calling set startingPoint with mainChain" << std::endl;
		if (_rightOfAx)
		{
			// std::cout << " rightOfAx true\nnumber of insertions:\t" << _insertions << std::endl;
			setStartingPointMainChain(mainChain, mainEnd, _insertions * (-1), elementSize);
		}
		else
		{
			// std::cout << " rightOfAx true\nnumber of insertions:\t" << _insertions << std::endl;
			// setStartingPointMainChain(mainChain, mainEnd, _insertions * (-1), elementSize); //
			// if we insert left then the range remains the same so the end is ax-1
			mainEnd = std::next(mainStart, _lastRange);
		}
		if (_rightCounter == 0)
		{

				// std::cout << "2." << std::endl;
				Ax = mainEnd;
		}
		else
		{
			// if (_rightCounter > 0)
			// std::cout << "3." << std::endl;
			Ax = std::prev(mainEnd, (_rightCounter) * elementSize); // ax is not before mainEnd if inserted right of ax before
		}

		std::cout << "start main chain: " << *mainStart << std::endl;
		std::cout << "end main chain: " << *mainEnd << std::endl;
		std::cout << "\033[36mAx - 1 is: \033[0m" << *Ax << std::endl;
		std::cout << "\033[36mthe range between elements in mainChain is: \033[0m" << std::distance(mainStart, mainEnd) * elementSize << std::endl;
		_lastRange = std::distance(mainStart, mainEnd);


		auto insertionPoint = identifyInsertionPoint(mainStart, mainEnd, currentPend, elementSize);
		// std::cout << "\033[34mmainMiddle chain: \033[0m" << *insertionPoint << std::endl;

		if (std::distance(Ax, insertionPoint) > 0)
		{
			_rightCounter++;
			_rightOfAx = true;
			// std::cout << "rightCounter: [" << _rightCounter << "]" << std::endl;
		}
		else
		{
			_leftCounter++;
			_rightOfAx = false;
			// std::cout << "leftCounter: [" << _leftCounter << "]" << std::endl;
		}

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
		std::cout << "\npend after Insetion:\t\t";
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
		std::cout << "calling set startingPoint with pend" << std::endl;
		setStartingPointPend(pend, start, previous, elementSize);
		// auto end = std::next(pend.begin(), elementSize - 1);
		// while (pend.size() > 0 && start != pend.begin() && std::distance(end, start) >= 0) // this makes the three disapear for both algorithms after comparison 64
		// while (pend.size() > 0 && start != pend.begin() && std::distance(end, start) >= 0) // this prematurely quits the algorithm after comparison 64
		for (auto end = std::next(pend.begin(), elementSize - 1); start >= end && pend.size() > 0;)// after comparison 64 is this check invalid for deque?
		{
			std::cout << "\033[32mpend.size(): \033[0m" << pend.size() << std::endl;
			std::cout << "startPend: " << *start << std::endl;
			std::cout << "endPend: " << *end << std::endl;
			binarySearch(mainChain, pend, elementSize, start);
			if (start == end)
			{
				updateJacobsthal(previous, current);
				std::cout << "updating jacobsthal to: " << _jacobsthal << std::endl;
				// std::cout << "updating previous jacobsthal to: " << previous << std::endl;
				// std::cout << "calling set startingPoint with pend" << std::endl;
				setStartingPointPend(pend, start, previous, elementSize);
				end = std::next(pend.begin(), elementSize - 1); 
				_rightOfAx = true;
				_leftCounter = 0;
				_rightCounter = 0;
			}
			else
			{

				std::cout << "moving back in pend... start before prev: " << *start << std::endl;
				start = std::prev(start, elementSize); // after comparison 64 did deque go here?
				std::cout << "start after prev: " << *start << std::endl;
			}
		}
		// std::cout << "\033[32mcontainer.size(): \033[0m" << container.size() << std::endl;
		// std::cout << "\033[32mmainChain.size(): \033[0m" << mainChain.size() << std::endl;
		auto restStart = container.end() - (container.size() - mainChain.size());
		std::cout << "\033[33mrestStart value: \033[0m" << *restStart << std::endl;

		mainChain.insert(mainChain.end(), restStart, container.end());
		container = mainChain;
		_insertions = 0;
		_leftCounter = 0;
		_rightCounter = 0;
		_rightOfAx = true;
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
		std::cout << "\nmainChain:\t";
		printContainer(mainChain, elementSize);
		std::cout << "mainChain:\t";
		printPos(mainChain, elementSize, 0);
		std::cout << "\nthe pend:\t";
		printContainer(pend, elementSize);
		std::cout << "the pend:\t";
		printPos(pend, elementSize, 1);
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
		std::cout << "unsorted container:\t";
		printPos(container, elementSize, 3);

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
		std::cout << "sorted container:\t";
		printPos(container, elementSize, 3);
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
};

#endif