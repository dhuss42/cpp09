
#include "PmergeMe.hpp"

int	main(int argc, char **argv)
{
	(void) argv;
	if (argc < 2)
		PmergeMe::err(E_ARGS);
	else
	{
		PmergeMe pmM;
		pmM.execute(argv);
	}
}