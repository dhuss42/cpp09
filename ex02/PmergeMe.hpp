#ifndef PMERGEME_HPP
# define PMERGEME_HPP

#include <iostream>
#include <deque>
#include <vector>
#include <algorithm>


class PmergeMe
{
	private:
		std::vector<int> _container;
	public:
		PmergeMe();
		~PmergeMe();

	typedef enum e_errors
	{
		E_ARGS,
	} t_errors;

	void	execute(char **argv);
	static void	err(t_errors err);
};


#endif