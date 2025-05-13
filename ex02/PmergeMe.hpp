#ifndef PMERGEME_HPP
# define PMERGEME_HPP

#include <iostream>
#include <deque>
#include <vector>
#include <algorithm>

typedef enum e_errors
{
	E_ARGS,
} t_errors;

class PmergeMe
{
	private:
		std::vector<int> _container;
	public:
		PmergeMe();
		~PmergeMe();

		void	execute(char **argv);
		static void	err(t_errors err);
};


#endif