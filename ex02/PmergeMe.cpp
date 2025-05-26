/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhuss <dhuss@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:53:22 by dhuss             #+#    #+#             */
/*   Updated: 2025/05/22 15:09:21 by dhuss            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

//----------------Constructors-----------------//

/*----------------------*/
/* Constructor			*/
/*----------------------*/
PmergeMe::PmergeMe() : _jacobsthal(3), _insertions(0), _comparisons(0), _right(true)
{
}

/*----------------------*/
/* Copy Constructor		*/
/*----------------------*/
PmergeMe::PmergeMe(const PmergeMe& src) : _jacobsthal(src._jacobsthal), _insertions(src._insertions), _comparisons(src._comparisons), _right(src._right)
{
	_vector = src._vector;
	_deque = src._deque;
}

/*------------------------------------------*/
/* Overloaded Assignment operator			*/
/*------------------------------------------*/
PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
	if (this != &other)
	{
		_jacobsthal = other._jacobsthal;
		_insertions = other._insertions;
		_comparisons = other._comparisons;
		_right = other._right;
		_vector = other._vector;
		_deque = other._deque;
	}
	return (*this);
}

/*----------------------*/
/* Destructor			*/
/*----------------------*/
PmergeMe::~PmergeMe()
{
}

//----------------Exceptions-----------------//

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

//----------------helper-----------------//

/*----------------------------------------------*/
/* returns true if only digits preceding symbol	*/
/*----------------------------------------------*/
bool	only_digits(std::string str)
{
	size_t start = 0;
	if (str[0] == '-' || str[0] == '+')
		start = 1;
	if (start == str.length())
		return (false);
	return (std::all_of(str.begin() + start, str.end(), ::isdigit));
}

//----------------Member Methods-----------------//

/*------------------------------*/
/* updates Jacobsthal Number	*/
/*------------------------------*/
void	PmergeMe::updateJacobsthal(int& previous, int& current)
{
	_jacobsthal = 2 * previous + current;
	previous = current;
	current = _jacobsthal;
}

bool	PmergeMe::alreadySorted()
{
	if (_vector.size() < 2)
	{
		std::cout << "\033[36mThe sequence consists of only one number. Try adding more numbers...\033[0m" << std::endl;
		_vector.clear();
		_deque.clear();
		return (true);
	}
	if (is_sorted(_vector.begin(), _vector.end()))
	{
		std::cout << "\033[36mThe sequence is already sorted. Try an unsorted sequence...\033[0m" << std::endl;
		_vector.clear();
		_deque.clear();
		return (true);
	}
	return (false);
}

/*--------------------------------*/
/* parses user input			  */
/*--------------------------------*/
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
	if (alreadySorted())
		return ;
	std::cout << "\033[32mUnsorted:\t\033[0m";
	printContainer(_deque, 0);

	_comparisons = 0;
	auto	start = std::chrono::high_resolution_clock::now();
	auto sorted = FordJohnson(_vector, 1, 1);
	auto	end = std::chrono::high_resolution_clock::now();
	auto	duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

	std::cout << "\033[32m\nsorted:\033[0m\t";
	printContainer(sorted, 0);

	std::cout << "Time to process a range of " << _vector.size() << " elements with std::vector : " << duration << " us" << std::endl;
	if (is_sorted(sorted.begin(), sorted.end()))
		std::cout << "\033[32mVector container is sorted!\033[0m" << std::endl;
	else
		std::cout << "\033[31mVector container is not sorted!\033[0m" << std::endl;
	std::cout << "The number of comparisons is: " << _comparisons << std::endl;

	auto tmp = _comparisons;
	_comparisons = 0;
	auto	startDeq = std::chrono::high_resolution_clock::now();
	auto sortedDeq = FordJohnson(_deque, 1, 1);
	auto	endDeq = std::chrono::high_resolution_clock::now();
	auto	durationDeq = std::chrono::duration_cast<std::chrono::microseconds>(endDeq - startDeq).count();

	std::cout << "Time to process a range of " << _deque.size() << " elements with std::deque : " << durationDeq << " us" << std::endl;
	if (is_sorted(sortedDeq.begin(), sortedDeq.end()))
		std::cout << "\033[32mDeque container is sorted!\033[0m" << std::endl;
	else
		std::cout << "\033[31mDeque container is not sorted!\033[0m" << std::endl;
	std::cout << "The number of comparisons is: " << _comparisons << std::endl;
	if (tmp == _comparisons)
		std::cout << "\033[32mComparisons match\033[0m" << std::endl;
	else
		std::cout << "\033[31mComparisons differ\033[0m" << std::endl;
	_vector.clear();
	_deque.clear();
	_jacobsthal = 3;
	_insertions = 0;
	_insertionsJacobsthal = 0;
	_comparisons = 0;
	_right = true;
	_rightCounter = 0;
	_leftCounter = 0;
	_rightOfAx = true;
	_lastRange = 0;
	_greaterJacob = 0;
}

/*--------------------------*/
/* handles errors			*/
/*--------------------------*/
void	PmergeMe::err(std::string msg)
{
	std::cerr << "\033[31mError: " << msg << "\033[0m" << std::endl;
	exit(EXIT_FAILURE);
}
