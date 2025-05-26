/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhuss <dhuss@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 16:09:55 by dhuss             #+#    #+#             */
/*   Updated: 2025/05/22 16:43:53 by dhuss            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		size_t	_insertionsJacobsthal = 0;
		size_t	_comparisons;
		bool	_right;
		size_t	_rightCounter = 0;
		size_t	_leftCounter = 0;
		bool	_rightOfAx = true;
		size_t	_lastRange = 0;
		size_t	_greaterJacob = 0;
	public:
		PmergeMe();
		PmergeMe(const PmergeMe& src);
		PmergeMe& operator=(const PmergeMe& other);
		~PmergeMe();
		void	updateJacobsthal(int& previous, int& current);
		void	parsing(int argc, char **argv);
		bool	alreadySorted();
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

	//----------------template Methods-----------------//

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

	/*----------------------------------------------------------*/
	/* prints position of elements								*/
	/*----------------------------------------------------------*/
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

	/*------------------------------------------------------------------------------------------*/
	/* identifies the insertion point															*/
	/* loop																						*/
	/*	find the middle of start and end in main Chain (has to be scaled with elementSize)		*/
	/*	compare the Value from pend (currentPend) to the middle Value							*/
	/*	if smaller: range is to the left of target value										*/
	/*		mainChain end becomes middle Value - elementSize									*/
	/*		mainChain start remains the same													*/
	/*	if larger: range is to the right of the target value									*/
	/*		main Chain end remains the same														*/
	/*		main Chain Start is middle Value + elementSize										*/
	/*	if the same insert it after the middle Value											*/
	/*------------------------------------------------------------------------------------------*/
	template <typename Iterator>
	auto identifyInsertionPoint(Iterator mainStart, Iterator mainEnd, Iterator &currentPend, unsigned long elementSize)
	{
		while (std::distance(mainStart, mainEnd) >= 0)
		{
			long dist = std::distance(mainStart, mainEnd);
			long distToMiddle = (dist / 2 / elementSize) * elementSize;
			auto mainMiddle = std::next(mainStart, distToMiddle);

			if (*currentPend < *mainMiddle)
			{
				_comparisons++;
				_right = false;
				if (mainMiddle == mainStart)
					break ;
				mainEnd = std::prev(mainMiddle, elementSize);
			}
			else if (*currentPend > *mainMiddle)
			{
				_comparisons++;
				_right = true;
				if (mainMiddle == mainEnd)
					break ;
				mainStart = std::next(mainMiddle, elementSize);
			}
			else
			{
				_right = true;
				_comparisons++;
				return (mainMiddle++);
			}
		}
		return (mainStart);
	}

	/*--------------------------------------------------------------------------*/
	/* Sets the starting point for the passed iterator for the passed container */
	/* - For mainChain 															*/
	/*		-> previous: the number of insertions * -1							*/
	/*		-> start: the bound element of the current pend (b5 -> a5)			*/
	/*		-> start is calculated by current jacobsthal + number of insertions	*/
	/*		   scaled by elementsize											*/
	/*		-> if the number is greater than the size of the contaienr it = end	*/
	/* - For Pend 																*/
	/* 		-> previous: is the previous jacobsthal number						*/
	/*		-> start: the current pend value to be inserted						*/
	/*		-> start is calculated by current jacobsthal - previous jacbosthal	*/
	/*		   scaled by elementsize											*/
	/*--------------------------------------------------------------------------*/
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
	void	binarySearch(T& mainChain, T& pend, unsigned long elementSize, Iterator &currentPend)
	{
		auto mainStart = std::next(mainChain.begin(), elementSize - 1);
		typename T::iterator mainEnd;
		typename T::iterator Ax;
		if (_rightOfAx)
			setStartingPoint(mainChain, mainEnd, _insertions * (-1), elementSize);
		else
			mainEnd = std::next(mainStart, _lastRange);
		if (_rightCounter == 0)
			Ax = mainEnd;
		else
			Ax = std::prev(mainEnd, (_rightCounter) * elementSize); // ax is not before mainEnd if inserted right of ax before
		_lastRange = std::distance(mainStart, mainEnd);
		auto insertionPoint = identifyInsertionPoint(mainStart, mainEnd, currentPend, elementSize);
		if (std::distance(Ax, insertionPoint) > 0)
		{
			_rightCounter++;
			_rightOfAx = true;
		}
		else
		{
			_leftCounter++;
			_rightOfAx = false;
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
		_insertionsJacobsthal++;
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
		T& mainChain = chains.first;
		T& pend = chains.second;

		int current, previous = 1;
		if (pend.size() < 1)
			return ;
		updateJacobsthal(previous, current);
		typename T::iterator start;
		setStartingPoint(pend, start, previous, elementSize);
		for (auto end = std::next(pend.begin(), elementSize - 1); start >= end && pend.size() > 0;)
		{
			binarySearch(mainChain, pend, elementSize, start);
			long	offSet = (_jacobsthal - previous - _insertionsJacobsthal) * elementSize - 1;
			if (offSet < 0)
			{
				updateJacobsthal(previous, current);

				setStartingPoint(pend, start, previous, elementSize);

				end = std::next(pend.begin(), elementSize - 1);
				_rightOfAx = true;
				_leftCounter = 0;
				_rightCounter = 0;
				_insertionsJacobsthal = 0;
				_greaterJacob = 0;
				if ((_jacobsthal - previous) * elementSize > pend.size())
					_greaterJacob = ((_jacobsthal - previous) * elementSize) - pend.size();
			}
			else if (static_cast<size_t>(offSet) - _greaterJacob > pend.size())
				start = pend.begin();
			else
				start = std::next(pend.begin(), offSet - _greaterJacob);
		}
		auto restStart = container.end() - (container.size() - mainChain.size());

		mainChain.insert(mainChain.end(), restStart, container.end());
		container = mainChain;
		_insertions = 0;
		_insertionsJacobsthal = 0;
		_leftCounter = 0;
		_rightCounter = 0;
		_rightOfAx = true;
		_greaterJacob = 0;
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
		T	mainChain;
		T	pend;

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
		for (auto it = container.begin(); std::distance(it, container.end()) >= 2 * elementSize; it += 2 * elementSize)
		{
			auto itA = it;
			auto itB = it + elementSize;
			auto& lastA = *(itA + elementSize - 1);
			auto& lastB = *(itB + elementSize - 1);
			if (lastB < lastA)
				std::swap_ranges(itA, itA + elementSize, itB);
			_comparisons++;
		}
		_insertions = 0;
		std::pair<T, T> chains;
		if (static_cast<unsigned long>(elementSize) * 2 <= container.size())
		{
			FordJohnson(container, elementSize * 2, counter + 1);
			chains = createChains(container, elementSize);
			binaryInsertionSort(chains, elementSize, container);
		}
		return (chains.first);
	};

};

#endif